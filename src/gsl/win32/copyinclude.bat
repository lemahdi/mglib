@echo off
rmdir /s /q include
if not exist include\gsl mkdir include\gsl

if not exist include\gsl exit

REM ==========================
REM Copy include files locally
REM ===========================

copy ..\gsl_*.h include\gsl

set DIRS=blas block bspline cblas cdf cheb combination complex const deriv dht diff eigen err fft fit gsl histogram ieee-utils integration interpolation linalg matrix min monte multifit multimin multiroots multiset ntuple ode-initval ode-initval2 permutation poly qrng randist rng roots siman sort specfunc statistics sum sys test utils vector wavelet



for %%f in (%DIRS%) do copy ..\%%f\gsl_*.h include\gsl




