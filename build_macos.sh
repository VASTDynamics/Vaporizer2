#!/usr/bin/env bash
clear
declare -a targets=("macOS64" "macOS64SSE2" "macOS32")
echo Building VAST Dynamics Software Vaporizer2 plugin
echo Cleaning
read -p "Press enter to continue"
sudo bash ./VASTvaporizer/Scripts/delete_all_old_stuff.sh
sudo rm -rf cmake-build
read -p "Press enter to continue"

for t in ${targets[@]}
do
    echo Building for "$t"
    if [ $t == "macOS64" ]
    then
        cmake -B"cmake-build\x64" -DCMAKE_PREFIX_PATH=../JUCE/install -DCMAKE_OSX_ARCHITECTURES="x86_64" -DBUILD_AVX=ON -DBUILD_32=OFF -DAAX_SDK_LOCATION:STRING="../../AAX_SDK/aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING="../../VST_SDK/VST2_SDK" 
        cmake --build "cmake-build\x64" --config Release
    fi
    
    if [ $t == "macOS64SSE2" ] #this is the universal binary supporting silicon M2 on Neon
    then
        cmake -B"cmake-build\x64SSE2" -DCMAKE_PREFIX_PATH=../JUCE/install -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DBUILD_AVX=OFF -DBUILD_32=OFF -DAAX_SDK_LOCATION:STRING="../../AAX_SDK/aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING="../../VST_SDK/VST2_SDK" 
        cmake --build "cmake-build\x64SSE2" --config Release
    fi
    
    if [ $t == "macOS32" ]
    then
        cmake -B"cmake-build\x86" -DCMAKE_PREFIX_PATH=../JUCE/install -DCMAKE_OSX_ARCHITECTURES="x86" -DBUILD_AVX=ON -DBUILD_32=ON -DAAX_SDK_LOCATION:STRING="../../AAX_SDK/aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING="../../VST_SDK/VST2_SDK" 
        cmake --build "cmake-build\x86" --config Release
    fi
done