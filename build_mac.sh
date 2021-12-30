#!/bin/sh

target=$1

cd godot-cpp/
CORES=$(grep -c ^processor /proc/cpuinfo 2>/dev/null || sysctl -n hw.ncpu)
scons platform=osx target=$target generate_bindings=yes macos_arch=x86_64 -j$CORES
scons platform=osx target=$target generate_bindings=yes macos_arch=arm64 -j$CORES

lipo -create ./bin/libgodot-cpp.osx.${target}.x86_64.a ./bin/libgodot-cpp.osx.${target}.arm64.a -output ./bin/libgodot-cpp.osx.${target}.universal.a
cd ..


if [ ! -d bin ]; then
    mkdir bin
fi

scons platform=osx target=$target
