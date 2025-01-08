# QRAT: A Reachability Analysis Tool for Quantum Programs
This repository presents a reachablity analysis tool for quantum programs called `QRAT`. The tool is written in C++ using [MQT Core](https://github.com/cda-tum/mqt-core) to simulate quantum computation.

## Dependencies
- [MQT Core](https://github.com/cda-tum/mqt-core) - The Backbone of the Munich Quantum Toolkit
- [C++ Compilier](https://clang.llvm.org) - Supporting C++17
- [CMake](https://cmake.org) - A minimum version of 3.19
- [google/googletest](https://github.com/google/googletest) - A testing framework for C++ (only used in tests)

## Getting Started

`QRAT` can be installed on your system after building it from source.
```shell
$ git clone --recurse-submodules https://github.com/canhminhdo/MQT-QRAT.git
$ cd MQT-QRAT
$ cmake -S . -B build -D CMAKE_BUILD_TYPE=Release # configure the project to be built with `Release` mode
$ cmake --build build --config Release -j 8 # building the project with `Release` mode and 8 workers in parallel
$ ctest --test-dir build # testing the project with some test cases
$ cmake --install build --config Release # install the project with `Release` mode in the `install` folder
$ cpack -G "ZIP" --config build/CPackConfig.cmake -B package # package the project to be distributed in the `package` directory
```