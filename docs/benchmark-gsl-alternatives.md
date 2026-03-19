# Benchmark: GSL vs Alternative Scientific Libraries

This document compares **GNU Scientific Library (GSL)** — currently used by MGlib — against
other mature C++ scientific/numerical libraries, evaluated across the functional areas that
MGlib actually relies on.

---

## Functional Areas Used by MGlib

| Area | GSL module(s) used |
|---|---|
| Random number generation (40+ generators) | `gsl_rng`, `gsl_qrng` |
| Probability distributions (Normal, Uniform, Lognormal) | `gsl_randist`, `gsl_cdf` |
| Monte Carlo integration (Plain, MISER, VEGAS) | `gsl_monte_plain/miser/vegas` |
| Root finding (Bisection, Brent, Newton, Secant, Steffenson) | `gsl_roots` |
| 1-D & 2-D Interpolation / Splines | `gsl_interp`, `gsl_spline` |
| Function minimization (Golden-section, Brent, Quad-golden) | `gsl_min` |
| Linear & multivariate regression | `gsl_fit`, `gsl_multifit` |
| Dense vector and matrix arithmetic | `gsl_vector`, `gsl_matrix` |

---

## Candidate Libraries

### 1. GSL (current — 2.8)

- **License**: GNU GPL v3 (copyleft)  
- **Language**: C API, header-only C++ wrappers exist (`gsl_vector`, etc.)
- **Version in use**: 2.8 (built from source in CI); check https://www.gnu.org/software/gsl/ for the current latest version
- **Coverage**: All areas above in a single library
- **Maturity**: ~25 years; widely used in scientific computing
- **Downsides**: C API requires manual memory management (`gsl_rng_alloc`/`gsl_rng_free`);
  copyleft license may be a concern for proprietary extensions; header API is verbose

### 2. Boost.Math + Boost.Random

- **License**: Boost Software License (permissive)  
- **Language**: Header-only C++
- **Coverage**:

| MGlib area | Boost equivalent |
|---|---|
| Probability distributions (PDF, CDF, InvCDF) | `boost::math::normal`, `boost::math::uniform`, `boost::math::lognormal` |
| Random number generation | `boost::random::mt19937`, `boost::random::sobol` (Quasi-random) |
| Root finding | `boost::math::tools::bisect`, `boost::math::tools::brent_find_minima` |
| Interpolation | `boost::math::interpolators::cardinal_cubic_b_spline`, `pchip` |
| Statistics / regression | `boost::math::statistics` (partial) |
| Monte Carlo integration | ❌ No direct equivalent |
| Dense vectors / matrices | ❌ Not covered by Boost.Math |

- **Assessment**: Strong alternative for distributions, root-finding, and interpolation.
  Does **not** cover Monte Carlo integration or dense linear algebra.
  No single-library replacement; would require mixing Boost + Eigen.
- **Performance**: Comparable to GSL for special functions; Boost's quasi-random Sobol
  is competitive with `gsl_qrng`.

### 3. Eigen 3

- **License**: MPL-2.0 (permissive)  
- **Language**: Header-only C++ templates  
- **Coverage**:

| MGlib area | Eigen equivalent |
|---|---|
| Dense vectors / matrices | `Eigen::VectorXd`, `Eigen::MatrixXd` — **superior** API to GSL |
| Linear regression | `Eigen::LeastSquaresConjugateGradient`, `bdcSvd().solve()` |
| Root finding | ❌ Not covered |
| Probability distributions | ❌ Not covered |
| Monte Carlo integration | ❌ Not covered |
| Interpolation | ❌ Not covered (community `EigenSpline` exists separately) |

- **Assessment**: Eigen is the gold standard for dense linear algebra in C++.
  It would be a very strong replacement for `MG_Vector`/`MG_Matrix` (which currently
  wrap `gsl_vector`/`gsl_matrix`). However, it covers only ~2 of the 8 functional areas.
- **Performance**: Generally **10-30% faster** than GSL for dense BLAS operations due
  to expression templates and SIMD auto-vectorisation.

### 4. Armadillo

- **License**: Apache-2.0 (permissive)  
- **Language**: C++ template library  
- **Coverage**:

| MGlib area | Armadillo equivalent |
|---|---|
| Dense vectors / matrices | `arma::vec`, `arma::mat` — expressive MATLAB-like API |
| Linear regression | `arma::solve()`, `arma::pinv()` |
| Random number generation | `arma::randu()`, `arma::randn()` (wraps system RNG) |
| Probability distributions | `arma::normpdf()`, `arma::normcdf()` |
| Root finding | ❌ Not covered |
| Monte Carlo integration | ❌ Not covered |
| Interpolation | ❌ Not covered |

- **Assessment**: Similar footprint to Eigen; excellent for the matrix/vector layer and
  has some built-in normal distribution support. Depends on LAPACK/BLAS at runtime.
  Does not cover Monte Carlo integration, interpolation, or solvers.

### 5. Intel oneAPI Math Kernel Library (oneMKL / MKL)

- **License**: Intel Simplified License (free for use, proprietary, x86 only)  
- **Coverage**:

| MGlib area | MKL equivalent |
|---|---|
| Random number generation | VSL (Vector Statistics Library) — 60+ generators, vectorised |
| Probability distributions | VSL random distributions (Normal, Uniform, Lognormal, ...) |
| Monte Carlo integration | ❌ Not as a standalone component |
| Root finding / minimization | Nonlinear solver via LAPACK extensions |
| Dense vectors / matrices | BLAS, LAPACK — fastest available on Intel hardware |
| Linear regression | LAPACK `DGELS`, `DGELSD` |
| Interpolation | Data Fitting component (`df*` routines) |

- **Assessment**: Best raw performance on Intel hardware due to hand-optimised SIMD code.
  Not portable (x86-only); proprietary license restricts redistribution.
  The VSL random distributions are **3-5x faster** than GSL for vectorised draws.
  Suitable for high-frequency Monte Carlo simulation on dedicated servers.

### 6. QuantLib

- **License**: QL License (permissive BSD-like)  
- **Language**: C++, widely used in financial mathematics  
- **Coverage**:

| MGlib area | QuantLib equivalent |
|---|---|
| Random number generation | `QuantLib::MersenneTwisterUniformRng`, `QuantLib::SobolRsg` |
| Probability distributions (Normal) | `QuantLib::CumulativeNormalDistribution`, `QuantLib::InverseCumulativeNormal` |
| Monte Carlo integration | `QuantLib::MonteCarloModel`, `QuantLib::PathGenerator` |
| Root finding | `QuantLib::Brent`, `QuantLib::NewtonSafe`, `QuantLib::Bisection` |
| Interpolation | `QuantLib::CubicInterpolation`, `QuantLib::LinearInterpolation` |
| Yield curve construction | `QuantLib::ZeroCurve` — direct overlap with `MG_ZeroCurve` |
| Option pricing | `QuantLib::BlackCalculator`, `QuantLib::HullWhite` |

- **Assessment**: Most functionally aligned with MGlib's domain (interest rate products,
  Black-Scholes, Hull-White, caps/floors/swaptions). Would provide significant overlap
  with MGlib's own pricing layer. Adopting QuantLib would likely mean replacing large
  parts of MGlib's business logic, not just the numerical layer.
- **Performance**: Not optimised for raw throughput; designed for correctness and
  expressiveness in financial modelling.

### 7. NAG Library (Numerical Algorithms Group)

- **License**: Commercial  
- **Coverage**: All areas used by MGlib and more; considered the gold standard for numerical accuracy
- **Assessment**: Extremely high accuracy and thorough documentation, but requires a
  commercial licence. Not suitable for open-source use.

---

## Head-to-Head Feature Matrix

| Feature | GSL 2.8 | Boost.Math | Eigen 3 | Armadillo | oneMKL | QuantLib |
|---|:---:|:---:|:---:|:---:|:---:|:---:|
| RNG (40+ types) | ✅ | ✅ partial | ❌ | ❌ | ✅ | ✅ partial |
| Quasi-random (Sobol) | ✅ | ✅ | ❌ | ❌ | ✅ | ✅ |
| Normal / Uniform / Lognormal CDF + InvCDF | ✅ | ✅ | ❌ | ✅ partial | ✅ | ✅ partial |
| Ziggurat / Ratio-method Gaussian sampling | ✅ | ❌ | ❌ | ❌ | ✅ | ❌ |
| Plain / MISER / VEGAS MC integration | ✅ | ❌ | ❌ | ❌ | ❌ | ✅ partial |
| Bisection / Brent / Newton root finding | ✅ | ✅ | ❌ | ❌ | ✅ partial | ✅ |
| Cubic spline / Akima interpolation | ✅ | ✅ | ❌ | ❌ | ✅ | ✅ |
| Linear / multifit regression | ✅ | ❌ | ✅ | ✅ | ✅ | ❌ |
| Dense vector / matrix (gsl_vector / gsl_matrix) | ✅ | ❌ | ✅✅ | ✅✅ | ✅✅ | ❌ |
| Finance-domain functions (Black, HW1F) | ❌ | ❌ | ❌ | ❌ | ❌ | ✅✅ |
| License | GPL-3 | Boost | MPL-2 | Apache-2 | Proprietary | QL (permissive) |
| Header-only | ❌ | ✅ | ✅ | ❌ | ❌ | ❌ |
| C++ API quality | C API | Modern C++ | Modern C++ | Modern C++ | C API | Modern C++ |

---

## Indicative Performance Comparison

The following numbers are indicative order-of-magnitude comparisons based on published
benchmarks and community results. They are **not** measurements run on this codebase.

| Operation | GSL 2.8 | Boost.Math | Eigen 3 | oneMKL |
|---|---|---|---|---|
| 1M Gaussian draws (MT19937) | baseline | ~1× | — | ~3-5× (vectorised VSL) |
| Normal CDF (scalar loop 1M) | baseline | ~0.9× | — | ~2× |
| Normal InvCDF (scalar loop 1M) | baseline | ~1.1× | — | ~2× |
| MISER MC (1M calls) | baseline | — | — | — |
| Dense 500×500 matrix multiply | baseline | — | ~2-3× | ~5-10× |
| Brent root finder (1000 iter) | baseline | ~0.95× | — | — |
| Cubic spline eval (1M points) | baseline | ~1.1× | — | — |

---

## Recommendation

Given MGlib's scope (interest-rate quant library, open source, cross-platform), the
recommended strategy is a **targeted migration** rather than a wholesale library swap:

1. **Keep GSL** for: Monte Carlo integration (MISER/VEGAS), root finding, minimization,
   interpolation, and quasi-random sequences. No single alternative covers all of these
   with comparable maturity.

2. **Consider Eigen** as a future replacement for `MG_Vector`/`MG_Matrix` (currently
   wrapping `gsl_vector`/`gsl_matrix`). Eigen's C++ API is significantly cleaner, and
   the performance uplift on matrix operations is real and measurable.

3. **Consider Boost.Math** as a future replacement for the distribution layer
   (`gsl_randist`/`gsl_cdf`). Boost is header-only, has a permissive licence, and covers
   Normal, Uniform, and Lognormal distributions with a clean C++ API.

4. **Do not adopt oneMKL** unless there is a specific performance bottleneck on
   Intel hardware that cannot be addressed by upgrading GSL.

5. **Do not adopt QuantLib** as a drop-in for GSL. QuantLib overlaps with MGlib's
   own business logic (pricing models, schedules, yield curves) and adopting it would
   require a substantial architectural rework.

The **nearest-term improvement** with the least disruption is the GSL version upgrade
described in `docs/gsl-migration-plan.md`.
