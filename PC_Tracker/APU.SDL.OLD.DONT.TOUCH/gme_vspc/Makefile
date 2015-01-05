

CC=g++
CPP=g++
debug=-g
gme_CFLAGS=$(debug) -c -I. -Igme -Igme/gme -Idemo `sdl2-config --cflags`
LDFLAGS=$(debug) -I. -L/opt/local/lib `sdl2-config --libs` -lm

debugger_CFLAGS=$(debug) -Idebugger/memory -Idebugger/windows -O3\
-funroll-loops -Wall -Idebugger `sdl2-config --cflags` -Wall

debugger_LDFLAGS=$(debug) `sdl2-config --libs` -lm

CFLAGS = -std=c++11 $(gme_CFLAGS) $(debugger_CFLAGS) -Wno-c++11-extensions
####
gme_SOURCES= gme/gme/M3u_Playlist.cpp gme/gme/Data_Reader.cpp gme/gme/Multi_Buffer.cpp\
gme/gme/Blip_Buffer.cpp gme/gme/Effects_Buffer.cpp gme/gme/Fir_Resampler.cpp\
gme/gme/Music_Emu.cpp gme/gme/gme.cpp gme/gme/gme_File.cpp gme/gme/Snes_Spc.cpp\
gme/gme/Snes_Spc_misc.cpp gme/gme/Snes_Spc_state.cpp gme/gme/Spc_Filter.cpp gme/gme/Spc_Dsp.cpp gme/gme/Spc_Emu.cpp gme/demo/Wave_Writer.cpp\
gme/player/Music_Player.cpp gme/gme/Spc_Dsp_Register_Map_Interface.o
###
OBJECTS=$(gme_SOURCES:.cpp=.o)
debugger_OBJECTS= main.o app.o debugger/font.o debugger/sdlfont.o debugger/report.o debugger/globals.o debugger/Voice_Control.o debugger/sdl_dblclick.o\
debugger/Port_Tool.o debugger/gui/cursor.o debugger/memory/Mouse_Hexdump_Area.o debugger/Colors.o\
debugger/BaseD.o debugger/Debugger.o  debugger/error.o debugger/utility.o debugger/memory/Main_Memory_Area.o\
debugger/memory/memory.o Render_Context.o Player_Context.o debugger/windows/Main_Window.o Screen.o\
debugger/windows/Dsp_Window.o
OBJECTS += $(debugger_OBJECTS)
EXECUTABLE=std

all: $(gme_SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

main.o: Screen.h main.cpp debugger/gui/cursor.h debugger/mouse_hexdump_events.h debugger/sdl_dblclick.h\
	debugger/sdl_dblclick_events.h debugger/sdl_userevents.h
	$(CPP) $(CFLAGS) -c main.cpp -o main.o

gme/gme/Spc_Dsp.o: gme/gme/Spc_Dsp.cpp gme/gme/Spc_Dsp.h debugger/report.h 
	$(CPP) $(CFLAGS) -c $< -o $@

gme/gme/Snes_Spc.o: gme/gme/Snes_Spc.cpp gme/gme/Snes_Spc.h debugger/report.h 
	$(CPP) $(CFLAGS) -c $< -o $@

#gme/gme/Spc_Cpu.o: gme/gme/Spc_Cpu.cpp gme/gme/Spc_Cpu.h debugger/report.h 
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
	rm -f $(EXECUTABLE); #gme/gme/*.o gme/demo/*.o gme/player/*.o debugger/*.o
	find . -name "*.o" | xargs rm -rf

