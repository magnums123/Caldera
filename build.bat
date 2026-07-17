@echo off
setlocal enabledelayedexpansion

:: Ask for Build Type
echo ====================================
echo Select Build Type:
echo [1] Debug
echo [2] Release
echo ====================================
set /p build_choice="Enter choice (1-2): "

if "%build_choice%"=="1" (
    set BUILD_TYPE=Debug
) else if "%build_choice%"=="2" (
    set BUILD_TYPE=Release
) else (
    echo Invalid choice. Defaulting to Debug.
    set BUILD_TYPE=Debug
)

:: Ask what to build
echo.
echo ====================================
echo Select Target:
echo [1] Entire Project (All)
echo [2] Sandbox (Executable only)
echo [3] Engine (Library only)
echo ====================================
set /p target_choice="Enter choice (1-3): "

if "%target_choice%"=="1" (
    set TARGET=
) else if "%target_choice%"=="2" (
    set TARGET=--target Sandbox
) else if "%target_choice%"=="3" (
    set TARGET=--target Engine
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
