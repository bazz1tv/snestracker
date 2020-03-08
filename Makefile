SPCDRIVER_FILENAME := tracker.spc
SPCDRIVER_RELPATH := pc/std/bin/$(SPCDRIVER_FILENAME)
APURAM_HEADER := pc/std/tracker/apuram.h
# trailing slash so that unsetting this variable makes no problem
# using binaries that are in $PATH
WLAPREFIX := ~/Code/git-repos/wla-dx/build/binaries/
WLASPC700 := $(WLAPREFIX)wla-spc700
# We need a makefile that first builds the apu driver, does the
# intermediary steps of importing the SPC RAM locations header file, then builds the
# tracker.
all: $(SPCDRIVER_RELPATH) snes_driver/spc.sym \
pc/std/tracker/apuram.h pc/std/bin/snes_tracker snes_driver/Makefile

# little dirty. Technically we depend on spc.bin but since the sym file
# has the FORCE rule, we'll use that to make sure it is re-assembled if
# necessary
$(SPCDRIVER_RELPATH): snes_driver/spc.sym snes_driver/gen_spc.sh Makefile
	cd snes_driver && ./gen_spc.sh ../$(SPCDRIVER_RELPATH)

snes_driver/spc.sym: FORCE Makefile snes_driver/Makefile
	make -C snes_driver

snes_driver/Makefile: snes_driver/genmake Makefile
	cd snes_driver ; wlaprefix=$(WLAPREFIX) ./genmake > Makefile

$(APURAM_HEADER): snes_driver/spc.sym Makefile \
		snes_driver/conv_public_syms_to_C.sh $(SPCDRIVER_RELPATH)
	cd snes_driver && ./conv_public_syms_to_C.sh > ../$(APURAM_HEADER)
	echo "#define SPCDRIVER_FILENAME \"$(SPCDRIVER_FILENAME)\"" >> $(APURAM_HEADER)
	printf "#define SPCDRIVER_CODESIZE 0x%x\n" $$(ls -l snes_driver/spc.bin | awk "{print \$$5}") >> $(APURAM_HEADER)
	$(WLASPC700) -t snes_driver/apu/memorymap_defs.i 2>/dev/null >> $(APURAM_HEADER) ; true
	echo >> $(APURAM_HEADER)
	$(WLASPC700) -t snes_driver/apu/playbackengine.i 2>/dev/null >> $(APURAM_HEADER) ; true
	echo >> $(APURAM_HEADER)
	$(WLASPC700) -t snes_driver/apu/commands.i 2>/dev/null >> $(APURAM_HEADER) ; true
	# the 'true' at the end is because the compilation will actually fail
	# since this is not a fully defined file for compilation.. but it's
	# enough to print out the definition
	# Now that I've figured out how to get the local definition into the
	# symbols file (export it), here's another way it can be done:
	# printf "#define SPCDRIVER_CODESTART 0x%x\n" $((0x$(grep -m1 "SPC_CODE_START" spc.sym | cut -f1 -d' ')))
	# but I guess I'll keep it non-global..?

pc/std/bin/snes_tracker: FORCE pc/std/tracker/apuram.h
	make -C pc/std

FORCE: ;

clean:
	# remove generated files
	rm $(APURAM_HEADER)
	rm $(SPCDRIVER_RELPATH)
	make -C pc/std clean
	make -C snes_driver clean
