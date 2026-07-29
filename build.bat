@echo off
setlocal enabledelayedexpansion

:: Ask for Build Type
REM echo ====================================
REM echo Select Build Type:
REM echo [1] Debug
REM echo [2] Release
REM echo ====================================
REM set /p build_choice="Enter choice (1-2): "
set build_choice=1

if "%build_choice%"=="1" (
    set BUILD_TYPE=Debug
) else if "%build_choice%"=="2" (
    set BUILD_TYPE=Release
) else (
    echo Invalid choice. Defaulting to Debug.
    set BUILD_TYPE=Debug
)

:: Ask what to build
REM echo.
REM echo ====================================
REM echo Select Target:
REM echo [1] Entire Project (All)
REM echo [2] Sandbox (Executable only)
REM echo [3] Engine (Library only)
REM echo ====================================
REM set /p target_choice="Enter choice (1-3): "
set target_choice=1

if "%target_choice%"=="1" (
    set TARGET=
) else if "%target_choice%"=="2" (
    set TARGET=--target Sandbox
) else if "%target_choice%"=="3" (
    set TARGET=--target Caldera
) else (
    echo Invalid choice. Building entire project.
    set TARGET=
)

echo.
echo [INFO] Configuring CMake with Clang (%BUILD_TYPE%)...
:: We use -G "Ninja" for fast, robust Clang builds on Windows
cmake -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -B build

if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake configuration failed!
    exit /b %ERRORLEVEL%
)

echo.
echo [INFO] Building targets...
cmake --build build --config %BUILD_TYPE% %TARGET%

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed!
    exit /b %ERRORLEVEL%
)

echo [SUCCESS] Build finished successfully.
endlocal
