@ECHO OFF
CLS

echo Building VAST Dynamics Software Vaporizer2 plugin

if "%~1"=="" (
    echo No parameters have been provided. Building all targets.
    set targets=x64 x64SSE Win32 Win32SSE
) else (
    echo Parameters: %*
    set targets=%*
)

echo Cleaning 
rmdir /S /Q cmake-build

for %%t in (%targets%) do (
   echo Building for %%t
   if "%%t"=="Win32" (
      cmake -B"cmake-build\Win32" -G "Visual Studio 16 2019" -A Win32 -DCMAKE_PREFIX_PATH=../JUCE/install -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK" -DBUILD_AVX=ON -DBUILD_32=ON
      cmake --build "cmake-build\Win32" --config Release 
   ) else if "%%t"=="Win32SSE" (
      cmake -B"cmake-build\Win32SSE2" -G "Visual Studio 16 2019" -A Win32 -DCMAKE_PREFIX_PATH=../JUCE/install -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK" -DBUILD_AVX=OFF -DBUILD_32=ON
      cmake --build "cmake-build\Win32SSE2" --config Release 
   ) else if "%%t"=="x64" (
      cmake -B"cmake-build\x64" -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH=../JUCE/install -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK" -DBUILD_AVX=ON -DBUILD_32=OFF
      cmake --build "cmake-build\x64" --config Release 
   ) else if "%%t"=="x64SSE" (
      cmake -B"cmake-build\x64SSE2" -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH=../JUCE/install -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK" -DBUILD_AVX=OFF -DBUILD_32=OFF
      cmake --build "cmake-build\x64SSE2" --config Release 
   ) else (
      echo Unknown target "%%t"
      goto error
   )
)

:success
echo Succesfully completed
exit /b 0
:error
echo Terminating with error
exit /b 1