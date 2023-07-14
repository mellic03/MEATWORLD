#!/bin/bash

# Delete old files
rm -R ./output/assets

# Build project
mkdir ./build ./output ./output/assets ./output/IDKGE ./output/IDKGE/IDKtools
cd ./build
cmake -G Ninja ..
ninja
cd ..


cp ./build/game output/game
cp -R IDKGame/assets ./output/.
cp -R ./IDKGameEngine/shaders ./output/.


rm -R ./output/assets/textures
mkdir ./output/assets/textures
cp ./build/texconvert ./output/IDKGE/IDKtools/texconvert
./output/IDKGE/IDKtools/texconvert ./IDKGame/assets/textures/ ./output/assets/textures/
