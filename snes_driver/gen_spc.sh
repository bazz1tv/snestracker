total=$((0x10200))
startsize=$((0x400 + 0x100))
dd if="$1" bs=1 count=$startsize of="$2"
cat apu/spc.bin >> "$2"
binsize=$(ls -l apu/spc.bin | awk '{print $5}')
dd if="$1" skip=$((startsize + binsize)) \
	 seek=$((startsize + binsize)) bs=1 count=$((total - startsize - binsize)) \
	 of="$2" oflag=append
