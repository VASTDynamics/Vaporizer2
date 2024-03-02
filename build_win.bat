@ECHO OFF
CLS

@echo Building VAST Dynamics Software Vaporizer2 plugin
@echo Start time: 
@echo %time%

if "%~1"=="" (
    @echo No parameters have been provided. Building all targets.
    rem set targets=x64 x64SSE2 Win32 Win32SSE2
    set targets=x64 x64SSE2
) else (
    @echo Parameters: %*
    set targets=%*
)

@echo Cleaning 
rmdir /S /Q cmake-build

for %%t in (%targets%) do (
   @echo Building for %%t
   if "%%t"=="Win32" (
      cmake -B"cmake-build\Win32" -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_VERSION=10.0.22000.0 -G "Visual Studio 16 2019" -T v142 -A Win32 -DCMAKE_PREFIX_PATH=../JUCE/install -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK" -DBUILD_AVX=ON -DBUILD_32=ON -DBUILD_SHARED_LIBS:BOOL=OFF -DBUILD_STATIC_LIBS:BOOL=ON
      cmake --build "cmake-build\Win32" --config Release 
   ) else if "%%t"=="Win32SSE2" (
      cmake -B"cmake-build\Win32SSE2" -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_VERSION=10.0.22000.0 -G "Visual Studio 16 2019" -T v142 -A Win32 -DCMAKE_PREFIX_PATH=../JUCE/install -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK" -DBUILD_AVX=OFF -DBUILD_32=ON -DBUILD_SHARED_LIBS:BOOL=OFF -DBUILD_STATIC_LIBS:BOOL=ON
      cmake --build "cmake-build\Win32SSE2" --config Release 
   ) else if "%%t"=="x64" (
      cmake -B"cmake-build\x64" -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_VERSION=10.0.22000.0 -G "Visual Studio 16 2019" -T v142 -A x64 -DCMAKE_PREFIX_PATH=../JUCE/install -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK" -DBUILD_AVX=ON -DBUILD_32=OFF -DBUILD_SHARED_LIBS:BOOL=OFF -DBUILD_STATIC_LIBS:BOOL=ON
      cmake --build "cmake-build\x64" --config Release 
   ) else if "%%t"=="x64SSE2" (
      cmake -B"cmake-build\x64SSE2" -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_VERSION=10.0.22000.0 -G "Visual Studio 16 2019" -T v142 -A x64 -DCMAKE_PREFIX_PATH=../JUCE/install -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK" -DBUILD_AVX=OFF -DBUILD_32=OFF -DBUILD_SHARED_LIBS:BOOL=OFF -DBUILD_STATIC_LIBS:BOOL=ON
      cmake --build "cmake-build\x64SSE2" --config Release 
   ) else (
      @echo Unknown target "%%t"
      goto error
   )
)

:success
@echo Succesfully completed
@echo End time: 
@echo %time%
exit /b 0
:error
@echo Terminating with error
@echo End time: 
@echo %time%
exit /b 1