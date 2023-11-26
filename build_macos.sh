#!/usr/bin/env bash
echo Building VAST Dynamics Software Vaporizer2 plugin

if [[ $@ == "" ]] 
then
    echo No parameters have been provided. Building all targets.
    declare -a targets=("macOS64" "macOS64SSE")
    #declare -a targets=("macOS64" "macOS64SSE" "macOS32")  #32-bit plugins not packaged but can be build if needed
else 
    echo Parameters: "$@"
    declare -a targets=("$@")
fi

echo Cleaning
sudo bash ./VASTvaporizer/Scripts/delete_all_old_stuff.sh
sudo rm -rf cmake-build

for t in ${targets[@]}
do
    echo Building for "$t"

    case $t in
        
        "macOS64")
            cmake -B"cmake-build\x64" -DCMAKE_BUILD_TYPE=Release -G Xcode -DCMAKE_PREFIX_PATH=../JUCE/install -DCMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_AVX=ON -DBUILD_32=OFF -DAAX_SDK_LOCATION:STRING="../AAX_SDK/aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING="../VST_SDK/VST2_SDK" 
            cmake --build "cmake-build\x64" --config Release
            ;;
        
        "macOS64SSE") #this is the universal binary supporting silicon M2 on Neon
            cmake -B"cmake-build\x64SSE" -DCMAKE_BUILD_TYPE=Release -G Xcode -DCMAKE_PREFIX_PATH=../JUCE/install -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_AVX=OFF -DBUILD_32=OFF -DAAX_SDK_LOCATION:STRING="../AAX_SDK/aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING="../VST_SDK/VST2_SDK" 
            cmake --build "cmake-build\x64SSE" --config Release
            ;;

        "macOS32")
            cmake -B"cmake-build\x86" -DCMAKE_BUILD_TYPE=Release -G Xcode -DCMAKE_PREFIX_PATH=../JUCE/install -DCMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_AVX=ON -DBUILD_32=ON -DAAX_SDK_LOCATION:STRING="../AAX_SDK/aax-sdk-2-6-0" -DVST2_SDK_LOCATION:STRING="../VST_SDK/VST2_SDK" 
            cmake --build "cmake-build\x86" --config Release
            ;;

         *)
            echo -n "ERROR: Unknown target"   
            break;
            ;;
    esac
done

