#!/bin/bash
# this script exports the spc.bin file into an SPC file for loading into
# SNES Tracker. Takes 1 argument:
# 1: the path of the output spc file
# This script is "internal use" only so it's not very helpful in general
# use

if [[ $1 = "" ]]; then
	echo "Provide output file path please"
	exit 1
fi

total=$((0x10200))
startsize=$((0x400 + 0x100))
dd if="template.spc" bs=1 count=$startsize of="$1"
cat spc.bin >> "$1"
binsize=$(ls -l spc.bin | awk '{print $5}')
dd if="template.spc" skip=$((startsize + binsize)) \
	 seek=$((startsize + binsize)) bs=1 count=$((total - startsize - binsize)) \
	 of="$1" oflag=append
