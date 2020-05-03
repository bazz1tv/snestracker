#!/bin/bash
# These commands serve to remind how I installed Boost manually to support
# older versions of OSX.
# Tested with Boost 1.73
# This is not an automated script!
./bootstrap.sh
patch -p1 < darwin.jam.patch
sudo ./b2 --prefix=/opt/local --with-system --with-filesystem --toolset=darwin \
macosx-version-min=10.6 variant=debug link=shared threading=multi install

# Confirm that the library path is included in its id. The SNES Tracker build system
# looks for a "local" in the 3rd party library names via `otool -L`, and will change
# it to use @rpath when it bundles it all into an .app.
cd /opt/local/lib
sudo install_name_tool -id $PWD/libboost_filesystem.dylib libboost_filesystem.dylib
sudo install_name_tool -id $PWD/libboost_system.dylib libboost_system.dylib
