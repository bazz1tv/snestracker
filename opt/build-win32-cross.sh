#!/bin/bash
# 64-bit Windows cross build script

version_major="$(grep "APP_VER_MAJOR " pc/shared/Organization.h | cut -f3 -d' ')"
version_minor="$(grep "APP_VER_MINOR " pc/shared/Organization.h | cut -f3 -d' ')"
version_micro="$(grep "APP_VER_MICRO " pc/shared/Organization.h | cut -f3 -d' ')"

version="${version_major}.${version_minor}.${version_micro}"

echo "v${version}"

make clean
prefix=/usr/i686-w64-mingw32/usr CROSS_COMPILE=i686-w64-mingw32- make clean
prefix=/usr/i686-w64-mingw32/usr CROSS_COMPILE=i686-w64-mingw32- make
cp /usr/i686-w64-mingw32/usr/{bin/SDL2.dll,lib/libboost_filesystem.dll} \
  /usr/lib/gcc/i686-w64-mingw32/9.3.0/{libgcc_s_sjlj-1.dll,libstdc++-6.dll} pc/bin
pushd pc
cp -r bin SNES\ Tracker\ v${version}-Win32
cd SNES\ Tracker\ v${version}-Win32
rm std.exe st.exe
popd

echo "v${version}"
echo "Remember to put in a README File bro!"