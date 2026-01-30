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

# building qrat
RUN cd /app \
    && git clone --recurse-submodules https://github.com/canhminhdo/qrat qrat

RUN cd /app/qrat \
    && mkdir build \
    && cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_COMPILER=c++ \
    && cmake --build build --config Release -j 8 \
    && cmake --install build --config Release \
    && cpack -G "ZIP" --config build/CPackConfig.cmake -B package \

RUN cd /app/qrat \
    && mv install /qrat-2.0 \
    && mv package/qrat-2.0-Linux.zip /qrat-2.0 \
    && mv artifact/* /app

RUN rm -rf /app/qrat

# set environment variables
ENV PATH=/qrat-2.0:$PATH

WORKDIR /app