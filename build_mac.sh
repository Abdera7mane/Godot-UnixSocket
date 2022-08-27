#!/bin/sh
target=$1

if [ ! -d bin ]; then
    mkdir bin
fi

scons platform=osx macos_arch=universal target=$target generate_bindings=yes bits=64 -j$(sysctl -n hw.logicalcpu)
