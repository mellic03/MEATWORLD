#!/bin/bash

# Delete old files
# rm -R ./output/assets

# Build project
mkdir -p ./build
cd ./build
cmake -G Ninja ..
ninja -j 6
cd ..



mkdir output
cp ./build/game output/game
cp -R IDKGame/src/shaders output/.
cp -R IDKGame/assets ./output/.

cp -R external/IDKGE ./output/.
# cp -R ./IDKGameEngine/shaders ./output/IDKGE/.


# mkdir ./output/IDKGE/IDKtools


# # mapIBL
# mkdir ./output/IDKGE/IDKtools/MapIBL
# mkdir ./output/IDKGE/IDKtools/MapIBL/input
# mkdir ./output/IDKGE/IDKtools/MapIBL/output

# cp -R ./IDKGameEngine/shaders/IDKtools/DiffuseIBL/. ./output/IDKGE/IDKtools/MapIBL/.
# cp -R ./IDKGameEngine/shaders/IDKtools/SpecularIBL/. ./output/IDKGE/IDKtools/MapIBL/.

# cp ./IDKGameEngine/src/IDKtools/mapIBL.sh ./output/IDKGE/IDKtools/MapIBL/.

# cp ./build/diffuseIBL ./output/IDKGE/IDKtools/MapIBL/diffuseIBL
# cp ./build/specularIBL ./output/IDKGE/IDKtools/MapIBL/specularIBL
