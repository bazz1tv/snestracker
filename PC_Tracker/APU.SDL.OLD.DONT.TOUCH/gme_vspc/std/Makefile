#unwritten rule .. app.h RULES ALL. if it changes, the whole fucking thing needs to be recompiled...
# tells make to use this shell: 
SHELL := /bin/bash
# the only real reason I'm using that ^ is for pushd popd commands.. below..
ifeq ($(OS),Windows_NT)
    uname_S := Windows
else
    uname_S := $(shell uname -s)
endif

global_requirements = $(CPPSOURCES) $(CSOURCES) $(MSOURCES) $(EXECUTABLE)

# This is where source that both tracker and STD will use. NOTE THE inclusion of the forward slash ;)
SHARED_DIR = ../shared/
PROJ_DIR = ..
#

CC=g++
CPP=g++
OBJCC=g++
debug = -g
optimize = -O3

debugger_CPPFLAGS=$(debug) -I. -I$(SHARED_DIR) -I$(PROJ_DIR)/NativeFileDialog/include\
-Idebugger/memory -Idebugger/content_areas -Idebugger/windows/Options_Window\
-funroll-loops -Wall -Idebugger -Wall

gme_default_PREFIX := /opt/local/

# TODO - add rule for libjdkmidi
## mkdir -p out.$(uname_S)
## cd out.$(uname_S)
## ../configure --native-platform-linux=1
# for MIDI
ifeq ($(uname_S), Darwin)
	global_requirements += $(EXECUTABLE).app 
	CPPFLAGS += -Wno-int-to-void-pointer-cast -Wno-c++11-narrowing
	MIDI_DEFS     =   -D__MACOSX_CORE__ -DRTMIDI_DEBUG
	MIDI_LIBRARY  = -framework CoreMIDI -framework CoreFoundation -framework CoreAudio -l jdkmidi
	MIDI_CPPFLAGS = -I$(SHARED_DIR)rtmidi -I$(SHARED_DIR)libjdkmidi/c++/include
	MIDI_LDFLAGS  = -L$(SHARED_DIR)libjdkmidi/c++/out.$(uname_S)/build/lib
	LDFLAGS += -framework AppKit
	CPP_DEFS += -DHAVE_STDINT_H # gme
	MSOURCES=$(wildcard $(PROJ_DIR)/NativeFileDialog/*.m)
	BOOST_LDFLAGS := -lboost_system-mt -lboost_filesystem-mt

	gme_PREFIX ?= $(gme_default_PREFIX)
	gme_CPPFLAGS := $(debug) -c -I$(gme_PREFIX)include
	gme_LDFLAGS := -L$(gme_PREFIX)lib -lgme_m

	sdl2_cflags = $(shell sdl2-config --cflags)
	sdl2_ldflags = $(shell sdl2-config --libs)
else ifeq ($(uname_S), Linux)
	MIDI_DEFS     =   -D__LINUX_ALSA__ -DRTMIDI_DEBUG
	MIDI_LIBRARY  = -L/opt/local/lib -ljdkmidi -lasound
	MIDI_CPPFLAGS = -I$(SHARED_DIR)rtmidi -I$(SHARED_DIR)libjdkmidi/c++/include
	MIDI_LDFLAGS	= -L$(SHARED_DIR)libjdkmidi/c++/out.$(uname_S)/build/lib
	CPP_DEFS += -DHAVE_STDINT_H
	CPPFLAGS += -fpermissive
	# NFD
	CSOURCES += $(PROJ_DIR)/NativeFileDialog/nfd_gtk.c
	gtk_cflags = $(shell pkg-config --cflags gtk+-3.0)
	gtk_ldflags = $(shell pkg-config --libs gtk+-3.0)
	CPPFLAGS += $(gtk_cflags) $(sdl2_cflags)
	LDFLAGS += $(gtk_ldflags) $(sdl2_ldflags) -Wl,-rpath=$(gme_PREFIX)/lib
	BOOST_LDFLAGS := -lboost_system-mt -lboost_filesystem-mt

	gme_PREFIX ?= $(gme_default_PREFIX)
	gme_CPPFLAGS := $(debug) -c -I$(gme_PREFIX)include
	gme_LDFLAGS := -L$(gme_PREFIX)lib -lgme_m

	sdl2_cflags = $(shell sdl2-config --cflags)
	sdl2_ldflags = $(shell sdl2-config --libs)
else ifeq ($(uname_S), Windows)
    
else
	
endif

## Native File Directory
CPPSOURCES += $(PROJ_DIR)/NativeFileDialog/nfd_common.cpp
###

LDFLAGS += $(debug) -I$(SHARED_DIR) -I. $(MIDI_LDFLAGS) $(BOOST_LDFLAGS) \
$(sdl2_ldflags) -lm $(MIDI_LIBRARY) $(gme_LDFLAGS)

# global CPP
CPP_DEFS 	+= $(MIDI_DEFS) 
CPPFLAGS 	+= -Wno-return-type -std=c++11 $(MIDI_CPPFLAGS) $(gme_CPPFLAGS) $(debugger_CPPFLAGS) -MMD -MP

# currently, only because of Native File Dialog Implementation on OS X
OBJCFLAGS = $(gme_CPPFLAGS) $(debugger_CPPFLAGS) -MMD -MP
#### CPPSOURCES
CPPSOURCES += $(wildcard $(SHARED_DIR)*.cpp) $(wildcard $(SHARED_DIR)*/*.cpp)\
$(wildcard $(SHARED_DIR)*/*/*.cpp)\
$(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)\
$(wildcard */*/*/*.cpp) $(wildcard */*/*/*/*.cpp)



# 
OBJECTS=$(CPPSOURCES:.cpp=.cpp.o) $(MSOURCES:.m=.m.o) $(CSOURCES:.c=.c.o)
EXECUTABLE=std

all: $(global_requirements)

$(EXECUTABLE).app: $(EXECUTABLE) unrar 7zDec
	#rm -rf $@
	mkdir -p $@ $@/Contents $@/Contents/Resources $@/Contents/MacOS
	cp std $@/Contents/MacOS
	cp Info.plist $@/Contents
	cp unrar $@/Contents/Resources
	cp 7zDec $@/Contents/Resources
	touch $@

$(EXECUTABLE): $(OBJECTS) unrar 7zDec
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

7zDec: ../lzma1505/C/Util/7z/7zDec ../lzma1505/C/Util/7z/7zMain.c
	cp ../lzma1505/C/Util/7z/7zDec 7zDec
../lzma1505/C/Util/7z/7zDec:
	make -C ../lzma1505/C/Util/7z -f makefile.gcc

unrar: ../unrar/unrar
	cp ../unrar/unrar unrar
../unrar/unrar:
	make -C ../unrar
#gme/gme/Spc_Cpu.o: gme/gme/Spc_Cpu.cpp gme/gme/Spc_Cpu.h debugger/report.h 
#	$(CPP) $(CFLAGS) -c $< -o $@
# Use the dependency files created by the -MMD option to gcc.

%.c.o: %.c
	$(CPP) $(CPP_DEFS) $(CPPFLAGS) -c $< -o $@

%.cpp.o: %.cpp
	$(CPP) $(CPP_DEFS) $(CPPFLAGS) -c $< -o $@

%.m.o: %.m
	$(OBJCC) $(OBJCFLAGS) -c $< -o $@
	
clean:
	rm -f $(EXECUTABLE); #gme/gme/*.o gme/demo/*.o gme/player/*.o debugger/*.o
	find . -name "*.o" -o -name "*.d" | xargs rm -rf
	find $(SHARED_DIR) -name "*.o" -o -name "*.d" | xargs rm -rf


# DO NOT DELETE THIS LINE -- make depend depends on it.
-include $(CPPSOURCES:.cpp=.cpp.d) $(MSOURCES:.m=.m.d) $(CSOURCES:.c=.c.d)
