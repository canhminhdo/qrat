FROM ubuntu:22.04

RUN apt-get -y update && apt-get install -y \
    vim \
    wget \
    git \
    curl \
    zip \
    unzip \
    g++ \
    bison \
    flex \
    default-jre \
    build-essential libssl-dev libboost-all-dev libcln-dev libgmp-dev libginac-dev automake libglpk-dev libhwloc-dev libz3-dev libxerces-c-dev libeigen3-dev

RUN mkdir -p /app

# building cmake manually
RUN cd /app \
    && wget https://github.com/Kitware/CMake/releases/download/v3.31.10/cmake-3.31.10.tar.gz \
    && tar -xzvf cmake-3.31.10.tar.gz \
    && rm cmake-3.31.10.tar.gz \
    && cd cmake-3.31.10 \
    && ./configure --prefix=/usr \
    && make \
    && make install

RUN rm -rf /app/cmake-3.31.10

# building qcheck
RUN cd /app \
    && git clone --recurse-submodules https://github.com/canhminhdo/qcheck qcheck

RUN cd /app/qcheck \
    && mkdir build \
    && cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_COMPILER=c++ \
    && cmake --build build --config Release -j 8 \
    && cmake --install build --config Release \
    && cpack -G "ZIP" --config build/CPackConfig.cmake -B package \

RUN cd /app/qcheck \
    && mv install /qcheck-1.0 \
    && mv package/qcheck-1.0-Linux.zip /qcheck-1.0 \
    && mv artifact/*.qw /app

RUN rm -rf /app/qcheck

# PRISM model checker
RUN cd /app \
    && wget https://www.prismmodelchecker.org/dl/prism-4.8.1-linux64-x86.tar.gz \
    && tar -xzf prism-4.8.1-linux64-x86.tar.gz \
    && rm prism-4.8.1-linux64-x86.tar.gz \
    && mv prism-4.8.1-linux64-x86 prism-4.8.1 \
    && cd prism-4.8.1 \
    && ./install.sh

# Storm model checker
RUN cd /app \
    && wget https://github.com/moves-rwth/storm/archive/stable.zip \
    && unzip stable.zip \
    && rm stable.zip \
    && mv storm-stable storm-1.11.1

RUN cd /app/storm-1.11.1 \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make binaries
    # && make check

# set environment variables
ENV PATH=/qcheck-1.0:/app/prism-4.8.1/bin:/app/storm-1.11.1/build/bin:$PATH
ENV PRISM_PATH=/app/prism-4.8.1/bin
ENV STORM_PATH=/app/storm-1.11.1/build/bin

WORKDIR /app