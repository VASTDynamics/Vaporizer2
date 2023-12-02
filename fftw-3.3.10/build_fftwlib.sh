#!/usr/bin/env bash

sudo rm -rf build
mkdir -p build

./configure --prefix=/Users/setup/Library/Developer/VST/Vaporizer2/fftw-3.3.10/build/x64 --exec-prefix=/Users/setup/Library/Developer/VST/Vaporizer2/fftw-3.3.10/build/x64 --enable-float --enable-sse --enable-avx --enable-sse2 -disable-fortran CFLAGS="-arch x86_64 -mmacosx-version-min=10.13" CPPFLAGS="-arch x86_64 -mmacosx-version-min=10.13" LDFLAGS="-mmacosx-version-min=10.13"
make
sudo make install

./configure --host=arm --disable-dependency-tracking --prefix=/Users/setup/Library/Developer/VST/Vaporizer2/fftw-3.3.10/build/x64SSE --exec-prefix=/Users/setup/Library/Developer/VST/Vaporizer2/fftw-3.3.10/build/x64SSE --enable-float --enable-neon -disable-fortran CFLAGS="-arch arm64 -mmacosx-version-min=10.13" CPPFLAGS="-arch arm64 -mmacosx-version-min=10.13" CXXFLAGS="-arch arm64 -mmacosx-version-min=10.13" LDFLAGS="-arch arm64 -mmacosx-version-min=10.13"
make
sudo make install

lipo -create libfftw3f_x64.a libfftw3f_arm64.a -output libfftw3f_universal.a

#sudo rm -rf cmake-build
#
#cmake -B"cmake-build/x64" -DCMAKE_BUILD_TYPE=Release -G Xcode -DCMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_SHARED_LIBS=0 -DENABLE_FLOAT=1 -DENABLE_LONG_DOUBLE=0 -DENABLE_SSE=1 -DENABLE_SSE2=0 -DENABLE_AVX=0 -DENABLE_AVX2=0 -DDISABLE_FORTRAN=1
#cmake --build "cmake-build/x64" --config Release
#
#cmake -B"cmake-build/x64" -DCMAKE_BUILD_TYPE=Release -G Xcode -DCMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_SHARED_LIBS=0 -DENABLE_FLOAT=0 -DENABLE_LONG_DOUBLE=0 -DENABLE_SSE=1 -DENABLE_SSE2=0 -DENABLE_AVX=0 -DENABLE_AVX2=0 -DDISABLE_FORTRAN=1
#cmake --build "cmake-build/x64" --config Release
#
#cmake -B"cmake-build/x64SSE" -DCMAKE_BUILD_TYPE=Release -G Xcode -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_SHARED_LIBS=0 -DENABLE_FLOAT=1 -DENABLE_LONG_DOUBLE=0 -DENABLE_SSE=1 -DENABLE_SSE2=0 -DENABLE_AVX=0 -DENABLE_AVX2=0 -DDISABLE_FORTRAN=1
#cmake --build "cmake-build/x64SSE" --config Release
#
#cmake -B"cmake-build/x64SSE" -DCMAKE_BUILD_TYPE=Release -G Xcode -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_SHARED_LIBS=0 -DENABLE_FLOAT=0 -DENABLE_LONG_DOUBLE=0 -DENABLE_SSE=1 -DENABLE_SSE2=0 -DENABLE_AVX=0 -DENABLE_AVX2=0 -DDISABLE_FORTRAN=1
#cmake --build "cmake-build/x64SSE" --config Release

#mkdir -p lib
#cp -frv cmake-build/x64/Release/libfftw3f.a lib/libfftw3f_x64.a
#cp -frv cmake-build/x64/Release/libfftw3.a lib/libfftw3_x64.a
#cp -frv cmake-build/x64SSE/Release/libfftw3f.a lib/libfftw3f_universal.a
#cp -frv cmake-build/x64SSE/Release/libfftw3.a lib/libfftw3_universal.a

#./configure --prefix x64 --enable-float --enable-sse --enable-avx --enable-sse2 -disable-fortran CFLAGS="-arch x86_64 -mmacosx-version-min=10.13" CPPFLAGS="-arch x86_64 -mmacosx-version-min=10.13" LDFLAGS="-mmacosx-version-min=10.13"
#make
#sudo make install

#./configure --prefix x64SSE --enable-float --enable-sse --enable-neon -disable-fortran CFLAGS="-arch x86_64;arm64 -mmacosx-version-min=10.13" CPPFLAGS="-arch x86_64;arm64 -mmacosx-version-min=10.13" LDFLAGS="-mmacosx-version-min=10.13"
#make
#sudo make install
