#!/bin/bash
../configure  "--target-platform-macosx=1"  "--prefix=/opt/local"
osxflags='-arch x86_64 -mmacosx-version-min=10.6 -DMAC_OS_X_VERSION_MIN_REQUIRED=1060 -I/usr/local/include'
sed -i .bak -e "s#CXXFLAGS=#CXXFLAGS=${osxflags}#" -e "s#LDFLAGS=#LDFLAGS=${osxflags} -dynamiclib#" Makefile

