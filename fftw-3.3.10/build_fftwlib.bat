rmdir /S /Q cmake-build

rem Setting muli threadded lib with -DCMAKECXX_FLAGS="/MT" -DCMAKECXX_FLAGS_RELEASE="/MT" -DCMAKECXX_FLAGS_DEBUG="/MTd does not work here. Has to be adapted in CMakeLists.txt
cmake -B"cmake-build\x64" -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_VERSION=10.0.22000.0 -G "Visual Studio 16 2019" -T v142 -A x64 -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_SHARED_LIBS=0 -DENABLE_FLOAT=1 -DENABLE_SSE=1 -DENABLE_SSE2=1 -DENABLE_AVX=1 -DENABLE_AVX2=0
cmake --build "cmake-build\x64" --config Release 
mkdir lib
copy /b/v/y cmake-build\x64\Release\fftw3f.lib lib\