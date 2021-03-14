#!/bin/bash
# 64-bit Windows cross build script
# Calls the 64-bit cross script but with specific libs from my win10 machine
libs="$(echo -n /mingw64/bin/{libstdc++-6.dll,libgcc_s_seh-1.dll} \
	/mingw64/x86_64-w64-mingw32/{lib/libboost_*-*.dll,bin/SDL2.dll} \
	/mingw64/bin/libwinpthread-1.dll)" opt/build-win64-cross.sh
