@ECHO OFF
cd vastVaporizer
cmake -Bbuild -DCMAKE_PREFIX_PATH=../JUCE/install
cmake --build build --config Release 
cd ..