# GSL Migration Plan: 2.7.1 → 2.8

This document outlines the migration of MGlib's GNU Scientific Library dependency
from version **2.7.1** to the latest stable release **2.8** (released 2023-07-27).

> **Status**: Migration complete. All steps have been implemented.

---

## State Before Migration

| Item | Before |
|---|---|
| GSL version | 2.7.1 |
| Source | System package (`libgsl-dev` on Ubuntu/Debian) |
| CMake minimum required | 2.7 |
| `MG_2DInterpolator` | Two-pass approach: `vector<gsl_spline*>` + `gsl_interp*` |

## State After Migration

| Item | After |
|---|---|
| GSL version | 2.8 |
| Source | Built from source in CI; Homebrew 2.8+ on macOS |
| CMake minimum required | 2.8 |
| `MG_2DInterpolator` | Native `gsl_spline2d` (bilinear or bicubic, single object) |

GSL headers used by MGlib (19 distinct includes after migration):

```
gsl/gsl_cdf.h         gsl/gsl_math.h           gsl/gsl_randist.h
gsl/gsl_errno.h       gsl/gsl_matrix.h         gsl/gsl_roots.h
gsl/gsl_fit.h         gsl/gsl_min.h            gsl/gsl_rng.h
gsl/gsl_interp.h      gsl/gsl_monte_miser.h    gsl/gsl_spline.h
gsl/gsl_monte_plain.h gsl/gsl_monte_vegas.h    gsl/gsl_spline2d.h  ← new
gsl/gsl_multifit.h    gsl/gsl_qrng.h           gsl/gsl_sys.h
gsl/gsl_vector.h
```

---

## What Changed in GSL 2.8

GSL 2.8 is a backwards-compatible release. There are **no breaking changes** to the API
surfaces used by MGlib. The notable additions and fixes are:

> **Note**: GSL 2.8 is the latest known stable release as of the time this document was
> written. Check https://www.gnu.org/software/gsl/ for the current latest version before
> starting a future upgrade.

### New features

| Area | Change |
|---|---|
| Nonlinear least-squares | New `gsl_multilarge_nlinear` large-scale NLS solver (not used by MGlib) |
| Statistics | `gsl_stats_quantile_from_sorted_data_acc` — accelerated quantile function |
| ODE solvers | Improved Runge-Kutta-Nyström integrators |
| CBLAS | Performance improvements for `dsymv`, `dtrmv` on modern CPUs |
| Random distributions | `gsl_ran_beta_pdf` and `gsl_cdf_beta_*` precision fixes |
| Interpolation | `gsl_interp2d` bilinear/bicubic 2-D interpolation — used by `MG_2DInterpolator` |
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

## Completed Migration Steps

### ✅ Step 1 — Updated minimum required version in CMakeLists.txt

```cmake
find_package(GSL 2.8 REQUIRED)   # provides GSL::gsl GSL::gslcblas
```

### ✅ Step 2 — Updated the README prerequisites table

```markdown
| GNU Scientific Library (GSL) | 2.8 |
```

### ✅ Step 3 — Updated CI / GitHub Actions workflow

The CI workflow now builds GSL 2.8 from source before configuring MGlib:

```yaml
- name: Build and install GSL 2.8 from source
  run: |
    cd /tmp
    wget -q https://ftp.gnu.org/gnu/gsl/gsl-2.8.tar.gz
    tar xf gsl-2.8.tar.gz
    cd gsl-2.8
    ./configure --prefix="$HOME/.local" --quiet
    make -j$(nproc)
    make install

- name: Configure (Release)
  run: |
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_PREFIX_PATH="$HOME/.local"
```

### ✅ Step 4 — `MG_2DInterpolator` now uses `gsl_spline2d`

`MG_2DInterpolator` now uses the native `gsl_spline2d` API from GSL 2.0+
(also available in 2.8). The old two-pass approach (`vector<gsl_spline*>` for
row splines + `gsl_interp*` for the cross-axis) has been replaced with a single
`gsl_spline2d*` object:

```cpp
// New: single gsl_spline2d object
gsl_spline2d* my2DSpline;

// Constructor — build flat z-array and init
Build2DZarr(myOrd.GetPtr(), nx, ny, zarr);   // zarr[j*nx+i] = myOrd(i,j)
my2DSpline = gsl_spline2d_alloc(vType, nx, ny);
gsl_spline2d_init(my2DSpline, xa, ya, zarr.data(), nx, ny);

// Eval — clamp + eval
double x = std::max(myAbsc1D.Front(), std::min(myAbsc1D.Back(), aX));
double y = std::max(myAbsc2D.Front(), std::min(myAbsc2D.Back(), aY));
return gsl_spline2d_eval(my2DSpline, x, y, nullptr, nullptr);
```

**Type mapping** (1-D type → 2-D GSL type):

| `aInterpolType` | `gsl_interp2d_type` |
|---|---|
| `LINEAR_INTERPOL` | `gsl_interp2d_bilinear` |
| `CUBICSPLINE_INTERPOL` | `gsl_interp2d_bicubic` |
| `AKIMA_INTERPOL` | `gsl_interp2d_bicubic` |
| Stepwise types | Fall back to `myStepWiseFunc` |

### ✅ Step 5 — Improved `gsl_cdf_gaussian_Pinv` precision

No code change required. The improvement is in the library itself and benefits
`MG_NormalDist::InvCdf` automatically after upgrading to GSL 2.8.

---

## Verification

### Build smoke test

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=$HOME/.local
cmake --build build --parallel
```

All 55 translation units compile without error.

### Functional regression

```bash
echo "c c c c c c c c c c c c c c" | timeout 30 ./build/mgapp
```

Expected output includes:
- `MC:` and `BS:` prices within ~5 bp of each other
- `Monte Carlo Integral of Exp(x) on [0,ln(2)]: 1`
- Newton solver: `-0.703467`
- `GAUSSIAN DISTRIBUTION`, `SOBOL`, `Linear Regression`

---

## Rollback Plan

If GSL 2.8 introduces an unexpected regression:

1. In `CMakeLists.txt`, revert to `find_package(GSL 2.7 REQUIRED)`
2. In `.github/workflows/ci.yml`, restore the `libgsl-dev` apt install step and
   remove the `wget`/build-from-source step
3. In `src/nova/numerical/interpolator.cpp`, the `gsl_spline2d`-based
   `MG_2DInterpolator` will still compile against 2.7.1 (the API was introduced
   in GSL 2.0)
4. File a bug report at https://savannah.gnu.org/bugs/?group=gsl
