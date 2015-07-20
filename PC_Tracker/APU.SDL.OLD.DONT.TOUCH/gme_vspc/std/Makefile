#unwritten rule .. app.h RULES ALL. if it changes, the whole fucking thing needs to be recompiled...
# tells make to use this shell: 
SHELL := /bin/bash
# the only real reason I'm using that ^ is for pushd popd commands.. below..

# This is where source that both tracker and STD will use. NOTE THE inclusion of the forward slash ;)
SHARED_DIR = ../shared/
#

CC=g++
CPP=g++
OBJCC=g++
debug = -g
optimize = -O3

gme_CPPFLAGS=$(debug) -c -I. -I$(SHARED_DIR) -I$(SHARED_DIR)gme\
-I$(SHARED_DIR)gme/gme -Idemo `/usr/local/bin/sdl2-config --cflags`

debugger_CPPFLAGS=$(debug) -I$(SHARED_DIR) -I$(SHARED_DIR)NativeFileDialog/include\
-Idebugger/memory -Idebugger/content_areas -Idebugger/windows/Options_Window\
-funroll-loops -Wall -Idebugger `/usr/local/bin/sdl2-config --cflags` -Wall

# for MIDI
MIDI_DEFS     =   -D__MACOSX_CORE__ -DRTMIDI_DEBUG
MIDI_LIBRARY  = -framework CoreMIDI -framework CoreFoundation -framework CoreAudio -l jdkmidi
MIDI_CPPFLAGS = -I$(SHARED_DIR)rtmidi -I$(SHARED_DIR)libjdkmidi/c++/include
MIDI_LDFLAGS	= -L$(SHARED_DIR)libjdkmidi/c++/b/build/lib
#
LDFLAGS=$(debug) -I$(SHARED_DIR) -I. $(MIDI_LDFLAGS) `/usr/local/bin/sdl2-config --libs` -lm -framework AppKit $(MIDI_LIBRARY)

# global CPP
CPP_DEFS 	= $(MIDI_DEFS) 
CPPFLAGS 	= -Wno-return-type -std=c++11 $(MIDI_CPPFLAGS) $(gme_CPPFLAGS) $(debugger_CPPFLAGS) -MMD -MP -Wno-int-to-void-pointer-cast

# currently, only because of Native File Dialog Implementation on OS X
OBJCFLAGS = $(gme_CPPFLAGS) $(debugger_CPPFLAGS) -MMD -MP
#### SOURCES
SOURCES=$(wildcard $(SHARED_DIR)*.cpp) $(wildcard $(SHARED_DIR)*/*.cpp)\
$(wildcard $(SHARED_DIR)*/*/*.cpp)\
$(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)\
$(wildcard */*/*/*.cpp) $(wildcard */*/*/*/*.cpp)

# Native File Directory
MSOURCES=$(wildcard $(SHARED_DIR)*/*.m)
###

# 
OBJECTS=$(SOURCES:.cpp=.cpp.o) $(MSOURCES:.m=.m.o)
EXECUTABLE=std


all: $(SOURCES) $(EXECUTABLE).app $(EXECUTABLE)

$(EXECUTABLE).app: $(EXECUTABLE) unrar 7zDec
	#rm -rf $@
	mkdir -p $@ $@/Contents $@/Contents/Resources $@/Contents/MacOS
	cp std $@/Contents/MacOS
	cp Info.plist $@/Contents
	cp unrar $@/Contents/Resources
	cp 7zDec $@/Contents/Resources
	touch $@
	
$(EXECUTABLE): $(OBJECTS) unrar 7zDec
	#echo $(SOURCES)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

7zDec: ../lzma1505/C/Util/7z/7zDec ../lzma1505/C/Util/7z/7zMain.c
	make -C ../lzma1505/C/Util/7z -f makefile.gcc
	cp ../lzma1505/C/Util/7z/7zDec 7zDec

unrar: ../unrar/unrar
	make -C ../unrar
	cp ../unrar/unrar unrar
#gme/gme/Spc_Cpu.o: gme/gme/Spc_Cpu.cpp gme/gme/Spc_Cpu.h debugger/report.h 
#	$(CPP) $(CFLAGS) -c $< -o $@
# Use the dependency files created by the -MMD option to gcc.


%.cpp.o: %.cpp
	$(CPP) $(CPP_DEFS) $(CPPFLAGS) -c $< -o $@

%.m.o: %.m
	$(OBJCC) $(OBJCFLAGS) -c $< -o $@
	
clean:
	rm -f $(EXECUTABLE); #gme/gme/*.o gme/demo/*.o gme/player/*.o debugger/*.o
	find . -name "*.o" -o -name "*.d" | xargs rm -rf
	find $(SHARED_DIR) -name "*.o" -o -name "*.d" | xargs rm -rf


# DO NOT DELETE THIS LINE -- make depend depends on it.
-include $(SOURCES:.cpp=.cpp.d) $(MSOURCES:.m=.m.d)
