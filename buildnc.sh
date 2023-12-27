#!/bin/bash

# Build project
mkdir -p ./build
cd ./build
cmake -G Ninja ..
ninja -j 12
cd ..



mkdir output
cp ./build/game output/game
