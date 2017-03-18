@setlocal enableextensions enabledelayedexpansion
@echo off

:: Set variables
if "%BITNESS%"=="32" (
    set CMAKE_GENERATOR_NAME="Visual Studio 14 2015"
    set CMAKE_PREFIX_PATH=C:\Qt\5.7\msvc2015
    set PYTHON_ROOT=C:\Python%PYTHON_VERSION%
) else (
    set CMAKE_GENERATOR_NAME="Visual Studio 14 2015 Win64"
    set CMAKE_PREFIX_PATH=C:\Qt\5.7\msvc2015_64
    set PYTHON_ROOT=C:\Python%PYTHON_VERSION%-x64
)
set BOOST_LIBRARY_DIR="%BOOST_ROOT%\lib%BITNESS%-msvc-14.0"
set PYTHON_EXE="!PYTHON_ROOT!\python.exe"
set PYTHON_PIP_EXE="!PYTHON_ROOT!\Scripts\pip.exe"

mkdir c:\projects\libpkmn\test-env\build

cd c:\projects\libpkmn\test-env\build
if not !ERRORLEVEL!==0 goto fail

!PYTHON_PIP_EXE! install ply CppHeaderParser nose_parameterized
if not !ERRORLEVEL!==0 goto fail

cmake -G %CMAKE_GENERATOR_NAME% ^
    -DAPPVEYOR=TRUE ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DDESIRED_QT_VERSION=5 ^
    -DBOOST_ROOT="%BOOST_ROOT%" ^
    -DBoost_LIBRARY_DIRS=%BOOST_LIBRARY_DIR% ^
    -DPYTHON_EXECUTABLE=!PYTHON_EXE! ^
    ..\..
if not !ERRORLEVEL!==0 goto fail

msbuild /p:configuration=Release ALL_BUILD.vcxproj
if not !ERRORLEVEL!==0 goto fail

testing\unit-tests\lua\lua_pokemon_pc_test.bat
testing\unit-tests\python\python_pokemon_pc_test.bat

ctest -E "python_paths_test" --output-on-failure
if not !ERRORLEVEL!==0 goto fail

goto pass

endlocal

:fail
exit /b 1

:pass
exit /b 0
