#!/bin/bash
echo Initializing Git Submodules
echo ---------------------------
git submodule update --init --recursive

echo
echo Building custom wla-dx
echo ----------------------
pushd submodules/wla-dx
	mkdir build-wla && cd build-wla &&
	cmake .. && cmake --build . --config Debug
popd

echo
echo Building pcx2snes
echo -----------------
make -C submodules/pcx2snes
echo

echo Building libgme_m
echo -----------------
make -C submodules/libgme_m
