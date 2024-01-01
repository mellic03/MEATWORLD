#!/bin/bash

# Delete old files
# rm -R ./output/assets

# Build project
mkdir -p ./build
cd ./build
cmake -G Ninja ..
ninja -j 12
cd ..



mkdir -p output/assets/
# cp ./build/game output/game
cp -R IDKGame/src/shaders output/.

cp -R IDKGame/assets/audio      output/assets/.
cp -R IDKGame/assets/cubemaps   output/assets/.
cp -R IDKGame/assets/heightmaps output/assets/.
cp -R IDKGame/assets/fonts      output/assets/.
cp -R IDKGame/assets/icons      output/assets/.
cp -R IDKGame/assets/models     output/assets/.
cp -R IDKGame/assets/textures   output/assets/.

