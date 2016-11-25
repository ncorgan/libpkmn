@setlocal enableextensions enabledelayedexpansion
@echo off

:: Set variables
if "%BITNESS%"=="32" set CMAKE_GENERATOR_NAME="Visual Studio 14 2015"
if "%BITNESS%"=="64" set CMAKE_GENERATOR_NAME="Visual Studio 14 2015 Win64"
if "%BITNESS%"=="32" set PYTHON_ROOT="C:\Python27"
if "%BITNESS%"=="64" set PYTHON_ROOT="C:\Python27-x64"
set BOOST_LIBRARY_DIR="%BOOST_ROOT%\lib%BITNESS%-msvc-14.0"

mkdir c:\projects\libpkmn\test-env\build

cd c:\projects\libpkmn\test-env\build
if not !ERRORLEVEL!==0 goto fail

"!PYTHON_ROOT!\scripts\pip" install ply CppHeaderParser
if not !ERRORLEVEL!==0 goto fail

cmake -G %CMAKE_GENERATOR_NAME% ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBOOST_ROOT="%BOOST_ROOT%" ^
    -DBoost_LIBRARY_DIRS=%BOOST_LIBRARY_DIR% ^
    -DPYTHON_EXECUTABLE="%PYTHON_ROOT%\python.exe" ^
    ..\..
if not !ERRORLEVEL!==0 goto fail

msbuild /p:configuration=Release ALL_BUILD.vcxproj
if not !ERRORLEVEL!==0 goto fail

ctest --output-on-failure
if not !ERRORLEVEL!==0 goto fail

goto pass

endlocal

:fail
exit /b 1

:pass
exit /b 0
