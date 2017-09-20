@setlocal enableextensions enabledelayedexpansion
@echo off

:: Set variables
set BOOST_ROOT=C:/Libraries/boost_%BOOST_VERSION%_0
dir C:\Libraries\boost_%BOOST_VERSION%_0
set CMAKE_PREFIX_PATH=%QT_DIR%
if "x%CMAKE_GENERATOR_NAME:2017=%"=="x%CMAKE_GENERATOR_NAME%" (
    set BOOST_LIBRARY_DIR="%BOOST_ROOT%\lib%BITNESS%-msvc-15.0"
) else (
    set BOOST_LIBRARY_DIR="%BOOST_ROOT%\lib%BITNESS%-msvc-14.0"
)
if "%BITNESS%"=="32" (
    set PYTHON_ROOT=C:\Python%PYTHON_VERSION%
    set "PATH=C:\Program Files (x86)\PKMN\bin;%BOOST_LIBRARY_DIR%;%CMAKE_PREFIX_PATH%\bin;%PATH%"
    set "LIB=C:\Program Files (x86)\PKMN\lib;%LIB%"
    set "INCLUDE=C:\Program Files (x86)\PKMN\include;%INCLUDE%"
) else (
    set PYTHON_ROOT=C:\Python%PYTHON_VERSION%-x64
    set "PATH=C:\Program Files\PKMN\bin;%BOOST_LIBRARY_DIR%;%CMAKE_PREFIX_PATH%\bin;%PATH%"
    set "LIB=C:\Program Files\PKMN\lib;%LIB%"
    set "INCLUDE=C:\Program Files\PKMN\include;%INCLUDE%"
)
set PYTHON_EXE="!PYTHON_ROOT!\python.exe"
set PYTHON_PIP_EXE="!PYTHON_ROOT!\Scripts\pip.exe"

mkdir c:\projects\libpkmn\test-env\build

cd c:\projects\libpkmn\test-env\build
if not !ERRORLEVEL!==0 goto fail

!PYTHON_PIP_EXE! install ply CppHeaderParser nose_parameterized
if not !ERRORLEVEL!==0 goto fail

cmake -G "%CMAKE_GENERATOR_NAME%" ^
    -DAPPVEYOR=TRUE ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DDESIRED_QT_VERSION=5 ^
    -DBOOST_ROOT="%BOOST_ROOT%" ^
    -DBoost_LIBRARY_DIRS=!BOOST_LIBRARY_DIR! ^
    -DPKMN_ENABLE_RUBY=OFF ^
    -DPKMN_ENABLE_QT=OFF ^
    -DPYTHON_EXECUTABLE=!PYTHON_EXE! ^
    ..\..
if not !ERRORLEVEL!==0 goto fail

cmake --build . --config "Release"
if not !ERRORLEVEL!==0 goto fail

ctest -E "python_paths_test" --output-on-failure
if not !ERRORLEVEL!==0 goto fail

:: So the log isn't too verbose
echo Installing...
msbuild /p:configuration=Release INSTALL.vcxproj 1>nul 2>nul
if not !ERRORLEVEL!==0 goto fail

mkdir c:\projects\libpkmn\testing\applications\cpp\build
cd c:\projects\libpkmn\testing\applications\cpp\build
cmake -G "%CMAKE_GENERATOR_NAME%" ..
if not !ERRORLEVEL!==0 goto fail
cmake --build . --config "Release"
if not !ERRORLEVEL!==0 goto fail

mkdir c:\projects\libpkmn\testing\applications\c\build
cd c:\projects\libpkmn\testing\applications\c\build
cmake -G "%CMAKE_GENERATOR_NAME%" ..
if not !ERRORLEVEL!==0 goto fail
cmake --build . --config "Release"
if not !ERRORLEVEL!==0 goto fail

goto pass

endlocal

:fail
exit /b 1

:pass
exit /b 0
