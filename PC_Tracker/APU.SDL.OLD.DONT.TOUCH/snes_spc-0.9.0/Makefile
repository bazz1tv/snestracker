CC=g++
CFLAGS=-c -I. -Isnes_spc -Idemo -I..
LDFLAGS=-I.
SOURCES=snes_spc/SNES_SPC.cpp snes_spc/SPC_DSP.cpp snes_spc/spc.cpp snes_spc/SNES_SPC_misc.cpp snes_spc/SPC_Filter.cpp\
snes_spc/SNES_SPC_state.cpp snes_spc/dsp.cpp\
demo/demo_util.c demo/wave_writer.c demo/play_spc.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=play_spc

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f play_spc demo/*.o snes_spc/*.o
