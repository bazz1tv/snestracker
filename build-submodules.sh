#!/bin/bash
echo Initializing Git Submodules
echo ---------------------------
git submodule update --init --recursive

echo
echo Building custom wla-dx
echo ----------------------
pushd submodules/wla-dx
	mkdir -p build-wla && cd build-wla &&
	cmake .. && cmake --build . --config Debug || (echo "CRITICAL ERROR" && exit 1)
popd

echo
echo Building pcx2snes
echo -----------------
make -C submodules/pcx2snes || (echo "CRITICAL ERROR" && exit 1)
echo

echo Building libgme_m
echo -----------------
make -C submodules/libgme_m || (echo "CRITICAL ERROR" && exit 1)
echo
echo Installing libgme_m dll into snestracker bin directory
echo ------------------------------------------------------
prefix=$PWD/pc/bin make -C submodules/libgme_m/ install-lib-direct || (echo "CRITICAL ERROR" && exit 1)
