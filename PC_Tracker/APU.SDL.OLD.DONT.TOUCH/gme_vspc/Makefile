#unwritten rule .. app.h RULES ALL. if it changes, the whole fucking thing needs to be recompiled...


CC=g++
CPP=g++
debug=-g
gme_CPPFLAGS=$(debug) -c -I. -Igme -Igme/gme -Idemo `/usr/local/bin/sdl2-config --cflags`


debugger_CPPFLAGS=$(debug) -INativeFileDialog/include -Idebugger/memory -Idebugger/windows -O3\
-funroll-loops -Wall -Idebugger `/usr/local/bin/sdl2-config --cflags` -Wall

# for MIDI
DEFS     			=   -D__MACOSX_CORE__ -DRTMIDI_DEBUG
LIBRARY  			= -framework CoreMIDI -framework CoreFoundation -framework CoreAudio
MIDI_CPPFLAGS = -Irtmidi
#
LDFLAGS=$(debug) -I. `/usr/local/bin/sdl2-config --libs` -lm -framework AppKit $(LIBRARY)

CPPFLAGS = -std=c++11 $(MIDI_CPPFLAGS) $(gme_CPPFLAGS) $(debugger_CPPFLAGS) -MMD -MP -Wno-int-to-void-pointer-cast
OBJCFLAGS = $(gme_CPPFLAGS) $(debugger_CPPFLAGS) -MMD -MP
####
SOURCES=$(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
MSOURCES=$(wildcard */*.m)
###
OBJECTS=$(SOURCES:.cpp=.cpp.o) $(MSOURCES:.m=.m.o)
#OBJECTS += $(debugger_OBJECTS)
EXECUTABLE=std

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

#gme/gme/Spc_Cpu.o: gme/gme/Spc_Cpu.cpp gme/gme/Spc_Cpu.h debugger/report.h 
#	$(CPP) $(CFLAGS) -c $< -o $@
# Use the dependency files created by the -MMD option to gcc.


%.cpp.o: %.cpp
	$(CPP) $(DEFS) $(CPPFLAGS) -c $< -o $@

%.m.o: %.m
	$(CPP) $(OBJCFLAGS) -c $< -o $@

# Order matters.. do the ones with headers first
#%.o: %.c %.h
#	echo "FLE"
#	$(CC) $(CFLAGS) -c $< -o $@

#%.o: %.c
#	echo "FLF"
#	$(CC) $(CFLAGS) -c $< -o $@
	


clean:
	rm -f $(EXECUTABLE); #gme/gme/*.o gme/demo/*.o gme/player/*.o debugger/*.o
	find . -name "*.o" -o -name "*.d" | xargs rm -rf


# DO NOT DELETE THIS LINE -- make depend depends on it.
-include $(SOURCES:.cpp=.cpp.d) $(MSOURCES:.m=.m.d)
