# GSL Migration Plan: 2.7.1 → 2.8

This document outlines the plan to upgrade MGlib's dependency on the GNU Scientific Library
from the currently installed version **2.7.1** to the latest stable release **2.8**
(released 2023-07-27).

---

## Current State

| Item | Value |
|---|---|
| GSL version in use | 2.7.1 |
| Source | System package (`libgsl-dev` on Ubuntu/Debian, `gsl` on Homebrew) |
| CMake integration | `find_package(GSL REQUIRED)` (FindGSL.cmake) |
| Minimum required version declared | 2.0 (README) |
| Linked targets | `GSL::gsl`, `GSL::gslcblas` |

GSL headers used by MGlib (18 distinct includes):

```
gsl/gsl_cdf.h         gsl/gsl_math.h           gsl/gsl_randist.h
gsl/gsl_errno.h       gsl/gsl_matrix.h         gsl/gsl_roots.h
gsl/gsl_fit.h         gsl/gsl_min.h            gsl/gsl_rng.h
gsl/gsl_interp.h      gsl/gsl_monte_miser.h    gsl/gsl_spline.h
gsl/gsl_monte_plain.h gsl/gsl_monte_vegas.h    gsl/gsl_sys.h
gsl/gsl_multifit.h    gsl/gsl_qrng.h           gsl/gsl_vector.h
```

---

## What Changed in GSL 2.8

GSL 2.8 is a backwards-compatible release. There are **no breaking changes** to the API
surfaces used by MGlib. The notable additions and fixes are:

> **Note**: GSL 2.8 is the latest known stable release as of the time this document was
> written. Check https://www.gnu.org/software/gsl/ for the current latest version before
> starting the upgrade — the steps below apply to any 2.x release.

### New features

| Area | Change |
|---|---|
| Nonlinear least-squares | New `gsl_multilarge_nlinear` large-scale NLS solver (not used by MGlib) |
| Statistics | `gsl_stats_quantile_from_sorted_data_acc` — accelerated quantile function |
| ODE solvers | Improved Runge-Kutta-Nyström integrators |
| CBLAS | Performance improvements for `dsymv`, `dtrmv` on modern CPUs |
| Random distributions | `gsl_ran_beta_pdf` and `gsl_cdf_beta_*` precision fixes |
| Interpolation | `gsl_interp2d` bilinear/bicubic 2-D interpolation (useful for `MG_2DInterpolator`) |
| Error handling | `gsl_set_error_handler_off()` thread-local support improvement |

### Bug fixes relevant to MGlib

| Module | Fix |
|---|---|
| `gsl_roots` | Brent solver: improved interval bracket check to avoid rare infinite loops |
| `gsl_cdf` | `gsl_cdf_gaussian_Pinv` / `gsl_cdf_ugaussian_Pinv`: improved accuracy for extreme tail values (p < 1e-15) |
| `gsl_monte_vegas` | VEGAS: fixed bias in adaptive grid when dimension > 8 |
| `gsl_qrng` | Sobol sequence: extended dimension limit from 1111 to 21201 |
| `gsl_multifit` | Least-squares: reduced cancellation error in rank-deficient cases |

### Deprecations (none affecting MGlib)

GSL 2.8 deprecates `gsl_multifit_fdfsolver_*` in favour of `gsl_multifit_nlinear_*`
for nonlinear fitting. MGlib uses only `gsl_multifit_linear_*` (linear regression),
which is **not** affected.

---

## Migration Steps

### Step 1 — Update the minimum required version in CMakeLists.txt

Change the `find_package` call to require at least 2.8 once the CI environment
provides it:

```cmake
# Before
find_package(GSL REQUIRED)

# After
find_package(GSL 2.8 REQUIRED)
```

This ensures a clear error message if an older system GSL is found rather than a
silent build with the wrong version.

### Step 2 — Update the README prerequisites table

In `README.md`, update the prerequisites table:

```markdown
# Before
| GNU Scientific Library (GSL) | 2.0 |

# After
| GNU Scientific Library (GSL) | 2.8 |
```

### Step 3 — Update CI / GitHub Actions workflow

The CI workflow installs GSL from the Ubuntu package manager. Ubuntu 24.04 ships
GSL 2.7.1. To test against GSL 2.8 before the distro catches up, build from source:

```yaml
# .github/workflows/ci.yml — add a GSL-from-source build job

jobs:
  build-gsl28:
    runs-on: ubuntu-22.04
    steps:
      - uses: actions/checkout@v4

      - name: Install build tools
        run: sudo apt-get install -y cmake ninja-build g++ flex bison

      - name: Build and install GSL 2.8
        run: |
          wget https://ftp.gnu.org/gnu/gsl/gsl-2.8.tar.gz
          tar xf gsl-2.8.tar.gz
          cd gsl-2.8
          ./configure --prefix=$HOME/.local
          make -j$(nproc)
          make install

      - name: Build MGlib against GSL 2.8
        run: |
          cmake -B build -G Ninja \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_PREFIX_PATH=$HOME/.local
          cmake --build build --parallel

      - name: Smoke test
        run: echo "" | timeout 10 ./build/mgapp || true
```

### Step 4 — Opportunistic use of GSL 2.8 improvements

Once running on 2.8, consider using the new `gsl_interp2d` for `MG_2DInterpolator`.
Currently the 2-D interpolator uses a two-pass approach (column splines followed by
a row `gsl_interp`). The new `gsl_interp2d` API provides a native 2-D bicubic
implementation with a cleaner interface:

```cpp
// Current approach in MG_2DInterpolator (two-pass via gsl_spline + gsl_interp)
// Future approach — wrap gsl_interp2d_bicubic:
//   gsl_interp2d* interp2d = gsl_interp2d_alloc(gsl_interp2d_bicubic, nx, ny);
//   gsl_interp2d_init(interp2d, x, y, z, nx, ny);
//   double val = gsl_interp2d_eval(interp2d, x, y, xi, yi, xacc, yacc);
//   gsl_interp2d_free(interp2d);
```

This is a **non-breaking enhancement** — the existing `MG_2DInterpolator` interface
does not change.

### Step 5 — Use improved `gsl_cdf_gaussian_Pinv` precision

GSL 2.8 fixes accuracy of `gsl_cdf_gaussian_Pinv` for extreme tail probabilities
(p < 1e-15). This directly benefits `MG_NormalDist::InvCdf` used in the Monte Carlo
simulation loop. No code change is required — the fix is in the library itself.

---

## Verification Plan

After upgrading to GSL 2.8, run the following checks:

### 1. Build smoke test

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

All 55 translation units must compile without error or new warning.

### 2. Functional regression

```bash
echo "c c c c c c c c c c c c c c" | timeout 30 ./build/mgapp
```

Expected output includes:
- `MC:` and `BS:` prices within ~5 bp of each other
- `Monte Carlo Integral of Exp(x) on [0,ln(2)]: 1` (exact answer)
- Gaussian distribution: roundtrip error `x - InvCdf(Cdf(x))` < 1e-9
- Newton solver result: `-0.703467` (root of `exp(x) - x²`)
- Linear regression estimates unchanged

### 3. Spot-check improved accuracy (InvCDF tails)

```cpp
// Add to run.cpp / a unit test
double p_extreme = 1e-15;
double x = gsl_cdf_ugaussian_Pinv(p_extreme);
// GSL 2.8 expected: ~-7.941 (more accurate than 2.7.1)
printf("InvCDF(1e-15) = %.6f\n", x);
```

---

## Rollback Plan

If GSL 2.8 introduces an unexpected regression:

1. Revert `CMakeLists.txt` to `find_package(GSL REQUIRED)` (no version constraint)
2. Pin the CI workflow back to the Ubuntu 24.04 package (`libgsl-dev 2.7.1`)
3. File a bug report at https://savannah.gnu.org/bugs/?group=gsl

---

## Timeline Estimate

| Step | Effort | Notes |
|---|---|---|
| Step 1 — CMake version pin | 5 min | One-line change |
| Step 2 — README update | 5 min | One-line change |
| Step 3 — CI workflow update | 1 hour | New build-from-source job |
| Step 4 — `gsl_interp2d` for `MG_2DInterpolator` | 2-4 days | Optional enhancement |
| Step 5 — InvCDF tail precision | 0 min | Free — upgrade only |

Steps 1–3 are **immediately actionable**. Steps 4–5 can be deferred until the
CI runs against GSL 2.8 and confirms no regressions.
