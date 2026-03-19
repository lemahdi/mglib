# MGlib — C++ Financial Pricing Library

MGlib is a C++ financial-math library that prices vanilla and exotic interest-rate products using a generic scripting pricer.
It was originally built for Windows/Excel using the [XLW](http://xlw.sourceforge.net/) framework; it has now been ported to be fully cross-platform and buildable with CMake.

---

## Features

- **Zero curve construction** with linear interpolation of zero rates
- **Black–Scholes model** for IR options: Caplet, Floorlet, DigitalUp/Down, Cap, Floor, Swaption
- **Hull–White 1-Factor model** (Vasicek / HW1F)
- **Generic scripting pricer** — describe cash-flows in a simple expression language (lexed/parsed with Flex + Bison)
- **Monte Carlo engine** — plain, MISER, VEGAS methods backed by GSL
- **Quasi-random** sequences (Sobol) for variance reduction
- **Business calendar** support (USD, EUR, GBP, JPY, CHF, CAD, AUD, SEK, NOK, DKK)
- **Schedule generation** for swaps, caps, floors, swaptions

---

## Build instructions

### Prerequisites

| Tool | Minimum version |
|------|----------------|
| C++ compiler | GCC 9 / Clang 10 / MSVC 2019 |
| CMake | 3.10 |
| GNU Scientific Library (GSL) | 2.8 |
| Flex | 2.6 |
| Bison | 3.0 |

#### Ubuntu / Debian

```bash
sudo apt-get install -y cmake ninja-build g++ libgsl-dev flex bison
```

#### macOS (Homebrew)

```bash
brew install cmake gsl flex bison
```

#### Windows (vcpkg)

```powershell
vcpkg install gsl:x64-windows
# Then pass -DCMAKE_TOOLCHAIN_FILE=<vcpkg root>/scripts/buildsystems/vcpkg.cmake to cmake
```

### Clone & build

```bash
git clone --recurse-submodules https://github.com/lemahdi/mglib.git
cd mglib
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

The build produces:

| Artifact | Description |
|----------|-------------|
| `build/libmglib.a` | Static library containing all pricing and infrastructure code |
| `build/mgapp` | Interactive test / demonstration application |

### Running the demo

```bash
./build/mgapp
```

The demo application walks through:
1. A generic-scripted option priced with Monte Carlo and compared to Black–Scholes
2. Expression-language parsing of exotic cash-flow payoffs
3. Calendar business-day checks
4. Random / quasi-random number generation benchmarks

---

## Project structure

```
mglib/
├── CMakeLists.txt          ← Cross-platform build (new)
├── makefile.mk             ← Legacy GNU Make build (Cygwin/Linux)
├── app/
│   └── mgapp/             ← Demo application (init.cpp, run.cpp)
└── src/
    ├── genpricer/          ← Generic scripting pricer
    │   ├── genmod/         ← Pricing models (Black–Scholes, HW1F, Vasicek)
    │   ├── genparam/       ← Model parameter objects
    │   ├── genpricer/      ← Pricer orchestrator
    │   ├── gensec/         ← Generic security description
    │   ├── infra/          ← Flex/Bison parser + AST nodes
    │   └── numerics/       ← Monte Carlo numerical methods
    ├── gsl/                ← GSL source (git submodule)
    ├── mktdata/            ← Market data (zero curves, vol surfaces, dividends)
    ├── nova/               ← Core infrastructure
    │   ├── glob/           ← Date, exception, argument converters
    │   ├── market/         ← Calendar, IR index, schedule, term structure
    │   ├── numerical/      ← Interpolator, distributions, random, solver, MC
    │   ├── patterns/       ← CountedPtr smart pointer, Singleton
    │   ├── utils/          ← String / date utilities
    │   └── wrapper/        ← GSL vector/matrix wrappers
    ├── vanilla/            ← Vanilla products
    │   ├── model/          ← Vanilla pricing models
    │   └── security/       ← CallPut, Digital, FRA, Swap, Swaption, etc.
    └── xlw/                ← Windows-only Excel XLL interface (not built on Linux/macOS)
```

---

## Design overview

### Pricing models

All models inherit from `MG_IRPricingModel` which exposes:

```
Libor(...)   Caplet(...)  Floorlet(...)
DigitalUp(…) DigitalDown(…)
Cap(…)       Floor(…)     Swaption(…)
```

### Generic pricer expression language

Cash-flow payoffs are described in a small expression language, e.g.:

```
EXP(-0.05*(EndDate[i]-StartDate[i])) * MAX(Spot[i]-Strike[i], 0)
```

Supported functions: `MAX`, `MIN`, `ABS`, `EXP`, `LOG`, `POW`, `IF`, `LIBOR`

### Excel interface (Windows only)

The `src/xlw/` and `app/mgxll/` directories contain the Excel XLL integration built with the XLW library.
This layer is excluded from the CMake build on Linux/macOS.

---

## GSL dependency

MGlib depends on the [GNU Scientific Library (GSL)](https://www.gnu.org/software/gsl/).

- **Minimum required version**: GSL 2.8
- **Ubuntu 24.04 note**: The `libgsl-dev` apt package provides 2.7.1. Build GSL 2.8 from source (see CI workflow) or use a newer distro.
- **Homebrew**: `brew install gsl` provides 2.8+.
- **Windows (vcpkg)**: `vcpkg install gsl:x64-windows` — verify the provided version is ≥ 2.8.
- **Why 2.8?** Includes the `gsl_spline2d` bicubic 2-D interpolation now used by `MG_2DInterpolator`, plus fixes to `gsl_cdf_gaussian_Pinv` tail accuracy, the Brent root finder, VEGAS Monte Carlo, and the Sobol quasi-random dimension limit.
- **Alternative library evaluation**: see [`docs/benchmark-gsl-alternatives.md`](docs/benchmark-gsl-alternatives.md)
- **Migration plan**: see [`docs/gsl-migration-plan.md`](docs/gsl-migration-plan.md)

---

## License

Copyright © 2011 MM Akkouh / MG. All rights reserved.
