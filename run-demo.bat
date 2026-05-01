@echo off
echo ===========================================
echo ⚡ FastWindow Demo Launcher ⚡
echo ===========================================

echo.
echo Building native core...
call compile.bat

echo.
echo Building main project...
call mvn clean install -DskipTests
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ Maven build failed.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo Running Demo...
cd examples\Demo
call mvn compile exec:java
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ Demo execution failed.
)
cd ..\..
pause
