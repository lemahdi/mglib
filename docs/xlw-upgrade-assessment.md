# XLW Migration — XLW 4.0.0f0 → XLW 6.0.0f0

## Status: **COMPLETED**

MGlib has been upgraded from **XLW 4.0.0f0** (2009) to **XLW 6.0.0f0**
(GitHub master, https://github.com/xlw/xlw).

---

## What Changed

### `src/xlw/` — XLW library sources replaced

| Old (4.0.0f0) | New (6.0.0f0) | Notes |
|---------------|---------------|-------|
| `XlfOper.h/.cpp` (monolithic, `XlfOperImpl` delegation) | `XlfOper.h` (header-only template, XLOPER12 native) | API-compatible for callers |
| `XlfOper4.h/.cpp`, `XlfOper12.h/.cpp` | Removed — merged into single `XlfOper` | `typedef XlfOper XlfOper12` kept for compat |
| `XlfOperImpl.h`, `XlfOperImpl4.h/.cpp`, `XlfOperImpl12.h/.cpp` | `XlfOperImpl.cpp` only (no header) | Implementation detail |
| `CellMatrix.cpp` | Header-only pimpl (`CellMatrixPimpl.h`, `CellValue.h`) | No `.cpp` needed |
| `MJmatrices.h/.cpp` | `MJCellMatrix.h/.cpp` | Renamed |
| `Dispatcher.cpp`, `FileConverter.cpp` | Removed | Merged upstream |
| `XlfException.cpp`, `XlfStr.h/.cpp` | Removed | Inlined into `XlfOper.h` |
| `XlfMutex.h` | `CriticalSection.h`, `ThreadLocalStorage.h` | Replaced |
| `macros.h` (`EXCEL_BEGIN` uses `FreeMemory()`) | `macros.h` (`EXCEL_BEGIN` uses `UsesTempMemory`) | Semantic change — same effect |
| `wrapper.h`, `ERR_Macros.h` | Removed | Unused |
| `ArgListFactory.h`, `ArgListFactoryHelper.h` | Removed | No longer needed |
| `mgw/` (cpp_xloper, xloper/xloper12 wrappers) | Removed | Replaced by `XlfServices` |
| `clw/` (Dispatcher, FileConverter) | Removed | Merged into core sources |

### New files added

| File | Purpose |
|------|---------|
| `TempMemory.h/.cpp` | Per-call Excel memory arena (used by `UsesTempMemory` in `EXCEL_BEGIN`) |
| `XlfServices.h/.cpp` | High-level Excel service functions (`GetCallingCell`, `GetSheetName`, …) |
| `XlfOperProperties.h` | XLOPER12 field accessors |
| `XlfWindows.h` | Windows SDK include with proper version guards |
| `CriticalSection.h`, `ThreadLocalStorage.h` | Thread-safety helpers |
| `Singleton.h` | XLW-internal singleton helper |
| `eshared_ptr.h`, `eshared_ptr_details.h` | Enhanced shared pointer |
| `PascalStringConversions.h/.cpp` | Pascal-string ↔ `std::string` |
| `HiResTimer.h/.cpp` | High-resolution timer |
| `NCmatrices.h/.cpp` | Non-copyable matrix type |

### MGlib-specific extensions preserved

| Extension | Location | Notes |
|-----------|----------|-------|
| `XlfOper::AsMGDate()` | `src/xlw/XlfOperMG.cpp` | Converts Excel serial-date → `MG_Date` |
| `XlfOper::AsMGGenDate()` | `src/xlw/XlfOperMG.cpp` | Converts string or serial-date → `MG_GenericDate` |
| `XlfOper::AsMGXLObject()` | `src/xlw/XlfOperMG.cpp` | Looks up cached `MG_XLObject` by reference string |
| `MG_Cache` / `MG_SCache` | `src/xlw/cache/cached.h/.cpp` | Object persistence cache |
| `MG_XL_Cached::GetCaller()` | `src/xlw/cache/xlcache.h/.cpp` | Ported from `cpp_xloper` → `XlfServices` |

### `app/mgxll/cpp_iface/justatest.h`

Removed `#include <xlw/Wrapper.h>` (no longer exists in XLW 6.x; was not used).

### `app/mgxll/xl_iface/xl_justatest.cpp`

Changed `#include <xlw/cache/cached.h>` → `#include "xlw/cache/cached.h"` (MGlib
local file, not part of the XLW public API).

---

## Key API Differences: 4.x → 6.x

### `EXCEL_BEGIN` macro

| 4.0.0f0 | 6.0.0f0 |
|---------|---------|
| `XlfExcel::Instance().FreeMemory(); try {` | `try { UsesTempMemory whileInScopeUseTempMemory;` |

The semantics are equivalent: both ensure the per-call Excel memory arena is
reclaimed when the function returns.  The 6.x approach uses RAII.

### `XlfOper` internal storage

| 4.0.0f0 | 6.0.0f0 |
|---------|---------|
| Union of `LPXLOPER` / `LPXLOPER12`, selected at runtime | Always `LPXLOPER12` (64-bit Excel only) |

`LPXLFOPER` is `typedef LPXLOPER12 LPXLFOPER;` in 6.x — the generated
`xl_justatest.cpp` uses `LPXLFOPER` throughout and remains unchanged.

### `MG_XL_Cached::GetCaller()` / `GetSheetNm()`

| 4.0.0f0 (via `cpp_xloper`) | 6.0.0f0 (via `XlfServices`) |
|---------------------------|------------------------------|
| `cpp_xloper vRetCaller; vRetCaller.Excel(xlfCaller)` | `XlfServices.Information.GetCallingCell()` |
| `vRetCaller.GetTopRow()` | `caller.AsRef().GetRowBegin()` |
| `vRetSheetname.Excel(xlSheetNm, ...)` | `XlfServices.Information.GetSheetName(caller)` |

---

## Build

The XLL is still built via `msbuild app\mgxll\mgxll.vcxproj` — unchanged from
the previous session's CI fix.  The `src/xlw/xlw.vcxproj` lists all 6.x
sources and the two MGlib-specific additions (`XlfOperMG.cpp`,
`cache/xlcache.cpp`).

### Validation

```powershell
# After building on Windows:
python tests/validate_xll.py --xll bin\win\Release\mgxll.xll --arch win32 --verbose
python tests/validate_xll.py --xll bin\win\x64\Release\mgxll.xll --arch x64 --verbose
```

---

## Related Files

| Path | Purpose |
|------|---------|
| `src/xlw/` | XLW 6.0.0f0 headers + sources (vendored) |
| `src/xlw/xlw.vcxproj` | VS project for XLW static library |
| `src/xlw/EXCEL32_API.h` | Version: 6.0.0f0 |
| `src/xlw/XlfOperMG.cpp` | MGlib-specific `XlfOper` extensions (new file) |
| `src/xlw/cache/` | MGlib object-persistence cache (preserved, updated) |
| `app/mgxll/cpp_iface/justatest.h` | C++ function declarations |
| `app/mgxll/xl_iface/xl_justatest.cpp` | Auto-generated Excel C API wrappers |
| `tests/validate_xll.py` | Post-build XLL validator |
| `.github/workflows/build-windows.yml` | CI: build + validate Win32 & x64 XLLs |


This release dates from around 2009 and targets VS2008/2010 + Excel 2003/2007.
The bundled code already includes `XlfOper12.h` / `XlfOperImpl12.cpp` which are
XLOPER12 (64-bit Excel) stubs — indicating some forward-looking work had started
but was never completed.

---

## XLW Release History (brief)

| Version | Approx. year | Notable changes |
|---------|-------------|-----------------|
| 4.0.0   | 2009        | Baseline in repo |
| 5.0.0   | 2012–2013   | CMake build, Excel 2010 XLOPER12, C++11 |
| 6.0.0   | 2014+       | Namespace cleanup, header reorganisation |

The project moved from SourceForge to GitHub (https://github.com/xlw/xlw) and
is now mirrored at https://xlw.github.io/.

---

## Technical Assessment

### What would need to change

#### 1. Build system
- XLW 4.x ships `.vcxproj` project files and compiles as a static library.
- XLW 5.x+ adopts CMake as the primary build system.
- **Impact**: `src/xlw/xlw.vcxproj` would be replaced by a CMake-controlled
  subdirectory or the prebuilt library artefact.

#### 2. Header layout
XLW 5.x reorganised the public headers under `xlw/` with a flatter structure.
The include path `#include <xlw/CellMatrix.h>` etc. stays the same, but some
internal headers moved or were removed:

| Removed / renamed in 5.x | Replacement |
|--------------------------|-------------|
| `xlw/clw/Dispatcher.h`   | merged into `xlw/XlFunctionRegistration.h` |
| `xlw/cache/xlcache.h`    | removed; caching now lives in `xlw/XlfExcel.h` |
| `xlw/wrapper.h`          | `xlw/Wrapper.h` (capitalisation change) |

#### 3. Namespace and type changes
- XLW 5.x introduces `xlw::` namespace explicitly on more symbols; the
  `using namespace xlw;` workaround in `justatest.h` continues to work but
  some internal types gained namespace qualifiers.
- `XlfOper` in 5.x is a thin adapter that selects between `XlfOper4` (classic)
  and `XlfOper12` (XLOPER12) at runtime; the call signatures are compatible.

#### 4. Code-generation tool (xlwIfaceGen.exe)
`app/mgxll/xl_iface/xlwIfaceGen.exe` is the 32-bit code-generator shipped with
XLW 4.x.  XLW 5.x ships the same tool rebuilt from source as part of the CMake
build.  The `//<xlw:libraryname=…>` annotation syntax **is unchanged**, so the
existing `justatest.h` metadata is fully forward-compatible.

Key constraint: `xlwIfaceGen.exe` is a Win32 binary checked into the repo.
Upgrading XLW requires replacing this binary with the 5.x build.

#### 5. `xl_justatest.cpp` (auto-generated wrapper)
The pre-generated `app/mgxll/xl_iface/xl_justatest.cpp` was produced by the
4.x generator.  The 5.x generator produces structurally identical code, so no
manual changes are expected.  In CI the file is regenerated from `justatest.h`
during the pre-build step; the committed copy acts as a fallback.

#### 6. `EXCEL32_API.h` macros
- `EXCEL_BEGIN` / `EXCEL_END` macros are unchanged across 4.x and 5.x.
- `EXCEL_EXPORT` (maps to `__declspec(dllexport)`) is unchanged.
- `LPXLFOPER` typedef is unchanged.

#### 7. 64-bit Excel support (XLOPER12)
XLW 5.x fully implements the XLOPER12 path required by 64-bit Excel.  The
current `XlfOper12.h` stubs would be replaced by the real implementation,
enabling the XLL to load in 64-bit Excel (currently only 32-bit is fully
supported).

---

## Feasibility Verdict

**Technically feasible with moderate effort (~1–2 days of porting work).**

The main constraints are:

1. `xlwIfaceGen.exe` must be replaced — requires downloading or building XLW 5.x
   on Windows.
2. A small number of internal include paths need updating.
3. The generated `xl_justatest.cpp` should be re-run through the 5.x generator.
4. The CMake Linux build is unaffected (xlw is already excluded from CMake).

There are no breaking API changes in the code paths used by MGlib:
`CellMatrix`, `ArgList`, `XlfOper`, `XlfExcel`, `EXCEL_BEGIN`/`EXCEL_END`, and
the function-registration helpers are all stable across 4.x → 5.x.

---

## Migration Steps (if proceeding)

1. **Obtain XLW 5.x sources**
   ```
   git clone https://github.com/xlw/xlw --depth 1 --branch xlw-5.0.2
   ```

2. **Build `xlwIfaceGen.exe` from source** on a Windows box or CI agent:
   ```
   cmake -B build -A Win32 xlw/xlwIfaceGen
   cmake --build build --config Release
   # → build/Release/xlwIfaceGen.exe
   ```

3. **Copy new XLW headers and sources** into `src/xlw/`, replacing the 4.0.0f0
   vendored copy.

4. **Update `xlw.vcxproj`** (or switch to a CMake-based build for the xlw
   library).

5. **Run the new generator** to refresh `xl_justatest.cpp`:
   ```
   xlwIfaceGen.exe cpp_iface\justatest.h xl_iface\xl_justatest.cpp
   ```

6. **Build and validate**: run the Windows CI workflow
   (`.github/workflows/build-windows.yml`) and verify both XLLs pass
   `tests/validate_xll.py`.

7. **Test with 64-bit Excel** once the XLOPER12 path is active.

---

## Windows XLL Loading Check

The CI workflow (`.github/workflows/build-windows.yml`) performs the following
checks after every build:

| Check | Tool | What it verifies |
|-------|------|-----------------|
| File exists & non-empty | `validate_xll.py` | XLL was produced |
| PE structure valid | `LoadLibraryExW` with `LOAD_LIBRARY_AS_DATAFILE` | Not a corrupt binary |
| PE architecture | PE header `Machine` field | Win32 XLL is 0x014C; x64 XLL is 0x8664 |
| All 26 required exports | Custom PE export-table parser | Every `xl*` function is present |
| Live Excel smoke-test | `xlwings` (optional, `--live` flag) | XLL loads and `MG_JustATest` returns correctly |

Run locally (no Excel needed):
```powershell
python tests/validate_xll.py --xll bin\win\Release\mgxll.xll --arch win32
python tests/validate_xll.py --xll bin\win\x64\Release\mgxll.xll --arch x64 --verbose
```

---

## Related Files

| Path | Purpose |
|------|---------|
| `src/xlw/` | Vendored XLW 4.0.0f0 C++ sources |
| `src/xlw/xlw.vcxproj` | VS project for XLW static library |
| `src/xlw/EXCEL32_API.h` | Version defines |
| `app/mgxll/cpp_iface/justatest.h` | C++ function declarations with `//\<xlw:\>` metadata |
| `app/mgxll/cpp_iface/justatest.cpp` | Function implementations (calls MGlib) |
| `app/mgxll/xl_iface/xl_justatest.cpp` | Auto-generated Excel C API wrappers |
| `app/mgxll/xl_iface/xlwIfaceGen.exe` | Code-generator (Win32, XLW 4.0.0f0) |
| `app/mgxll/mgxll.vcxproj` | VS project for the XLL DLL |
| `lib/win/xlcall32.lib` | Excel C API stub library (Win32) |
| `lib/win/x64/xlcall32.def` | DEF file used to generate x64 stub |
| `tests/validate_xll.py` | Post-build XLL validator |
| `.github/workflows/build-windows.yml` | CI: build + validate Win32 & x64 XLLs |
