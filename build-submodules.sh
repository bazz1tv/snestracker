#!/bin/bash
echo Initializing Git Submodules
echo ---------------------------
git submodule update --init --recursive

echo
echo Building custom wla-dx
echo ----------------------
pushd submodules/wla-dx
	mkdir -p build-wla && cd build-wla
	# Conditional settings for compiling directly on Windows with MSYS
	cmakeFlags=
	if [[ "$OSTYPE" == "msys" ]]; then
		cmakeFlags="-G 'MSYS Makefiles'"
	fi
	cmake .. ${cmakeFlag} && cmake --build . --config Debug || (echo "CRITICAL ERROR" && exit 1)
popd

echo
echo Building pcx2snes
echo -----------------
make -C submodules/pcx2snes || (echo "CRITICAL ERROR" && exit 1)
echo
