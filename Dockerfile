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
    cmake

RUN mkdir -p /app

# building qrat
RUN cd /app \
    && git clone --recurse-submodules https://github.com/canhminhdo/qrat qrat

RUN cd /app/qrat \
    && mkdir build \
    && cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_COMPILER=c++ \
    && cmake --build build --config Release -j 8 \
    && cmake --install build --config Release \
    && cpack -G "ZIP" --config build/CPackConfig.cmake -B package

RUN cd /app/qrat \
    && mv install /qrat-2.0 \
    && mv package/qrat-2.0-Linux.zip /qrat-2.0 \
    && mv artifact/* /app

RUN rm -rf /app/qrat

# set environment variables
ENV PATH=/qrat-2.0:$PATH

WORKDIR /app