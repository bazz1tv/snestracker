#!/bin/bash
# These commands are run from SDL2 source code top-directory. It will build a fat library supporting back to osx 10.6

mkdir build ; cd build ; CC=$PWD/../build-scripts/gcc-fat.sh ../configure ; make
