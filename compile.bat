@echo off
setlocal
set PROJECT_NAME=fastwindow
set JAVA_PATH=C:\Program Files\Java\jdk-25
set VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools

echo ===========================================
echo FastWindow Native Builder (v0.1.0)
echo ===========================================

if not exist src\main\resources\native mkdir src\main\resources\native

:: Only call vcvars if cl.exe isn't already in the path
where cl.exe >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [SYSTEM] Loading Visual Studio Environment...
    call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat" >nul
)

echo.
echo [COMPILER] Compiling Native Library...

:: Constructing arguments carefully to avoid length issues
set "INCLUDES=/I"%JAVA_PATH%\include" /I"%JAVA_PATH%\include\win32""
set "LIBS=user32.lib gdi32.lib "%JAVA_PATH%\lib\jawt.lib""
set "FLAGS=/LD /EHsc /nologo"
set "OUTPUT=/Fe:src\main\resources\native\%PROJECT_NAME%.dll"

cl.exe %FLAGS% %OUTPUT% native\fastwindow.cpp %INCLUDES% /link /SUBSYSTEM:WINDOWS %LIBS% /def:native\FastWindow.def

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ===========================================
    echo ✅ BUILD SUCCESSFUL
    echo ===========================================
) else (
    echo.
    echo ❌ BUILD FAILED
    pause
)
endlocal
