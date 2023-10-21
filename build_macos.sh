#!/usr/bin/env bash
clear
sudo bash ./VASTvaporizer/Scripts/delete_all_old_stuff.sh
read -p "Press enter to continue"
sudo rm -rf cmake-build
read -p "Press enter to continue"
set /p plugin_version=<VERSION
cmake -Bcmake-build -DCMAKE_PREFIX_PATH=../JUCE/install -DPLUGIN_VERSION:STRING="%plugin_version%" -DCMAKE_OSX_ARCHITECTURES=arm64;x86_64
cmake --build cmake-build --config Release 