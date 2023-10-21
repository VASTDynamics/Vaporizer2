@ECHO OFF
CLS
echo Building VAST Dynamics Software Vaporizer2 plugin
cmake -B"cmake-build\x64" -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH=../JUCE/install -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK" -DBUILD_AVX=ON -DBUILD_32=OFF
cmake --build "cmake-build\x64" --config Release 
