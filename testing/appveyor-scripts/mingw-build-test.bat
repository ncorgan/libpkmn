@setlocal enableextensions enabledelayedexpansion
@echo off

:: Set variables
set BOOST_ROOT=%MINGW_DIR%
set BOOST_INCLUDE_DIR=%BOOST_ROOT%\include\boost
set BOOST_LIBRARY_DIR=%BOOST_ROOT%\lib

set INCLUDE=%BOOST_INCLUDE_DIR%;%INCLUDE%
set LIB=%BOOST_LIBRARY_DIR%;%LIB%

set PATH=%MINGW_DIR%\bin;C:\msys64\usr\bin;%PATH%
bash -lc "pacman --needed --noconfirm -S mingw-w64-%MINGW_ARCH%-boost"

set CMAKE_PREFIX_PATH=%QT_DIR%

if "%MINGW_ARCH%"=="i686" (
    set PYTHON_ROOT=C:\Python%PYTHON_VERSION%
    set "PATH=C:\Program Files (x86)\PKMN\bin;%CMAKE_PREFIX_PATH%\bin;%PATH%"
    set "LIB=C:\Program Files (x86)\PKMN\lib;%LIB%"
    set "INCLUDE=C:\Program Files (x86)\PKMN\include;%INCLUDE%"
) else (
    set PYTHON_ROOT=C:\Python%PYTHON_VERSION%-x64
    set "PATH=C:\Program Files\PKMN\bin;%CMAKE_PREFIX_PATH%\bin;%PATH%"
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

cmake -G "MSYS Makefiles" ^
    -DAPPVEYOR=TRUE ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DDESIRED_QT_VERSION=5 ^
    -DBOOST_ROOT="%BOOST_ROOT%" ^
    -DBoost_INCLUDE_DIRS=%BOOST_INCLUDE_DIR% ^
    -DBoost_LIBRARY_DIRS=%BOOST_LIBRARY_DIR% ^
    -DPKMN_ENABLE_CSHARP=OFF ^
    -DPKMN_ENABLE_LUA=OFF ^
    -DPKMN_ENABLE_PYTHON=OFF ^
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
cmake --build . --target install --config "Release" 1>nul 2>nul
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
