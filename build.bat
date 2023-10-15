@ECHO OFF
CLS
set /p plugin_version=<VERSION
cmake -Bcmake-build -DCMAKE_PREFIX_PATH=../JUCE/install -DPLUGIN_VERSION:STRING="%plugin_version%" -DASIO_SDK_LOCATION:STRING=".\asiosdk"
cmake --build cmake-build --config Release 

