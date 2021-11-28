#!/bin/sh

cd godot-cpp/
CORES=$(grep -c \^processor /proc/cpuinfo 2>/dev/null || sysctl -n hw.ncpu)
scons platform=linux target=$1 generate_bindings=yes bits=64 -j$CORES
cd ..

scons platform=x11 target=$1