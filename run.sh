#!/usr/bin/env bash

# Default to Debug mode for running, feel free to change to Release
BUILD_TYPE="Debug"

echo "[INFO] Auto-building Sandbox ($BUILD_TYPE) before running..."

# cmake -G "Ninja" \
#    -DCMAKE_CXX_COMPILER=clang++ \
#    -DCMAKE_C_COMPILER=clang \
#    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
#    -B build

if [ $? -ne 0 ]; then
    echo "[ERROR] CMake configuration failed!"
    exit $?
fi

cmake --build build --config "$BUILD_TYPE" --parallel 3 --target Sandbox Caldera


if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed! Unable to run Sandbox."
    exit $?
fi

echo
echo "[INFO] Starting Sandbox..."
echo "===================================="

# Navigate to where the Sandbox executable is outputted and run it
# Adjust this path if your generator outputs to "build/Sandbox/" instead of "build/"
if [ -f "build/Sandbox/Sandbox" ]; then
    cd build/Sandbox || exit 1

    if [ "$BUILD_TYPE" = "Debug" ]; then
        gdb -ex run ./Sandbox
    else
        ./Sandbox
    fi
else
    echo "[ERROR] Could not find Sandbox executable!"
fi

echo "===================================="


