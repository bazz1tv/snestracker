# build snes stuffz
pushd snes-side
cd spc; tasm spc.asm SPC.OBJ; cd ..
wla playsample
popd

# build PC tools
pushd ../tools/export_spc
g++ export_spc.cpp -o ../bin/export_spc
popd
../tools/bin/export_spc -s samples/blip.brr -a 0x200 -p snes-side/spc/SPC.OBJ -c 0x1000 -o PC-side/out/derp.spc
pushd PC-side/gme
make && ./play ../out/derp.spc
popd