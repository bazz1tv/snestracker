CC=g++
CFLAGS=-c -I. -Isnes_spc -Idemo -I..
LDFLAGS=-I.
SOURCES=snes_spc/SNES_SPC.cpp snes_spc/SPC_DSP.cpp snes_spc/spc.cpp snes_spc/SNES_SPC_misc.cpp snes_spc/SPC_Filter.cpp\
snes_spc/SNES_SPC_state.cpp snes_spc/dsp.cpp\
demo/demo_util.c demo/wave_writer.c 

SOURCES1 = demo/play_spc.c
SOURCES1 += $(SOURCES)
SOURCES2 = demo/save_state.c
SOURCES2 += $(SOURCES)
OBJECTS1=$(SOURCES1:.cpp=.o)
OBJECTS2=$(SOURCES2:.cpp=.o)
EXECUTABLE1=play_spc
EXECUTABLE2=save_state

all: $(SOURCES1) $(SOURCES2) $(EXECUTABLE1) $(EXECUTABLE2)
	
$(EXECUTABLE1): $(OBJECTS1) 
	$(CC) -g $(LDFLAGS) $(OBJECTS1) -o $@

$(EXECUTABLE2): $(OBJECTS2) 
	$(CC) -g $(LDFLAGS) $(OBJECTS2) -o $@

.cpp.o:
	$(CC) -g $(CFLAGS) $< -o $@

clean:
	rm -f play_spc demo/*.o snes_spc/*.o
