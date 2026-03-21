#!/usr/bin/env python3
"""
validate_xll.py  –  Validate mgxll.xll after a Windows build.

What this script checks
-----------------------
1. The XLL file exists and is non-empty.
2. Its PE structure is intact (Windows: LoadLibraryEx with LOAD_LIBRARY_AS_DATAFILE).
3. The PE machine architecture matches the expected bitness (Win32 vs x64).
4. It exports every required symbol (parsed from the PE export directory).
5. *Optionally*, if Excel is installed and the ``xlwings`` package is available,
   it loads the XLL into a live Excel session and calls a smoke-test function.

Usage
-----
    # CI / headless (no Excel required):
    python tests/validate_xll.py --xll bin\\win\\Release\\mgxll.xll

    # Explicit architecture check:
    python tests/validate_xll.py --xll bin\\win\\Release\\mgxll.xll --arch win32
    python tests/validate_xll.py --xll bin\\win\\x64\\Release\\mgxll.xll --arch x64

    # Verbose: print all exported symbols:
    python tests/validate_xll.py --xll bin\\win\\Release\\mgxll.xll --verbose

    # Interactive / manual (with Excel installed):
    pip install xlwings
    python tests/validate_xll.py --xll bin\\win\\x64\\Release\\mgxll.xll --live

Exit codes
----------
0  all mandatory checks pass
1  at least one check failed
"""

from __future__ import annotations

import argparse
import os
import struct
import sys
from typing import List, Optional

# ── required XLL exports ────────────────────────────────────────────────────
# These are the C-exported names as they appear in the PE export table.
REQUIRED_EXPORTS: List[str] = [
    "xlAutoOpen",
    "xlAutoClose",
    "xlJustATest",
    "xlRobot",
    "xlPrice",
    "xlDfModel",
    "xlBSModel",
    "xlZeroCurve_Create",
    "xlComputeZeroRate",
    "xlVolatilityCurve_Create",
    "xlComputeVolatility",
    "xlDividendsTable_Create",
    "xlComputeDiscountedDivs",
    "xlGenSec_Create",
    "xlRandGen_Create",
    "xlNextBusinessDay",
    "xlPreviousBusinessDay",
    "xlBetweenDates",
    "xlAddPeriod",
    "xlIRIndex_Create",
    "xlSchedule_Create",
    "xlSchedule_GetData",
    "xlTermStructure_Create",
    "xlTermStructure_Compute",
    "xlSwapLeg_Create",
    "xlCallPut_Create",
]

# PE machine constants
_MACHINE_I386  = 0x014C  # Win32 / x86
_MACHINE_AMD64 = 0x8664  # x64 / AMD64

# ── PE export-table parser ──────────────────────────────────────────────────

def parse_pe_exports(dll_path: str) -> List[str]:
    """Return the list of exported names from a PE DLL/XLL without loading it."""
    with open(dll_path, "rb") as fh:
        data = fh.read()

    if data[:2] != b"MZ":
        raise ValueError("Not a valid PE file (MZ header missing)")

    pe_off = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe_off : pe_off + 4] != b"PE\x00\x00":
        raise ValueError("Invalid PE signature")

    machine = struct.unpack_from("<H", data, pe_off + 4)[0]
    is_pe32_plus = machine == _MACHINE_AMD64

    num_sections   = struct.unpack_from("<H", data, pe_off + 6)[0]
    opt_hdr_sz     = struct.unpack_from("<H", data, pe_off + 20)[0]
    opt_hdr_off    = pe_off + 24

    # Data-directory index 0 == Export directory
    dd_off = opt_hdr_off + (112 if is_pe32_plus else 96)
    export_rva = struct.unpack_from("<I", data, dd_off)[0]
    if export_rva == 0:
        return []

    # Find the section that contains the export directory
    sect_off = opt_hdr_off + opt_hdr_sz
    sec_vaddr = sec_raw = None
    for i in range(num_sections):
        s = sect_off + i * 40
        vaddr = struct.unpack_from("<I", data, s + 12)[0]
        vsz   = struct.unpack_from("<I", data, s + 16)[0]
        raw   = struct.unpack_from("<I", data, s + 20)[0]
        if vaddr <= export_rva < vaddr + vsz:
            sec_vaddr, sec_raw = vaddr, raw
            break

    if sec_vaddr is None:
        return []

    def rva2off(rva: int) -> int:
        return sec_raw + (rva - sec_vaddr)

    exp_off  = rva2off(export_rva)
    num_names = struct.unpack_from("<I", data, exp_off + 24)[0]
    names_rva = struct.unpack_from("<I", data, exp_off + 32)[0]
    names_off = rva2off(names_rva)

    exports: List[str] = []
    for i in range(num_names):
        name_rva = struct.unpack_from("<I", data, names_off + i * 4)[0]
        name_off = rva2off(name_rva)
        end = data.index(b"\x00", name_off)
        exports.append(data[name_off:end].decode("ascii", errors="replace"))

    return exports


def get_pe_machine(dll_path: str) -> int:
    """Return the PE Machine field value (e.g. 0x014C for x86, 0x8664 for x64)."""
    with open(dll_path, "rb") as fh:
        data = fh.read(0x100)  # only need the headers

    if data[:2] != b"MZ":
        raise ValueError("Not a valid PE file (MZ header missing)")
    pe_off = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe_off : pe_off + 4] != b"PE\x00\x00":
        raise ValueError("Invalid PE signature")
    return struct.unpack_from("<H", data, pe_off + 4)[0]


# ── LoadLibraryEx check (Windows only) ─────────────────────────────────────

def verify_loads_as_datafile(dll_path: str) -> bool:
    """Verify the XLL is a valid Windows DLL by loading it as a data file."""
    import ctypes
    LOAD_LIBRARY_AS_DATAFILE = 0x00000002
    k32 = ctypes.windll.kernel32  # type: ignore[attr-defined]
    handle = k32.LoadLibraryExW(dll_path, None, LOAD_LIBRARY_AS_DATAFILE)
    if handle:
        k32.FreeLibrary(handle)
        return True
    err = k32.GetLastError()
    print(f"  LoadLibraryExW error code: {err}")
    return False


# ── live Excel smoke test ───────────────────────────────────────────────────

def live_excel_test(xll_path: str) -> bool:
    """
    Load the XLL in a live Excel session and call MG_JustATest.

    Requires: Excel installed + ``pip install xlwings``.
    """
    import xlwings as xw  # noqa: F401 – imported to surface ImportError early

    print("\n[live] Starting Excel …")
    try:
        with xw.App(visible=False, add_book=False) as app:
            # Register the XLL
            rc = app.api.RegisterXLL(xll_path)
            if not rc:
                print(f"  FAIL: RegisterXLL returned False for {xll_path}")
                return False
            print(f"  PASS: RegisterXLL({xll_path!r}) succeeded")

            wb = app.books.add()
            ws = wb.sheets[0]

            # MG_JustATest echoes the date passed to it
            test_date = "2024-01-15"
            ws["A1"].value = test_date
            ws["B1"].formula = "=MG_JustATest(A1)"
            result = ws["B1"].value

            wb.close(save=False)

            if result is None:
                print("  FAIL: MG_JustATest returned None")
                return False

            print(f"  PASS: MG_JustATest({test_date!r}) → {result!r}")
    except Exception as exc:
        print(f"  FAIL: Excel session raised: {exc}")
        return False

    return True


# ── main ────────────────────────────────────────────────────────────────────

def _separator(title: str) -> None:
    print(f"\n{'─' * 60}")
    print(f"  {title}")
    print(f"{'─' * 60}")


def main(argv: Optional[List[str]] = None) -> int:
    parser = argparse.ArgumentParser(
        description="Validate mgxll.xll – exports and optional live Excel test",
    )
    parser.add_argument(
        "--xll",
        default=r"bin\win\Release\mgxll.xll",
        help="Path to mgxll.xll (default: %(default)s)",
    )
    parser.add_argument(
        "--arch",
        choices=["win32", "x64"],
        help="Expected PE architecture: 'win32' (x86) or 'x64'. "
             "When omitted the check is skipped.",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Print every exported symbol found in the XLL.",
    )
    parser.add_argument(
        "--live",
        action="store_true",
        help="Run a live Excel smoke-test (requires Excel + xlwings)",
    )
    args = parser.parse_args(argv)

    xll_path = os.path.abspath(args.xll)
    failures = 0

    # ── 1. File exists ───────────────────────────────────────────────────────
    _separator("1. File check")
    if not os.path.isfile(xll_path):
        print(f"FAIL: XLL not found: {xll_path}")
        return 1
    size = os.path.getsize(xll_path)
    print(f"PASS: {xll_path}")
    print(f"      size = {size:,} bytes")

    # ── 2. PE structure (Windows only) ──────────────────────────────────────
    if sys.platform == "win32":
        _separator("2. PE load check (LOAD_LIBRARY_AS_DATAFILE)")
        if verify_loads_as_datafile(xll_path):
            print("PASS: DLL structure is valid")
        else:
            print("FAIL: DLL failed to load as data file")
            failures += 1
    else:
        print("\n[skip] PE load check (non-Windows platform)")

    # ── 3. Architecture check ────────────────────────────────────────────────
    _separator("3. PE architecture check")
    try:
        machine = get_pe_machine(xll_path)
    except Exception as exc:
        print(f"FAIL: Could not read PE machine field: {exc}")
        failures += 1
        machine = None

    if machine is not None:
        arch_name = {
            _MACHINE_I386:  "Win32 (x86, 0x014C)",
            _MACHINE_AMD64: "x64  (AMD64, 0x8664)",
        }.get(machine, f"Unknown (0x{machine:04X})")
        print(f"PE Machine: {arch_name}")

        if args.arch:
            expected_machine = _MACHINE_I386 if args.arch == "win32" else _MACHINE_AMD64
            if machine == expected_machine:
                print(f"PASS: architecture matches --arch {args.arch}")
            else:
                print(f"FAIL: expected --arch {args.arch} but got {arch_name}")
                failures += 1
        else:
            print("(no --arch flag supplied; architecture not enforced)")

    # ── 4. Export table ──────────────────────────────────────────────────────
    _separator("4. Export table check")
    try:
        exports = parse_pe_exports(xll_path)
    except Exception as exc:
        print(f"FAIL: Could not parse PE export table: {exc}")
        return 1

    export_set = set(exports)
    print(f"Found {len(exports)} exported symbol(s) in total.")

    if args.verbose:
        print("\nAll exported symbols:")
        for sym in sorted(exports):
            print(f"  {sym}")
        print()

    missing: List[str] = []
    for name in REQUIRED_EXPORTS:
        if name in export_set:
            print(f"  OK  {name}")
        else:
            print(f"  MISSING  {name}")
            missing.append(name)

    if missing:
        print(f"\nFAIL: {len(missing)} required export(s) absent: {missing}")
        failures += 1
    else:
        print(f"\nPASS: All {len(REQUIRED_EXPORTS)} required exports present.")

    # ── 5. Optional live Excel test ──────────────────────────────────────────
    if args.live:
        _separator("5. Live Excel smoke test")
        try:
            ok = live_excel_test(xll_path)
        except ImportError:
            print("SKIP: xlwings not installed.")
            print("      Install with:  pip install xlwings")
        else:
            if not ok:
                failures += 1

    # ── summary ──────────────────────────────────────────────────────────────
    _separator("Summary")
    if failures == 0:
        print("ALL CHECKS PASSED")
    else:
        print(f"FAILED: {failures} check(s) did not pass")

    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
