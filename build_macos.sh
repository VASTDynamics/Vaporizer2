#!/usr/bin/env bash
clear
set /p plugin_version=<VERSION
cmake -Bcmake-build -DCMAKE_PREFIX_PATH=../JUCE/install -DPLUGIN_VERSION:STRING="%plugin_version%" 
cmake --build cmake-build --config Release 