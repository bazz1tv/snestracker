# the bin directory name, where all o and d files go as well
BIN = bin
# tells make to use this shell: 
SHELL := /bin/bash
# the only real reason I'm using that ^ is for pushd popd commands.. below..
ifeq ($(OS),Windows_NT)
    uname_S := Windows
else
    uname_S := $(shell uname -s)
endif

ifdef CROSS_COMPILE
	uname_S = Cross_Windows
endif

global_requirements = $(CPPSOURCES) $(CSOURCES) $(MSOURCES) \
$(BIN)/$(EXECUTABLE_DEBUGGER) $(BIN)/$(EXECUTABLE_TRACKER)

# This is where source that both tracker and STD will use. NOTE THE inclusion of the forward slash ;)
SHARED_DIR = shared/
PROJ_DIR = ext
#

CC=$(CROSS_COMPILE)g++
CPP=$(CROSS_COMPILE)g++
OBJCC=$(CROSS_COMPILE)g++
debug = -g
optimize = -O3

debugger_CPPFLAGS=$(debug) -I. -I$(SHARED_DIR) -I$(PROJ_DIR)/rtmidi -I$(PROJ_DIR)/NativeFileDialog/include\
-Idebugger/memory -Idebugger/content_areas -Ishared/windows/Options_Window\
-funroll-loops -Wall -Idebugger

tracker_CPPFLAGS=$(debug) -I. -I$(SHARED_DIR) -I$(PROJ_DIR)/rtmidi -I$(PROJ_DIR)/NativeFileDialog/include\
-Itracker -Itracker/content_areas -Ishared/windows/Options_Window\
-funroll-loops -Wall



prefix ?= /opt/local
gme_PREFIX ?= $(prefix)

# TODO - add rule for libjdkmidi
## mkdir -p out.$(uname_S)
## cd out.$(uname_S)
## ../configure --native-platform-linux=1
# for MIDI
#../configure --native-platform-linux=1 --cross-compiling=1 --compiler-prefix=i686-pc-mingw32- --target-platform-mingw32=1 --prefix=/usr/i686-pc-mingw32/usr/
# sudo -E make install-dev
ifeq ($(uname_S), Darwin)
	global_requirements += $(BIN)/$(EXECUTABLE_DEBUGGER).app $(BIN)/$(EXECUTABLE_TRACKER).app 
 
	CPPFLAGS += -Wno-int-to-void-pointer-cast -Wno-c++11-narrowing
	MIDI_DEFS     =   -D__MACOSX_CORE__ -DRTMIDI_DEBUG
	MIDI_LIBRARY  = -framework CoreMIDI -framework CoreFoundation -framework CoreAudio -ljdkmidi
	MIDI_CPPFLAGS = -I$(PROJ_DIR)/rtmidi -I$(prefix)/include
	MIDI_LDFLAGS  = -L$(prefix)/lib
	CPPSOURCES += $(PROJ_DIR)/rtmidi/RtMidi.cpp
	LDFLAGS += -framework AppKit
	CPP_DEFS += -DHAVE_STDINT_H # gme
	MSOURCES = $(PROJ_DIR)/NativeFileDialog/nfd_cocoa.m
	BOOST_LDFLAGS := -lboost_system-mt -lboost_filesystem-mt

	#gme_PREFIX ?= $(gme_default_PREFIX)
	gme_CPPFLAGS := $(debug) -c -I$(gme_PREFIX)include
	gme_LDFLAGS := -L$(gme_PREFIX)/lib -lgme_m -Xlinker -rpath -Xlinker $(gme_PREFIX)/lib

	sdl2_cflags = $(shell sdl2-config --cflags)
	sdl2_ldflags = $(shell sdl2-config --libs)

	EXECUTABLE_DEBUGGER = std
	EXECUTABLE_TRACKER = snes_tracker
	
	UNRAR = unrar
	DEC7Z = 7zDec
else ifeq ($(uname_S), Linux)
	MIDI_DEFS     =   -D__LINUX_ALSA__ -DRTMIDI_DEBUG
	MIDI_LIBRARY  = -ljdkmidi -lasound
	MIDI_CPPFLAGS = -I$(PROJ_DIR)/rtmidi -I$(prefix)/include
	MIDI_LDFLAGS  = -L$(prefix)/lib -lpthread
	CPPSOURCES += $(PROJ_DIR)/rtmidi/RtMidi.cpp
	#
	CPP_DEFS += -DHAVE_STDINT_H
	CPPFLAGS += -fpermissive
	# NFD
	CSOURCES += $(PROJ_DIR)/NativeFileDialog/nfd_gtk.c
	gtk_cflags = $(shell pkg-config --cflags gtk+-2.0)
	gtk_ldflags = $(shell pkg-config --libs gtk+-2.0)
	CPPFLAGS += $(gtk_cflags)
	LDFLAGS += $(gtk_ldflags) -Wl,-rpath=$(gme_PREFIX)/lib
	BOOST_LDFLAGS := -lboost_system-mt -lboost_filesystem-mt

	#gme_PREFIX ?= $(gme_default_PREFIX)
	gme_CPPFLAGS := $(debug) -c -I$(gme_PREFIX)include
	gme_LDFLAGS := -L$(gme_PREFIX)lib -lgme_m

	sdl2_cflags = $(shell sdl2-config --cflags)
	sdl2_ldflags = $(shell sdl2-config --libs)

	EXECUTABLE_DEBUGGER = std
	EXECUTABLE_TRACKER = snes_tracker
	
	UNRAR = unrar
	DEC7Z = 7zDec
else ifeq ($(uname_S), Cross_Windows)
	MIDI_DEFS     =   -D__WINDOWS_MM__ -DRTMIDI_DEBUG
	MIDI_LIBRARY  = -ljdkmidi -lwinmm
	MIDI_CPPFLAGS = -I$(PROJ_DIR)/rtmidi -I$(prefix)/include
	MIDI_LDFLAGS  = -L$(prefix)/lib
	CPPSOURCES += $(PROJ_DIR)/rtmidi/RtMidi.cpp
	#
	CPP_DEFS += -DHAVE_STDINT_H
	CPPFLAGS += -fpermissive
	# NFD
	CPPSOURCES += $(PROJ_DIR)/NativeFileDialog/nfd_win.cpp
	LDFLAGS += -mwindows -lole32 #-static-libgcc #-lcomctl32 -lcomdlg32
	BOOST_LDFLAGS := -lboost_system-mt -lboost_filesystem-mt
	# quick notes: /usr/lib/gcc/i686-pc-mingw32/4.9.3

	#gme_PREFIX ?= $(gme_default_PREFIX)
	gme_CPPFLAGS := $(debug) -c -I$(gme_PREFIX)/include
	gme_LDFLAGS := -L$(gme_PREFIX)/lib -lgme_m

	sdl2_cflags = $(shell PKG_CONFIG_LIBDIR="$(prefix)/lib/pkgconfig" pkg-config sdl2 --cflags)
	sdl2_ldflags = $(shell PKG_CONFIG_LIBDIR="$(prefix)/lib/pkgconfig" pkg-config sdl2 --libs)

	EXECUTABLE_DEBUGGER = std.exe
	EXECUTABLE_TRACKER = snes_tracker.exe
	UNRAR = unrar.exe
	DEC7Z = 7zDec.exe
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
CPPFLAGS 	+= $(sdl2_cflags) -Wno-return-type -std=c++11 $(MIDI_CPPFLAGS) \
$(gme_CPPFLAGS) -MMD -MP

# currently, only because of Native File Dialog Implementation on OS X
OBJCFLAGS = $(gme_CPPFLAGS) $(debugger_CPPFLAGS) -MMD -MP
#### CPPSOURCES
CPPSOURCES_SHARED = $(wildcard $(SHARED_DIR)*.cpp) $(wildcard $(SHARED_DIR)*/*.cpp)\
										$(wildcard $(SHARED_DIR)*/*/*.cpp)\

CPPSOURCES_DEBUGGER = $(CPPSOURCES) $(CPPSOURCES_SHARED) \
$(wildcard debugger/*.cpp) $(wildcard debugger/*/*.cpp) \
$(wildcard debugger/*/*/*.cpp)
											 
CPPSOURCES_TRACKER = $(CPPSOURCES) $(CPPSOURCES_SHARED) \
$(wildcard tracker/*.cpp) $(wildcard tracker/*/*.cpp) \
$(wildcard tracker/*/*/*.cpp)


# 
OBJECTS_DEBUGGER = $(CPPSOURCES_DEBUGGER:.cpp=.cpp.o) $(MSOURCES:.m=.m.o) $(CSOURCES:.c=.c.o)
OBJECTS_TRACKER = $(CPPSOURCES_TRACKER:.cpp=.cpp.o) $(MSOURCES:.m=.m.o) $(CSOURCES:.c=.c.o)

OBJECTS_DEBUGGER2 = $(addprefix bin/, $(OBJECTS_DEBUGGER))
OBJECTS_TRACKER2 = $(addprefix bin/, $(OBJECTS_TRACKER))

all: $(global_requirements)

$(OBJECTS_DEBUGGER2): CPPFLAGS += $(debugger_CPPFLAGS)
$(OBJECTS_TRACKER2): CPPFLAGS += $(tracker_CPPFLAGS)

$(BIN)/$(EXECUTABLE_DEBUGGER).app: $(EXECUTABLE_DEBUGGER) unrar 7zDec
	mkdir -p $@ $@/Contents $@/Contents/Resources $@/Contents/MacOS
	cp std $@/Contents/MacOS
	cp Info.plist $@/Contents
	cp unrar $@/Contents/Resources
	cp 7zDec $@/Contents/Resources
	touch $@

$(BIN)/$(EXECUTABLE_TRACKER).app: $(EXECUTABLE_TRACKER) unrar 7zDec
	mkdir -p $@ $@/Contents $@/Contents/Resources $@/Contents/MacOS
	cp std $@/Contents/MacOS
	cp Info.plist $@/Contents
	cp unrar $@/Contents/Resources
	cp 7zDec $@/Contents/Resources
	touch $@


$(BIN)/$(EXECUTABLE_DEBUGGER): $(OBJECTS_DEBUGGER2) $(BIN)/$(UNRAR) $(BIN)/$(DEC7Z)
	$(CC) $(OBJECTS_DEBUGGER2) $(LDFLAGS) -o $@

$(BIN)/$(EXECUTABLE_TRACKER): $(OBJECTS_TRACKER2) $(BIN)/$(UNRAR) $(BIN)/$(DEC7Z)
	$(CC) $(OBJECTS_TRACKER2) $(LDFLAGS) -o $@


$(BIN)/$(DEC7Z): $(PROJ_DIR)/lzma1505/C/Util/7z/7zDec $(PROJ_DIR)/lzma1505/C/Util/7z/7zMain.c
	cp $(PROJ_DIR)/lzma1505/C/Util/7z/7zDec* $(BIN)

$(PROJ_DIR)/lzma1505/C/Util/7z/7zDec:
	uname_S=$(uname_S) make -C $(PROJ_DIR)/lzma1505/C/Util/7z -f makefile.gcc

$(BIN)/$(UNRAR): $(PROJ_DIR)/unrar/$(UNRAR)
	cp $(PROJ_DIR)/unrar/$(UNRAR) $(BIN)
$(PROJ_DIR)/unrar/$(UNRAR):
	uname_S=$(uname_S) make -C $(PROJ_DIR)/unrar

$(BIN)/%.c.o: %.c %.h
	mkdir -p $(@D)
	$(CPP) $(CPP_DEFS) $(CPPFLAGS) -c $< -o $@

$(BIN)/%.cpp.o: %.cpp %.h
	mkdir -p $(@D)
	$(CPP) $(CPP_DEFS) $(CPPFLAGS) -c $< -o $@

$(BIN)/%.m.o: %.m %.h
	mkdir -p $(@D)
	$(OBJCC) $(OBJCFLAGS) -c $< -o $@
	
clean:
	rm -f $(BIN)/$(EXECUTABLE_DEBUGGER);
	rm -f $(BIN)/$(EXECUTABLE_TRACKER);
	find . -name "*.o" -o -name "*.d" | xargs rm -rf
	rm -f {$(PROJ_DIR)/lzma1505/C/Util/7z/,}{7zDec,7zDec.exe}
	rm -f {$(PROJ_DIR)/unrar/,}{unrar,unrar.exe}



# DO NOT DELETE THIS LINE -- make depend depends on it.
-include $(CPPSOURCES:.cpp=.cpp.d) $(MSOURCES:.m=.m.d) $(CSOURCES:.c=.c.d)
