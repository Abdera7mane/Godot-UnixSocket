#!/bin/sh

cd godot-cpp/
CORES=$(grep -c ^processor /proc/cpuinfo 2>/dev/null || sysctl -n hw.ncpu)
scons platform=osx target=$1 generate_bindings=yes macos_arch=x86_64 -j$CORES
scons platform=osx target=$1 generate_bindings=yes macos_arch=arm64 -j$CORES
cd ..

if [ ! -d bin ]; then
    mkdir bin
fi

scons platform=x11 target=$1
