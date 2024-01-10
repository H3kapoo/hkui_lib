#!/bin/sh
cmake -B build -GNinja . # generate ninja data
cmake --build build      # link and compile
mv debug/hkui .      # move out of debug folder
# ./simpleUI               # run at root level