@echo off
setlocal enabledelayedexpansion

echo ===========================================
echo FastWindow Native Builder (v0.2.0)
echo ===========================================

set LIB_NAME=fastwindow

:: 1. Detect JAVA_HOME
if "%JAVA_HOME%"=="" (
    for %%d in (
        "C:\Program Files\Java\jdk-21.0.12.1"
        "C:\Program Files\Java\jdk-21"
        "C:\Program Files\Java\jdk-17"
        "C:\Program Files\Java\jdk-25"
    ) do (
        if exist %%d (
            set "JAVA_HOME=%%~d"
            goto :found_java
        )
    )
)
:found_java
if "%JAVA_HOME%"=="" (
    echo Error: JAVA_HOME not found.
    exit /b 1
)
echo Using JAVA_HOME: %JAVA_HOME%

:: 2. Detect Visual Studio vcvars64.bat
where cl.exe >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    set "VS_DIR="
    for %%v in (
        "C:\Program Files\Microsoft Visual Studio\18\Community"
        "C:\Program Files\Microsoft Visual Studio\2026\Community"
        "C:\Program Files\Microsoft Visual Studio\2022\Community"
        "C:\Program Files\Microsoft Visual Studio\2022\BuildTools"
        "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools"
    ) do (
        if exist "%%~v\VC\Auxiliary\Build\vcvars64.bat" (
            set "VS_DIR=%%~v"
            goto :found_vs
        )
    )
    :found_vs
    if not defined VS_DIR (
        echo Error: Visual Studio compiler not found.
        exit /b 1
    )
    echo Found Visual Studio at: !VS_DIR!
    call "!VS_DIR!\VC\Auxiliary\Build\vcvars64.bat" >nul
)

if not exist release mkdir release
if not exist build mkdir build
if not exist src\main\resources\native mkdir src\main\resources\native

echo.
echo [COMPILER] Compiling Native Library...

cl.exe /O2 /W3 /std:c++17 /MD /EHsc /LD ^
   /I "%JAVA_HOME%\include" ^
   /I "%JAVA_HOME%\include\win32" ^
   /Fo:build\ ^
   /Fe:release\%LIB_NAME%.dll ^
   native\fastwindow.cpp ^
   user32.lib gdi32.lib dwmapi.lib uxtheme.lib "%JAVA_HOME%\lib\jawt.lib" ^
   /link /DLL /MACHINE:X64 /def:native\FastWindow.def

if %ERRORLEVEL% EQU 0 (
    copy release\%LIB_NAME%.dll . >nul 2>&1
    copy release\%LIB_NAME%.dll src\main\resources\native\%LIB_NAME%.dll >nul 2>&1
    echo.
    echo ===========================================
    echo [SUCCESS] DLL built and copied to resources
    echo ===========================================
) else (
    echo.
    echo [FAILED] Compilation failed.
    exit /b 1
)
endlocal

