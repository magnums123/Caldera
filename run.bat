@echo off
setlocal

:: Default to Debug mode for running, feel free to change to Release
set "BUILD_TYPE=Debug"

echo [INFO] Auto-building Sandbox (%BUILD_TYPE%) before running...
cmake -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -B build

if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake configuration failed!
    exit /b %ERRORLEVEL%
)

cmake --build build --config %BUILD_TYPE% --target Sandbox Engine

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed! Unable to run Sandbox.
    exit /b %ERRORLEVEL%
)

echo.
echo [INFO] Starting Sandbox...
echo ====================================
:: Navigate to where the Sandbox executable is outputted and run it
:: Adjust this path if your generator outputs to "build/Sandbox/" instead of "build/"
if exist "build\Sandbox\Sandbox.exe" (
    cd build\Sandbox
    if /I "%BUILD_TYPE%"=="Debug" (
      gdb -ex run Sandbox.exe
    ) else (
      Sandbox.exe
    )
) else (
    echo [ERROR] Could not find Sandbox.exe executable!
)
echo ====================================

endlocal
