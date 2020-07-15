# The object directory name, where all o and d files go as well
OBJ = obj
# the bin directory name
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

global_requirements = $(LIBJDKMIDI_LIB) $(CPPSOURCES) $(CSOURCES) $(MSOURCES) \
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
gme_PREFIX ?= ../submodules/libgme_m

ifeq ($(uname_S), Darwin)
	# Potentially useful code to check Mac OS Version
	#MAC_VER_MAJOR := $(shell sw_vers -productVersion | cut -f1 -d.)
	#MAC_VER_MINOR := $(shell sw_vers -productVersion | cut -f2 -d.)
	#MAC_VER_GT_10_14 := $(shell [ $(MAC_VER_MAJOR) -ge 10 -a $(MAC_VER_MINOR) -ge 14 ] && echo true)
	#ifeq ($(MAC_VER_GT_10_14),true)
		#CPPFLAGS += -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk
	#endif
	global_requirements += $(BIN)/$(EXECUTABLE_DEBUGGER).app $(BIN)/$(EXECUTABLE_TRACKER).app

	LIBJDKMIDI_OSDIR = $(PROJ_DIR)/libjdkmidi/c++/out.Mac

	#CPPFLAGS += -stdlib=libc++
	CPPFLAGS += -Wno-int-to-void-pointer-cast -Wno-c++11-narrowing
	MIDI_DEFS     =   -D__MACOSX_CORE__ -DRTMIDI_DEBUG
	MIDI_LIBRARY  = -framework CoreMIDI -framework CoreFoundation -framework CoreAudio
	MIDI_CPPFLAGS = -I$(PROJ_DIR)/rtmidi
	MIDI_LDFLAGS  =
	CPPSOURCES += $(PROJ_DIR)/rtmidi/RtMidi.cpp
	LDFLAGS += -framework AppKit
	CPP_DEFS += -DHAVE_STDINT_H # gme
	MSOURCES = $(PROJ_DIR)/NativeFileDialog/nfd_cocoa.m
	BOOST_LDFLAGS := -L /opt/local/lib -lboost_system -lboost_filesystem
	# It would be nice to automatically detect the Boost include path
	# (installed in seperate locations on HomeBrew vs. Macports)
	# for now, hardcode the Boost lib and include path installed from Homebrew
	CPPFLAGS += -I/opt/local/include

	# Due to use of C++11, with stdc++ I can only back support back to 10.9, but
	# if libc++ was used I could go back to 10.7. However, this would require
	# also configuring external/3rd party libraries to also compile with libc++.
	# It seems too much a hassle for the current set of priorities.
	ifeq ($(OSX_BACKSUPPORT), 1)
		OSX_BACKSUPPORT = -arch x86_64 -mmacosx-version-min=10.9 \
		-DMAC_OS_X_VERSION_MIN_REQUIRED=1090
	endif

	OBJCFLAGS += $(OSX_BACKSUPPORT)
	CPPFLAGS += $(OSX_BACKSUPPORT)
	LDFLAGS += $(OSX_BACKSUPPORT)

	#gme_PREFIX ?= $(gme_default_PREFIX)
	gme_CPPFLAGS := $(debug) -I$(gme_PREFIX)
	gme_LDFLAGS := -L$(gme_PREFIX) -lgme_m

	sdl2_cflags = $(shell sdl2-config --cflags)
	sdl2_ldflags = $(shell sdl2-config --libs)

	EXECUTABLE_DEBUGGER = std
	EXECUTABLE_TRACKER = st
	
	UNRAR = unrar
	DEC7Z = 7zDec
else ifeq ($(uname_S), Linux)
	LIBJDKMIDI_OSDIR = $(PROJ_DIR)/libjdkmidi/c++/out.Linux

	MIDI_DEFS     =   -D__LINUX_ALSA__ -DRTMIDI_DEBUG
	MIDI_LIBRARY  = -lasound
	MIDI_CPPFLAGS = -I$(PROJ_DIR)/rtmidi
	MIDI_LDFLAGS  = -lpthread
	CPPSOURCES += $(PROJ_DIR)/rtmidi/RtMidi.cpp
	#
	CPP_DEFS += -DHAVE_STDINT_H
	CPPFLAGS += -fpermissive
	# NFD
	CSOURCES += $(PROJ_DIR)/NativeFileDialog/nfd_gtk.c
	gtk_cflags = $(shell pkg-config --cflags gtk+-2.0)
	gtk_ldflags = $(shell pkg-config --libs gtk+-2.0)
	#CPPFLAGS += $(gtk_cflags)
	LDFLAGS += $(gtk_ldflags)
	BOOST_LDFLAGS := -lboost_system -lboost_filesystem

	# Allow Linux binary to source DLLs from same directory binary is located
	LDFLAGS += '-Wl,-rpath=$$ORIGIN'

	#gme_PREFIX ?= $(gme_default_PREFIX)
	gme_CPPFLAGS := $(debug) -I$(gme_PREFIX)
	gme_LDFLAGS := -L$(gme_PREFIX) -lgme_m

	sdl2_cflags = $(shell sdl2-config --cflags)
	sdl2_ldflags = $(shell sdl2-config --libs)

	EXECUTABLE_DEBUGGER = std
	EXECUTABLE_TRACKER = st
	
	UNRAR = unrar
	DEC7Z = 7zDec
else ifeq ($(uname_S), Cross_Windows)
	LIBJDKMIDI_OSDIR = $(PROJ_DIR)/libjdkmidi/c++/out.Windows
# debugging symbols are only visible from winedbg by -gstabs
	debug = -gstabs
	MIDI_DEFS     =   -D__WINDOWS_MM__ -DRTMIDI_DEBUG
	MIDI_LIBRARY  = -lwinmm
	MIDI_CPPFLAGS = -I$(PROJ_DIR)/rtmidi
	MIDI_LDFLAGS  =
	CPPSOURCES += $(PROJ_DIR)/rtmidi/RtMidi.cpp
	#
	CPP_DEFS += -DHAVE_STDINT_H
	CPPFLAGS += -fpermissive
	# NFD
	CPPSOURCES += $(PROJ_DIR)/NativeFileDialog/nfd_win.cpp
	# uuid needed for nfd
	LDFLAGS += -lole32 -luuid #-static-libgcc #-lcomctl32 -lcomdlg32
	BOOST_LDFLAGS := -lboost_system -lboost_filesystem
	# Example of static library inclusion:
	#BOOST_LDFLAGS := $(prefix)/lib/libboost_system.a $(prefix)/lib/libboost_filesystem.a
	# quick notes: /usr/lib/gcc/i686-pc-mingw32/4.9.3

	#gme_PREFIX ?= $(gme_default_PREFIX)
	gme_CPPFLAGS := $(debug) -I$(gme_PREFIX)
	gme_LDFLAGS := -L$(gme_PREFIX) -lgme_m

# Since we manually compile SDL2 on Gentoo, there is no difference, but leave the
# stanza here for good form when it becomes needed.
	ifneq (,$(wildcard /etc/gentoo-release))
		sdl2_cflags = $(shell PKG_CONFIG_LIBDIR="$(prefix)/lib/pkgconfig" pkg-config sdl2 --cflags)
		sdl2_ldflags = $(shell PKG_CONFIG_LIBDIR="$(prefix)/lib/pkgconfig" pkg-config sdl2 --libs)
	else
		# Ubuntu
		sdl2_cflags = $(shell PKG_CONFIG_LIBDIR="$(prefix)/lib/pkgconfig" pkg-config sdl2 --cflags)
		sdl2_ldflags = $(shell PKG_CONFIG_LIBDIR="$(prefix)/lib/pkgconfig" pkg-config sdl2 --libs)
	endif

	# Have a debug console
	sdl2_ldflags := $(subst -mwindows,,$(sdl2_ldflags)) -mconsole -lcomdlg32

	EXECUTABLE_DEBUGGER = std.exe
	EXECUTABLE_TRACKER = st.exe
	UNRAR = unrar.exe
	DEC7Z = 7zDec.exe

	OBJECTS_TRACKER := winres.o
else ifeq ($(uname_S), Windows)
    
else
	
endif

LIBJDKMIDI_LIB = $(LIBJDKMIDI_OSDIR)/build/lib/libjdkmidi.a
MIDI_CPPFLAGS += -I$(PROJ_DIR)/libjdkmidi/c++/include
MIDI_LIBRARY += $(LIBJDKMIDI_LIB)

## Native File Directory
CPPSOURCES += $(PROJ_DIR)/NativeFileDialog/nfd_common.cpp
###

LDFLAGS += $(debug) -I$(SHARED_DIR) -I. $(MIDI_LDFLAGS) $(BOOST_LDFLAGS) \
$(sdl2_ldflags) -lm $(MIDI_LIBRARY) $(gme_LDFLAGS)

# global CPP
CPP_DEFS 	+= $(MIDI_DEFS) 
CPPFLAGS 	+= $(sdl2_cflags) -Wno-return-type -std=c++11 $(MIDI_CPPFLAGS) \
$(gme_CPPFLAGS) -MMD -MP -MT '$@'

# currently, only because of Native File Dialog Implementation on OS X
OBJCFLAGS += $(gme_CPPFLAGS) $(debugger_CPPFLAGS) -MMD -MP
#### CPPSOURCES
CPPSOURCES_SHARED = $(wildcard $(SHARED_DIR)*.cpp) $(wildcard $(SHARED_DIR)*/*.cpp)\
										$(wildcard $(SHARED_DIR)*/*/*.cpp)\

# The Tracker cpp files here are so std can rip to STI file format
CPPSOURCES_DEBUGGER = $(CPPSOURCES) $(CPPSOURCES_SHARED) \
$(wildcard debugger/*.cpp) $(wildcard debugger/*/*.cpp) \
$(wildcard debugger/*/*/*.cpp) tracker/Instruments.cpp tracker/Samples.cpp tracker/ChunkLoader.cpp \
tracker/PanelCommon.cpp
											 
CPPSOURCES_TRACKER = $(CPPSOURCES) $(CPPSOURCES_SHARED) \
$(wildcard tracker/*.cpp) $(wildcard tracker/*/*.cpp) \
$(wildcard tracker/*/*/*.cpp)


# 
OBJECTS_DEBUGGER = $(CPPSOURCES_DEBUGGER:.cpp=.o) $(MSOURCES:.m=.o) $(CSOURCES:.c=.o)
OBJECTS_TRACKER += $(CPPSOURCES_TRACKER:.cpp=.o) $(MSOURCES:.m=.o) $(CSOURCES:.c=.o)

OBJECTS_DEBUGGER2 = $(addprefix $(OBJ)/, $(OBJECTS_DEBUGGER))
OBJECTS_TRACKER2 = $(addprefix $(OBJ)/, $(OBJECTS_TRACKER))

DEP_DEBUGGER = $(OBJECTS_DEBUGGER2:.o=.d)
DEP_TRACKER = $(OBJECTS_TRACKER2:.o=.d)

all: $(global_requirements)

$(DEP_DEBUGGER) $(OBJECTS_DEBUGGER2): CPPFLAGS += $(debugger_CPPFLAGS)
$(DEP_TRACKER) $(OBJECTS_TRACKER2): CPPFLAGS += $(tracker_CPPFLAGS)

$(BIN)/$(EXECUTABLE_DEBUGGER).app: Info.plist osx_ship.sh $(BIN)/$(EXECUTABLE_DEBUGGER) $(BIN)/$(UNRAR) $(BIN)/$(DEC7Z)
	mkdir -p $@ $@/Contents/Lib $@/Contents/Resources $@/Contents/MacOS
	cp $(BIN)/$(EXECUTABLE_DEBUGGER) $@/Contents/MacOS
	cp Info.plist $@/Contents
	cp $(BIN)/$(UNRAR) $@/Contents/Resources
	cp $(BIN)/$(DEC7Z) $@/Contents/Resources
	cp -r $(BIN)/cursors $@/Contents/Resources # Copy mouse icons
	bash ./osx_ship.sh $@ $(EXECUTABLE_DEBUGGER) $@/Contents/Lib
	touch $@

$(BIN)/$(EXECUTABLE_TRACKER).app: Info.plist osx_ship.sh $(BIN)/$(EXECUTABLE_TRACKER) $(BIN)/$(UNRAR) $(BIN)/$(DEC7Z)
	mkdir -p $@ $@/Contents/Lib $@/Contents/Resources $@/Contents/MacOS
	cp $(BIN)/$(EXECUTABLE_TRACKER) $@/Contents/MacOS
	cp Info.plist $@/Contents
	cp $(BIN)/$(UNRAR) $@/Contents/Resources
	cp $(BIN)/$(DEC7Z) $@/Contents/Resources
	cp -r $(BIN)/cursors $@/Contents/Resources # Copy mouse icons
	cp $(BIN)/tracker.spc $@/Contents/Resources # copy SPC driver
	bash ./osx_ship.sh $@ $(EXECUTABLE_TRACKER) $@/Contents/Lib
	#$@/Contents/MacOS/$(EXECUTABLE_TRACKER)
	touch $@


$(BIN)/$(EXECUTABLE_DEBUGGER): $(OBJECTS_DEBUGGER2) $(BIN)/$(UNRAR) $(BIN)/$(DEC7Z) $(LIBJDKMIDI_LIB)
	$(CC) $(OBJECTS_DEBUGGER2) $(LDFLAGS) -o $@

$(BIN)/$(EXECUTABLE_TRACKER): $(OBJECTS_TRACKER2) $(BIN)/$(UNRAR) $(BIN)/$(DEC7Z) $(LIBJDKMIDI_LIB)
	$(CC) $(OBJECTS_TRACKER2) $(LDFLAGS) -o $@


$(BIN)/$(DEC7Z): $(PROJ_DIR)/lzma1505/C/Util/7z/$(DEC7Z) $(PROJ_DIR)/lzma1505/C/Util/7z/7zMain.c
	cp $(PROJ_DIR)/lzma1505/C/Util/7z/$(DEC7Z) $(BIN)

$(PROJ_DIR)/lzma1505/C/Util/7z/$(DEC7Z):
	uname_S=$(uname_S) make -C $(PROJ_DIR)/lzma1505/C/Util/7z -f makefile.gcc

$(BIN)/$(UNRAR): $(PROJ_DIR)/unrar/$(UNRAR)
	cp $(PROJ_DIR)/unrar/$(UNRAR) $(BIN)
$(PROJ_DIR)/unrar/$(UNRAR):
	uname_S=$(uname_S) make -C $(PROJ_DIR)/unrar

$(LIBJDKMIDI_LIB): $(LIBJDKMIDI_OSDIR)/Makefile
	cd $(LIBJDKMIDI_OSDIR) && make

$(LIBJDKMIDI_OSDIR)/Makefile:
	cd $(LIBJDKMIDI_OSDIR) && osxflags="$(OSX_BACKSUPPORT)" ./configure


$(OBJ)/ext/NativeFileDialog/nfd_gtk.o: CPPFLAGS += $(gtk_cflags)

$(OBJ)/%.o: %.c
	@mkdir -p $(@D)
	$(CPP) $(CPP_DEFS) $(CPPFLAGS) -c $< -o $@

$(OBJ)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CPP) $(CPP_DEFS) $(CPPFLAGS) -c $< -o $@

$(OBJ)/%.o: %.m
	@mkdir -p $(@D)
	$(OBJCC) $(OBJCFLAGS) -c $< -o $@

$(OBJ)/winres.o: winres.rc
	$(CROSS_COMPILE)windres winres.rc $(OBJ)/winres.o
	
clean:
	@# Remove both Linux and Windows versions of unrar and 7zdec
	rm -f $(BIN)/{unrar,unrar.exe}
	rm -f $(BIN)/{7zDec,7zDec.exe}
	rm -f $(BIN)/$(EXECUTABLE_DEBUGGER);
	rm -f $(BIN)/$(EXECUTABLE_TRACKER);
	find . -name "*.o" -o -name "*.d" | xargs rm -rf
	rm -f {$(PROJ_DIR)/lzma1505/C/Util/7z/,}{7zDec,7zDec.exe}
	rm -f {$(PROJ_DIR)/unrar/,}{unrar,unrar.exe}
	cd $(LIBJDKMIDI_OSDIR) && make clean

# DO NOT DELETE THIS LINE -- make depend depends on it.
-include $(DEP_DEBUGGER) $(DEP_TRACKER)
