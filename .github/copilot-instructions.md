# GitHub Copilot Instructions for MGlib

MGlib is a C++17 financial-math library that prices vanilla and exotic interest-rate
products via a generic scripting pricer. It uses CMake, GSL 2.8, Flex, and Bison.

---

## Language and standard

- **C++17** throughout (`std::optional`, structured bindings, `if constexpr`, etc.)
- All source lives under `src/` with sub-namespaces mirroring the directory layout.
- `#pragma once` is the include guard convention (no `#ifndef` guards).

---

## Namespace and naming conventions

| Element | Convention | Example |
|---|---|---|
| Namespace | `MG_NAMESPACE_BEGIN` / `MG_NAMESPACE_END` macros (expands to `namespace MG { }`) | `MG_NAMESPACE_BEGIN` |
| Classes | `MG_` prefix + PascalCase | `MG_IRPricingModel`, `MG_ZeroCurve` |
| Smart-pointer aliases | `MG_<Class>Ptr` via `using` in `typedef.h` | `MG_ZeroCurvePtr` |
| Constants | `K_` prefix + UPPER_CASE | `K_PI`, `K_EPS` |
| Member variables | `my` prefix + PascalCase | `myStartDate`, `myNbSteps` |
| Enum values | PascalCase | `LINEAR`, `CUBICSPLINE` |
| File names | lowercase with underscores for generated Flex/Bison output | `lex.yy.cpp`, `my_parser.tab.cpp` |

---

## Smart pointers

All heap-owned objects use `std::shared_ptr`.  Aliases defined in
`src/nova/glob/typedef.h` keep call sites clean:

```cpp
// typedef.h  (abbreviated)
using MG_ZeroCurvePtr = std::shared_ptr<MG_ZeroCurve>;
```

Prefer passing `const MG_FooPtr&` rather than raw `MG_Foo*` for ownership-sharing
parameters.  Use raw pointers only for non-owning, short-lived observers.

---

## Singleton pattern

Use `Singleton<T>::Instance()` (Meyers' singleton, thread-safe under C++11):

```cpp
MG_RandomGenerator* rng = Singleton<MG_RandomGenerator>::Instance();
```

`Instance()` returns `T*`.  Never delete the returned pointer.

---

## Copy/move semantics

Classes that manage resources implement the **copy-and-swap** idiom:

```cpp
void Swap(MG_Foo& aFrom);
ASSIGN_OPERATOR(MG_Foo)   // macro from typedef.h
```

Prefer `= default` for rule-of-zero classes.

---

## Error handling

Throw `MG_Exception` (defined in `src/nova/glob/exception.h`); never use raw
`std::runtime_error` or `std::exception` directly:

```cpp
throw MG_Exception("MG_Foo::Bar", "invalid argument: n must be > 0");
```

---

## Build system

```bash
# Configure + build (Release)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

# Run the demo / smoke test
echo "" | timeout 10 ./build/mgapp || true
```

Key CMake facts:
- Requires **GSL ≥ 2.8** (`find_package(GSL 2.8 REQUIRED)`).
- Ubuntu 24.04 ships GSL 2.7.1 — see `docs/gsl-migration-plan.md` for how to build 2.8 from source.
- Pre-generated Flex/Bison outputs (`lex.yy.cpp`, `my_parser.tab.cpp`) are committed; regeneration only happens when those files are absent.
- The Windows XLL target (`src/xlw/`, `app/mgxll/`) is excluded from CMake on Linux/macOS.

---

## Project layout

```
src/
  nova/           – Core infrastructure (date, calendar, distributions, interpolation, MC)
  mktdata/        – Market data objects (zero curve, vol surface)
  vanilla/        – Vanilla products + models (Black–Scholes, HW1F, Vasicek)
  genpricer/
    genmod/       – Generic pricing models
    genparam/     – Model parameter objects
    genpricer/    – Pricer orchestrator
    gensec/       – Generic security description
    infra/        – Flex/Bison lexer + parser + AST nodes
    numerics/     – Monte Carlo (plain, MISER, VEGAS), quasi-random (Sobol)
app/
  mgapp/          – Demo / integration test application (init.cpp, run.cpp)
tests/
  validate_xll.py – Windows XLL validation helper
docs/
  benchmark-gsl-alternatives.md
  gsl-migration-plan.md
```

---

## Expression language (generic scripting pricer)

Cash-flow payoffs are described in a small domain-specific language parsed by
`src/genpricer/infra/my_parser.ypp` (Bison) and `src/genpricer/infra/my_scanner.l`
(Flex).  Supported built-ins:

```
MAX  MIN  ABS  EXP  LOG  POW  IF  LIBOR
```

Example payoff:

```
EXP(-0.05*(EndDate[i]-StartDate[i])) * MAX(Spot[i]-Strike[i], 0)
```

When modifying the grammar, regenerate with:

```bash
bison -d src/genpricer/infra/my_parser.ypp -o src/genpricer/infra/my_parser.tab.cpp
flex  -o src/genpricer/infra/lex.yy.cpp src/genpricer/infra/my_scanner.l
```

---

## Key numeric types and utilities

| Symbol | Location | Notes |
|---|---|---|
| `MG_2DInterpolator` | `nova/numerical/interpolator.h` | GSL `gsl_spline2d`; bilinear or bicubic |
| `MG_RandomGenerator` | `nova/numerical/random.h` | Wraps GSL RNG; singleton |
| `MG_MonteCarlo` | `nova/numerical/montecarlo.h` | Plain / MISER / VEGAS |
| `MG_Date` | `nova/glob/date.h` | Modified Julian Day integer arithmetic |
| `MG_Calendar` | `nova/market/` | Business-day conventions for USD, EUR, GBP, JPY, CHF, CAD, AUD, SEK, NOK, DKK |

---

## CI / workflows

| Workflow | Trigger | What it does |
|---|---|---|
| `ci.yml` | push/PR to master/main/copilot/** | Builds GSL 2.8 from source, configures with Ninja, builds, smoke-tests |
| `build-windows.yml` | push/PR to master/main/copilot/** | MSBuild Win32 + x64 XLL, uploads artifacts |
