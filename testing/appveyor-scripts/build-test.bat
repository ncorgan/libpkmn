@setlocal enableextensions enabledelayedexpansion
@echo off

:: Set variables
set BOOST_ROOT=C:/Libraries/boost_%BOOST_VERSION%_0
set BOOST_LIBRARY_DIR="%BOOST_ROOT%\lib%BITNESS%-msvc-14.0"
set "PATH=C:\Ruby%RUBY_VERSION%\bin;%PATH%"
if "%BITNESS%"=="32" (
    set CMAKE_GENERATOR_NAME="Visual Studio 14 2015"
    set CMAKE_PREFIX_PATH=C:\Qt\5.7\msvc2015
    set PYTHON_ROOT=C:\Python%PYTHON_VERSION%
    set RUBY_LIBRARY="C:\Ruby%RUBY_VERSION%\lib\libmsvcrt-ruby%RUBY_VERSION%0.dll.a"
    set "PATH=C:\Program Files (x86)\PKMN\bin;%BOOST_LIBRARY_DIR%;%CMAKE_PREFIX_PATH%\bin;%PATH%"
    set "LIB=C:\Program Files (x86)\PKMN\lib;%LIB%"
    set "INCLUDE=C:\Program Files (x86)\PKMN\include;%INCLUDE%"
) else (
    set CMAKE_GENERATOR_NAME="Visual Studio 14 2015 Win64"
    set CMAKE_PREFIX_PATH=C:\Qt\5.7\msvc2015_64
    set PYTHON_ROOT=C:\Python%PYTHON_VERSION%-x64
    set RUBY_LIBRARY="C:\Ruby%RUBY_VERSION%\lib\libx64-msvcrt-ruby%RUBY_VERSION%0.dll.a"
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

cmake -G %CMAKE_GENERATOR_NAME% ^
    -DAPPVEYOR=TRUE ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DDESIRED_QT_VERSION=5 ^
    -DBOOST_ROOT="%BOOST_ROOT%" ^
    -DBoost_LIBRARY_DIRS=%BOOST_LIBRARY_DIR% ^
    -DRUBY_LIBRARY=%RUBY_LIBRARY% ^
    -DPKMN_ENABLE_RUBY=OFF ^
    -DPYTHON_EXECUTABLE=!PYTHON_EXE! ^
    ..\..
if not !ERRORLEVEL!==0 goto fail

msbuild /p:configuration=Release ALL_BUILD.vcxproj
if not !ERRORLEVEL!==0 goto fail

ctest -E "python_paths_test" --output-on-failure
if not !ERRORLEVEL!==0 goto fail

:: So the log isn't too verbose
echo Installing...
msbuild /p:configuration=Release INSTALL.vcxproj 1>nul 2>nul
if not !ERRORLEVEL!==0 goto fail

mkdir c:\projects\libpkmn\testing\applications\cpp\build
cd c:\projects\libpkmn\testing\applications\cpp\build
cmake -G %CMAKE_GENERATOR_NAME% ..
if not !ERRORLEVEL!==0 goto fail
msbuild /p:configuration=Release ALL_BUILD.vcxproj
if not !ERRORLEVEL!==0 goto fail

mkdir c:\projects\libpkmn\testing\applications\c\build
cd c:\projects\libpkmn\testing\applications\c\build
cmake -G %CMAKE_GENERATOR_NAME% ..
if not !ERRORLEVEL!==0 goto fail
msbuild /p:configuration=Release ALL_BUILD.vcxproj
if not !ERRORLEVEL!==0 goto fail

goto pass

endlocal

:fail
exit /b 1

:pass
exit /b 0
