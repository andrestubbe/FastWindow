@echo off
chcp 65001 >nul
cd /d "%~dp0"
echo ===========================================
echo âš¡ FastWindow Demo Launcher âš¡
echo ===========================================

echo Building native core...
call compile.bat

echo.
echo Installing FastWindow to local Maven cache...
call mvn -q clean install -DskipTests

echo.
echo Running FastWindow Black Window Demo...
cd examples\Demo
call mvn -q compile exec:java
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Demo execution failed.
)
cd ..\..
pause
