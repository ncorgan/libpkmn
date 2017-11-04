@echo off

:: Now we declare a scope
Setlocal EnableDelayedExpansion EnableExtensions

if defined MINGW_ARCH ( goto :mingw )

set arch=x86

if "%BITNESS%" EQU "64" ( set arch=x86_amd64 )

if "x%CMAKE_GENERATOR_NAME:2017=%" == "x%CMAKE_GENERATOR_NAME" (
    set SET_VS_ENV="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
) else (
    set SET_VS_ENV= "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
)

:: Visual Studio detected
endlocal & call %SET_VS_ENV% %arch%
goto :eof

:: MinGW detected
:mingw
endlocal & set PATH=%MINGW_DIR%\bin;%PATH%
