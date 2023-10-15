@ECHO OFF
cd vastVaporizer

cmake -Bbuild -DCMAKE_PREFIX_PATH=../JUCE/install
cmake --build build --config Release -DASIO_SDK_LOCATION="path\to\Vaporizer2\asiosdk" -DAAX_SDK_LOCATION="path\to\aaxsdk"

cd ..