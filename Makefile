SPCDRIVER_FILENAME := tracker.spc
APURAM_HEADER := "pc/std/tracker/apuram.h"
# We need a makefile that first builds the apu driver, does the
# intermediary steps of importing the SPC RAM locations header file, then builds the
# tracker.
all: pc/std/bin/tracker.spc snes_driver/spc.sym pc/std/tracker/apuram.h pc/std/bin/snes_tracker

# little dirty. Technically we depend on spc.bin but since the sym file
# has the FORCE rule, we'll use that to make sure it is re-assembled if
# necessary
pc/std/bin/$(SPCDRIVER_FILENAME): snes_driver/spc.sym snes_driver/gen_spc.sh Makefile
	cd snes_driver && ./gen_spc.sh ../pc/std/bin/$(SPCDRIVER_FILENAME)

snes_driver/spc.sym: FORCE Makefile
	make -C snes_driver

pc/std/tracker/apuram.h: snes_driver/spc.sym Makefile \
		snes_driver/conv_public_syms_to_C.sh pc/std/bin/$(SPCDRIVER_FILENAME)
	cd snes_driver && ./conv_public_syms_to_C.sh > ../$(APURAM_HEADER)
	echo "#define SPCDRIVER_FILENAME \"$(SPCDRIVER_FILENAME)\"" >> $(APURAM_HEADER)

pc/std/bin/snes_tracker: FORCE pc/std/tracker/apuram.h
	make -C pc/std


FORCE: ;

clean:
	make -C pc/std clean
	make -C snes_driver clean
