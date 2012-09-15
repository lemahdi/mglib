@echo off
REM ==============================
REM Start from a clean environment
REM ==============================
REM rmdir /s /q obj
REM rmdir /s /q bin
REM rmdir /s /q lib
REM rmdir /s /q include

rmdir /s /q gsl
if not exist gsl\include\gsl mkdir gsl\include\gsl
if not exist gsl\lib mkdir gsl\lib
if not exist gsl\example mkdir gsl\example

REM devenv /build Release ..\gsl.sln
REM devenv /build Debug ..\gsl.sln
REM devenv /build "Release MT" ..\gsl.sln
REM devenv /build "Debug MT" ..\gsl.sln

REM ==========================
REM Copy include files locally
REM ===========================
copy include\gsl\* gsl\include\gsl

copy ..\BUGS gsl
copy ..\COPYING gsl
copy ..\ChangeLog gsl
copy ..\NEWS gsl
copy ..\README gsl
copy ..\THANKS gsl

copy lib\*.lib gsl\lib
copy lib\*.pdb gsl\lib
copy README gsl\README.win32

copy example\example.cpp gsl\example
copy example\example.sln gsl\example
copy example\example.vcproj gsl\example





