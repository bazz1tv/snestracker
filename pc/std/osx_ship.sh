#!/bin/bash
# OSX app library bundling script
#    This script locates and copies non-system libraries into the .app bundle
#    It modifies the bundled main-program and copied libraries so that they
#    will load from their new location.
#
# Copyright 2020 Michael Bazzinotti (Bazz)
# https://bazz1.com
#
# Input args provided by Makefile:
#             $1 : app bundle path
#             $2 : executable name
#             $3 : lib path



# Get the location of project libraries that are not system libs
# The assumption is that all non-system libs were installed into a directory having
# the string "local" in it. eg. /usr/local /opt/local.
# This is currently the case for the following libs being used:
# SDL2, Boost (via Homebrew), libgme_e

# construct the app bundle executable path
EXE=$1/Contents/MacOS/$2

# otool -L will list all lib deps for the executable, in the following format:
#
# /usr/local/lib/libboost_system-mt.dylib (compatibility version 0.0.0, current version 0.0.0)
#
# So, use awk to work only with the path, and not the 2nd column version data
# Finally, only work with paths that have "local" string.
paths=$(otool -L ${EXE} | awk '{print $1}' | grep "local")

# Now, let's re-instruct the exe to use libs that are about to be placed into
# the app bundle
for path in $paths; do
	# copy lib to app bundle
	cp ${path} $3
	# modify the exe to load the lib from a runtime path
	install_name_tool -change ${path} @rpath/${path##*/} ${EXE}
done

# add the library rpath to the exe
install_name_tool -add_rpath @executable_path/../lib ${EXE}
# reminder: @executable_path equates to where the exe is, eg. .app/Contents/MacOS