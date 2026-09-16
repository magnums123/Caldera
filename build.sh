#!/usr/bin/env bash

# Ask for Build Type
# echo "===================================="
# echo "Select Build Type:"
# echo "[1] Debug"
# echo "[2] Release"
# echo "===================================="
# read -r -p "Enter choice (1-2): " build_choice
build_choice=1

if [ "$build_choice" = "1" ]; then
    BUILD_TYPE="Debug"
elif [ "$build_choice" = "2" ]; then
    BUILD_TYPE="Release"
else
    echo "Invalid choice. Defaulting to Debug."
    BUILD_TYPE="Debug"
fi

# Ask what to build
# echo
# echo "===================================="
# echo "Select Target:"
# echo "[1] Entire Project (All)"
# echo "[2] Sandbox (Executable only)"
# echo "[3] Engine (Library only)"
# echo "===================================="
# read -r -p "Enter choice (1-3): " target_choice
target_choice=1

if [ "$target_choice" = "1" ]; then
    TARGET=()
elif [ "$target_choice" = "2" ]; then
    TARGET=(--target Sandbox)
elif [ "$target_choice" = "3" ]; then
    TARGET=(--target Caldera)
else
    echo "Invalid choice. Building entire project."
    TARGET=()
fi

echo
echo "[INFO] Configuring CMake with Clang ($BUILD_TYPE)..."
# We use -G "Ninja" for fast, robust Clang builds on Linux
cmake -G "Ninja" \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -B build

if [ $? -ne 0 ]; then
    echo "[ERROR] CMake configuration failed!"
    exit $?
fi

echo
echo "[INFO] Building targets..."
cmake --build build --config "$BUILD_TYPE" "${TARGET[@]}"

if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed!"
    exit $?
fi

echo "[SUCCESS] Build finished successfully."
