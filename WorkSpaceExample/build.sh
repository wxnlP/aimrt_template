#!/bin/bash

# exit on error and print each command
set -e

if [ -d ./build/install ]; then
    rm -rf ./build/install
fi

cmake -B build -G Ninja $@

cmake --build build --config Release --parallel $(nproc)
