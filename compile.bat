@echo off
set PROJECT_NAME=fastwindow
set JAVA_PATH=C:\Program Files\Java\jdk-25
set VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools

echo ===========================================
echo FastWindow Native Builder (v0.1.0)
echo ===========================================

if not exist build mkdir build
if not exist src\main\resources\native mkdir src\main\resources\native

echo.
echo Using JDK: %JAVA_PATH%
echo Using VS:  %VS_PATH%

call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat"

echo.
echo Compiling C++ Native Library...
cl.exe /LD /Fe:src\main\resources\native\%PROJECT_NAME%.dll native\fastwindow.cpp /I"%JAVA_PATH%\include" /I"%JAVA_PATH%\include\win32" /EHsc /link /SUBSYSTEM:WINDOWS user32.lib gdi32.lib dwmapi.lib "%JAVA_PATH%\lib\jawt.lib" /def:native\FastWindow.def

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ===========================================
    echo BUILD SUCCESSFUL 
    echo DLL Location: src\main\resources\native\%PROJECT_NAME%.dll
    echo ===========================================
) else (
    echo.
    echo !!!!!!!!! BUILD FAILED !!!!!!!!!
    pause
)
