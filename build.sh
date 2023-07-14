#!/bin/bash


# Build project
mkdir ./build ./output
cd ./build
cmake -G Ninja ..
ninja
cd ..


# Move output to output folder
cp ./build/game output/game
cp -R IDKGame/assets ./output/.
cp -R ./IDKGameEngine/shaders ./output/.

