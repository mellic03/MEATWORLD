#!/bin/bash


# Build project
mkdir ./build ./output
cd ./build
cmake -G Ninja ..
ninja
cd ..

