@echo off
echo ===========================================
echo âš¡ FastWindow Demo Launcher âš¡
echo ===========================================

echo.
echo Building native core...
call compile.bat

echo.

echo.
echo Running Demo...
cd examples\Demo
call mvn compile exec:java
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo âŒ Demo execution failed.
)
cd ..\..
pause
