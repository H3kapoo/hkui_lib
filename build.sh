#!/bin/sh
cmake -B build -GNinja .            # generate ninja data
cmake --build build                 # link and compile
rm -rf ./lib
mkdir ./lib 
mv build/libhkui.a ./lib/libhkui.a  # move out of debug folder