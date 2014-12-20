pushd snes-side
tasm spc.asm SPC.OBJ
wla playsample
popd
g++ export_spc.c -o export_spc
./export_spc -s blip.brr -a 0x200 -p snes-side/SPC.OBJ -c 0x1000 -o derp.spc
pushd PC-side
make && ./play ../derp.spc
popd