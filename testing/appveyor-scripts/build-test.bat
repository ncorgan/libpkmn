@setlocal enableextensions enabledelayedexpansion
@echo off

mkdir c:\projects\libpkmn\test-env\build

cd c:\projects\libpkmn\test-env\build
if not !ERRORLEVEL!==0 goto fail

dir %BOOST_LIBRARYDIR%

cmake -G %CMAKE_GENERATOR_NAME% ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBOOST_ROOT="%BOOST_ROOT%" ^
    -DBOOST_LIBRARYDIR=%BOOST_LIBRARYDIR% ^
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
