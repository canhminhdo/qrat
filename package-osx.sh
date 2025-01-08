#!/bin/bash

mkdir -p build-osx
cmake -S . -B build-osx -D CMAKE_BUILD_TYPE=Release
cmake --build build-osx --config Release -j 8
ctest --test-dir build-osx
cmake --install build-osx
cpack -G "ZIP" --config build-osx/CPackConfig.cmake -B package
rm -rf build-osx