CC=g++
CPP=g++
debug=-g
gme_CFLAGS=$(debug) -c -I. -Igme -Igme/gme -Idemo `sdl-config --cflags`
LDFLAGS=$(debug) -I. -L/opt/local/lib `sdl-config --libs` -lm

vspc_CFLAGS=$(debug) -O3 -funroll-loops -Wall -Ivspc `sdl-config --cflags` -Wall
vspc_LDFLAGS=$(debug) -I vspc `sdl-config --libs` -lm

CFLAGS = $(gme_CFLAGS) $(vspc_CFLAGS)
####
gme_SOURCES= gme/gme/M3u_Playlist.cpp gme/gme/Data_Reader.cpp gme/gme/Multi_Buffer.cpp\
gme/gme/Blip_Buffer.cpp gme/gme/Effects_Buffer.cpp gme/gme/Fir_Resampler.cpp\
gme/gme/Music_Emu.cpp gme/gme/gme.cpp gme/gme/gme_File.cpp gme/gme/Snes_Spc.cpp\
gme/gme/Snes_Spc_misc.cpp gme/gme/Snes_Spc_state.cpp gme/gme/Spc_Filter.cpp gme/gme/Spc_Dsp.cpp gme/gme/Spc_Emu.cpp gme/demo/Wave_Writer.cpp\
gme/player/Music_Player.cpp
###
OBJECTS=$(gme_SOURCES:.cpp=.o)
vspc_OBJECTS=  vspc/main.o vspc/font.o vspc/sdlfont.o vspc/report.o vspc/globals.o vspc/events.o vspc/track.o vspc/voices.o vspc/sdl_dblclick.o\
vspc/gui/porttool.o vspc/gui/cursor.o vspc/mouse_hexdump.o
OBJECTS += $(vspc_OBJECTS)
EXECUTABLE=vspc_blargg

all: $(gme_SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

vspc/main.o: vspc/main.cpp vspc/mouse_hexdump.h vspc/gui/cursor.h vspc/gui/porttool.h vspc/mouse_hexdump_events.h vspc/sdl_dblclick.h\
	vspc/sdl_dblclick_events.h vspc/sdl_userevents.h\
	vspc/mode.h
	$(CPP) $(CFLAGS) -c vspc/main.cpp -o vspc/main.o

gme/gme/Spc_Dsp.o: gme/gme/Spc_Dsp.cpp gme/gme/Spc_Dsp.h vspc/report.h 
	$(CPP) $(CFLAGS) -c $< -o $@

gme/gme/Snes_Spc.o: gme/gme/Snes_Spc.cpp gme/gme/Snes_Spc.h vspc/report.h 
	$(CPP) $(CFLAGS) -c $< -o $@

#gme/gme/Spc_Cpu.o: gme/gme/Spc_Cpu.cpp gme/gme/Spc_Cpu.h vspc/report.h 
#	$(CPP) $(CFLAGS) -c $< -o $@

%.o: %.cpp %.h
	$(CPP) $(CFLAGS) -c $< -o $@
	
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

# Order matters.. do the ones with headers first
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@




%.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@
	


clean:
	rm -f $(EXECUTABLE); #gme/gme/*.o gme/demo/*.o gme/player/*.o vspc/*.o
	find . -name "*.o" | xargs rm -rf

