# We need a makefile that first builds the apu driver, does the
# intermediary steps of importing the SPC RAM locations header file, then builds the
# tracker.
all: snes_driver/spc.sym pc/std/tracker/apuram.h pc/std/bin/snes_tracker

snes_driver/spc.sym: FORCE Makefile
	make -C snes_driver

pc/std/tracker/apuram.h: snes_driver/spc.sym Makefile snes_driver/conv_public_syms_to_C.sh
	cd snes_driver && ./conv_public_syms_to_C.sh > ../pc/std/tracker/apuram.h


pc/std/bin/snes_tracker: FORCE pc/std/tracker/apuram.h
	make -C pc/std


FORCE: ;
