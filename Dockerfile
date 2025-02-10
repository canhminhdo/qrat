FROM ubuntu:22.04

RUN apt-get -y update && apt-get install -y \
    vim \
    wget \
    git \
    curl \
    zip \
    unzip \
    g++ \
    cmake \
    bison \
    flex

RUN mkdir -p /app

# building qrat
RUN cd /app \
    && git clone --recurse-submodules https://github.com/canhminhdo/MQT-QRAT.git qrat

RUN cd /app/qrat \
    && mkdir build \
    && cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_COMPILER=c++ \
    && cmake --build build --config Release -j 8 \
    && ctest --test-dir build \
    && cmake --install build --config Release \
    && cpack -G "ZIP" --config build/CPackConfig.cmake -B package \

RUN cp /app/qrat \
    && mv install /qrat-1.0 \
    && mv package/qrat-1.0-Linux.zip /qrat-1.0 \
    && mv /qrat-1.0/teleport/*.qw /qrat-1.0/loop/*.qw /qrat-1.0/grover/*.qw /app

## clean up
RUN rm -rf /app/qrat

## set environment variables
ENV PATH=/qrat-1.0:$PATH

WORKDIR /app