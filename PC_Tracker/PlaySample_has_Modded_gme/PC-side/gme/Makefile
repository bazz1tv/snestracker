CC=g++
debug=-g
CFLAGS=$(debug) -c -I. -Igme -Idemo -I/opt/local/include/SDL
LDFLAGS=$(debug) -I. -L/opt/local/lib -lsdl -lSDLMain -framework Cocoa
####
SOURCES= gme/M3u_Playlist.cpp gme/Data_Reader.cpp gme/Multi_Buffer.cpp\
gme/Blip_Buffer.cpp gme/Effects_Buffer.cpp gme/Fir_Resampler.cpp\
gme/Music_Emu.cpp gme/gme.cpp gme/Gme_File.cpp gme/Snes_Spc.cpp\
gme/Spc_Cpu.cpp gme/Spc_Dsp.cpp gme/Spc_Emu.cpp demo/Wave_Writer.cpp\
$(wildcard player/*.cpp)
###
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=play

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) gme/*.o demo/*.o player/*.o
