#!/bin/bash
# 64-bit Windows cross build script

version_major="$(grep "APP_VER_MAJOR " pc/shared/Organization.h | cut -f3 -d' ')"
version_minor="$(grep "APP_VER_MINOR " pc/shared/Organization.h | cut -f3 -d' ')"
version_micro="$(grep "APP_VER_MICRO " pc/shared/Organization.h | cut -f3 -d' ')"

version="${version_major}.${version_minor}.${version_micro}"

echo "v${version}"

make clean
prefix=/usr/x86_64-w64-mingw32/usr CROSS_COMPILE=x86_64-w64-mingw32- make clean
prefix=/usr/x86_64-w64-mingw32/usr CROSS_COMPILE=x86_64-w64-mingw32- make
cp /usr/x86_64-w64-mingw32/usr/{bin/SDL2.dll,lib/libboost_filesystem.dll} \
  /usr/lib/gcc/x86_64-w64-mingw32/9.2.0/{libgcc_s_seh-1.dll,libstdc++-6.dll} pc/bin
pushd pc
cp -r bin SNES\ Tracker\ v${version}-Win64
cd SNES\ Tracker\ v${version}-Win64
rm std.exe st.exe
popd

echo "v${version}"
echo "Remember to put in a README File bro!"