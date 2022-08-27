#!/bin/sh
target=$1

if [ ! -d bin ]; then
    mkdir bin
fi

scons platform=linux target=$target generate_bindings=yes bits=64 -j$(nproc)
