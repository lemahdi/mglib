# Migrating MGlib from Visual Studio to VS Code

This document summarizes the trade-offs of moving the MGlib development
workflow from Visual Studio (Windows IDE) to Visual Studio Code (cross-platform
editor) and provides a step-by-step setup guide.

---

## Pros

| # | Benefit | Detail |
|---|---------|--------|
| 1 | **Cross-platform** | VS Code runs identically on Windows, Linux, and macOS. The existing CMake build already targets Linux in CI; developers on any OS can now use the same editor workflow. |
| 2 | **Free and open-source** | No licence required. VS Code Community is fully featured, unlike the Visual Studio Community edition which has commercial-use restrictions. |
| 3 | **CMake-native workflow** | The *CMake Tools* extension integrates configure/build/test directly in the status bar, matching the CMake-first approach already used in CI (`ci.yml`). |
| 4 | **Accurate IntelliSense via `compile_commands.json`** | CMake generates `compile_commands.json` (`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`). The C/C++ extension uses it for exact include paths and flags — no manual `.vcxproj` maintenance needed. |
| 5 | **Lightweight and fast** | VS Code starts in seconds; Visual Studio takes considerably longer. This matters during rapid edit–compile–test cycles. |
| 6 | **Rich extension ecosystem** | Extensions cover Flex/Bison syntax highlighting, spell-checking, Git history, remote development (SSH, containers, WSL), and more. |
| 7 | **Integrated terminal** | A built-in terminal lets developers run CMake, Ninja, or GSL build scripts without switching windows. |
| 8 | **Remote development** | The *Remote – SSH* and *Dev Containers* extensions allow editing and debugging code running on a Linux server or in a Docker container — directly relevant for the CI environment. |
| 9 | **Git integration** | Source Control view provides diff, staging, branching, and PR review without leaving the editor. |

---

## Cons

| # | Drawback | Detail |
|---|----------|--------|
| 1 | **No native `.vcxproj` / `.sln` support** | The Windows XLL target (`app/mgxll/`) relies on `mgxll.vcxproj` and the bundled `src/gsl/VC/` projects. These must still be built with MSBuild on Windows; VS Code cannot replace Visual Studio for the XLL artefact. |
| 2 | **Debugger setup is manual** | Visual Studio has a fully integrated debugger. VS Code requires `launch.json` configuration and a separate GDB/LLDB installation; the experience is good but not as seamless on Windows. |
| 3 | **No built-in profiler** | Visual Studio ships a CPU/memory profiler. VS Code relies on external tools (Valgrind, perf, Instruments) invoked from the terminal. |
| 4 | **IntelliSense cold-start latency** | On first open, the C/C++ extension indexes the entire workspace. With ~700 source files this can take a minute; Visual Studio's IntelliSense is typically faster after the initial solution load. |
| 5 | **Extension fragmentation** | Each developer must install and configure extensions individually (mitigated by `extensions.json` in this PR). There is no enforced extension version, which can lead to subtle behavioural differences across machines. |
| 6 | **Refactoring tools are limited** | Visual Studio has deep rename/refactor support for C++. VS Code's C++ refactoring (via `clangd` or the Microsoft extension) is improving but not yet equivalent. |

---

## Recommendation

For **Linux and macOS** development, VS Code with CMake Tools is the recommended
workflow — it matches the CI environment exactly.  
For **Windows** development, Visual Studio remains necessary to build the XLL
add-in; VS Code can still be used for editing and Linux-compatible targets via WSL.

---

## Setup Guide

### Prerequisites

```bash
# Linux (Ubuntu 24.04)
sudo apt-get install cmake ninja-build g++ flex bison

# GSL 2.8 (Ubuntu ships 2.7.1 — build from source; see docs/gsl-migration-plan.md)
cd /tmp && wget https://ftp.gnu.org/gnu/gsl/gsl-2.8.tar.gz
tar xf gsl-2.8.tar.gz && cd gsl-2.8
./configure --prefix="$HOME/.local" && make -j$(nproc) && make install
```

### 1. Open the workspace

```bash
code /path/to/mglib
```

VS Code will prompt you to install the recommended extensions defined in
`.vscode/extensions.json`. Click **Install All**.

### 2. Configure the project

Open the Command Palette (`Ctrl+Shift+P`) and run:

```
CMake: Configure
```

This runs:

```bash
cmake -B build -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

If GSL 2.8 was installed to `$HOME/.local`, add it to CMake Tools settings:

```json
// .vscode/settings.json  (user override — do not commit personal paths)
"cmake.configureArgs": [
  "-DCMAKE_BUILD_TYPE=Release",
  "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
  "-DCMAKE_PREFIX_PATH=/home/<you>/.local"
]
```

### 3. Build

Press **Ctrl+Shift+B** (default build task) or run:

```
CMake: Build
```

### 4. Run the smoke test

Open the Terminal (`Ctrl+\``) and run:

```bash
echo "" | timeout 10 ./build/mgapp || true
```

Or use the *Run: mgapp (smoke test)* task from **Terminal → Run Task**.

### 5. Debug

1. Set a breakpoint in `app/mgapp/run.cpp`.
2. Select the **Debug mgapp** configuration in the Run & Debug panel (`Ctrl+Shift+D`).
3. Press **F5**.

> **Note:** ensure GDB is installed (`sudo apt-get install gdb`) and that a Debug
> build is configured (`CMake: Configure (Debug)` task).

---

## Files added by this migration

| File | Purpose |
|------|---------|
| `.vscode/extensions.json` | Recommended extensions (C++, CMake Tools, Flex/Bison) |
| `.vscode/settings.json` | IntelliSense, CMake Tools, and editor defaults |
| `.vscode/tasks.json` | Configure, build, smoke-test, and clean tasks |
| `.vscode/launch.json` | GDB debug launch configuration for `mgapp` |
