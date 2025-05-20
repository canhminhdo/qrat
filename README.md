# QCheck: A Framework for Probabilistic Model Checking of Quantum Programs
This repository presents `QCheck` a framework for probabilistic model checking of quantum programs. The framework is implemented in C++ using [MQT Core](https://github.com/cda-tum/mqt-core) to simulate quantum computation and probabilistic model checkers like PRISM and Storm as alternative backends for verification.

## Dependencies
- [MQT Core](https://github.com/cda-tum/mqt-core) - The Backbone of the Munich Quantum Toolkit
- [PRISM](https://www.prismmodelchecker.org) - A version of 4.8.1
- [Storm](https://www.stormchecker.org) - A version of 1.9.0
- [C++ Compilier](https://clang.llvm.org) - Supporting C++17
- [CMake](https://cmake.org) - A minimum version of 3.19
- [google/googletest](https://github.com/google/googletest) - A testing framework for C++ (only used in tests)
- [Flex](https://github.com/westes/flex) - A version of 2.6.4
- [Bison](https://www.gnu.org/software/bison/) - A version of 3.8.2

## Installation
`QCheck` can be installed on your system using several ways as follows:

### 1. Downloading from executable files
QCheck executable files are available for macOS and Linux at the following link.
```
https://github.com/canhminhdo/qcheck/releases/tag/v1.0
```

### 2. Running QCheck from Docker

The `qcheck` program is available from the command line in the Docker container.

```shell
$ docker build --no-cache -t qcheck .
$ docker run --rm -it qcheck
```

### 3. Building from source
`QCheck` can be installed on your system after building it from the source.
```shell
$ git clone --recurse-submodules https://github.com/canhminhdo/qcheck
$ cd qcheck
$ cmake -S . -B build -D CMAKE_BUILD_TYPE=Release # configure the project to be built with `Release` mode
$ cmake --build build --config Release -j 8 # building the project with `Release` mode and 8 workers in parallel
$ ctest --test-dir build # testing the project with some test cases
$ cmake --install build --config Release # install the project with `Release` mode in the `install` folder
$ cpack -G "ZIP" --config build/CPackConfig.cmake -B package # package the project to be distributed in the `package` directory
```

## Probabilistic Model Checkers
Make sure to install PRISM and Storm model checkers and set the following environment variables
```shell
export PRISM_PATH=<path-to-the-folder-where-PRISM-is-available>
export STORM_PATH=<path-to-the-folder-where-Storm-is-available>
```

## Getting Started
We can use QCheck to verify the correctness of Quantum Teleportation easily as follows.

- Go to the `examples/teleport` folder
```
$ cd examples/teleport
```

- Check the `teleport.qw` program file whose content is as follows
```
prog TELEPORT is
var q0, q1, q2 : qubit;
init
    q0 := random;
    q1 := |0>;
    q2 := |0>;
prop
    target := P(q2, init[q0]);
begin
    q1 := H[q1];
    q1, q2 := CX[q1, q2];
    q0, q1 := CX[q0, q1];
    q0 := H[q0];
    if M[q1] = 1 then q2 := X[q2]; else skip; fi;
    if M[q0] = 1 then q2 := Z[q2]; else skip; fi;
end

pcheck in TELEPORT with 'P=? [F "target"]' --backend=PRISM .
```
    
By default, `QCheck` uses PRISM model checker, but we can change it using `--backend=<name>` where `<name>` is either PRISM or Storm.


- Feed the `teleport.qw` program file into `QCheck` to verify its correctness with PRISM model checker.
```shell
qcheck teleport.qw
```

## Acknowledgements
This research was partially supported by the JAIST Research Grant for Fundamental Research, by JSPS KAKENHI Grant Numbers JP23K28060, JP23K19959, JP24K20757, and JP24KK0185.