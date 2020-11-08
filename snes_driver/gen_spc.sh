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

codestart=$(($(${wlaprefix}wla-spc700 -t apu/memorymap_defs.i 2>/dev/null | cut -f3 -d' ' | tr -d '\r')))


total=$((0x10200))
start_to_pc=$((0x25))
startsize=$((codestart + 0x100 - start_to_pc - 2))
dd if="template.spc" bs=1 count=${start_to_pc} of="$1"
printf $(printf "%04x" ${codestart} | sed 's/\(..\)\(..\)/\\x\2\\x\1/') >> "$1"
dd if="template.spc" bs=1 skip=$((start_to_pc+2)) seek=$((start_to_pc+2)) \
  count=${startsize} of="$1"
cat spc.bin >> "$1"
binsize=$(ls -l spc.bin | awk '{print $5}')

skip=$((startsize + binsize + start_to_pc + 2))
dd if="template.spc" skip=$skip \
	 seek=$skip bs=1 count=$((total - skip)) \
	 of="$1"
