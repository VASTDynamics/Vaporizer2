#!/usr/bin/env bash

sudo rm -rf cmake-build
cmake -B"cmake-build/x64" -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_SHARED_LIBS=0 -DENABLE_FLOAT=1 -DENABLE_SSE=0 -DENABLE_SSE2=0 -DENABLE_AVX=0 -DENABLE_AVX2=0
cmake --build "cmake-build/x64" --config Release

cmake -B"cmake-build/x64SSE" -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_SHARED_LIBS=0 -DENABLE_FLOAT=1 -DENABLE_SSE=0 -DENABLE_SSE2=0 -DENABLE_AVX=0 -DENABLE_AVX2=0
cmake --build "cmake-build/x64SSE" --config Release

mkdir -p lib
cp -frv cmake-build/x64/libfftw3f.a lib/libfftw3_x64.a
cp -frv cmake-build/x64SSE/libfftw3f.a lib/libfftw3_universal.a
