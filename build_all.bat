@ECHO OFF
CLS
set /p plugin_version=<VERSION
cmake -Bcmake-build -DCMAKE_PREFIX_PATH=../JUCE/install -DPLUGIN_VERSION:STRING="%plugin_version%" -DASIO_SDK_LOCATION:STRING=".\asiosdk" -DAAX_SDK_LOCATION:STRING=".\AAX_SDK\aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING=".\VST_SDK\VST2_SDK"
cmake --build cmake-build --config Release 

