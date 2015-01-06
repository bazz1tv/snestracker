

CC=g++
CPP=g++
debug=-g
gme_CFLAGS=$(debug) -c -I. -Igme -Igme/gme -Idemo `sdl2-config --cflags`
LDFLAGS=$(debug) -I. -L/opt/local/lib `sdl2-config --libs` -lm

debugger_CFLAGS=$(debug) -Idebugger/memory -Idebugger/windows -O3\
-funroll-loops -Wall -Idebugger `sdl2-config --cflags` -Wall

debugger_LDFLAGS=$(debug) `sdl2-config --libs` -lm

CFLAGS = -std=c++11 $(gme_CFLAGS) $(debugger_CFLAGS) -MMD -MP
####
SOURCES=$(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
###
OBJECTS=$(SOURCES:.cpp=.o)
#OBJECTS += $(debugger_OBJECTS)
EXECUTABLE=std

all: $(gme_SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

#gme/gme/Spc_Cpu.o: gme/gme/Spc_Cpu.cpp gme/gme/Spc_Cpu.h debugger/report.h 
#	$(CPP) $(CFLAGS) -c $< -o $@
# Use the dependency files created by the -MMD option to gcc.


%.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

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
-include $(SOURCES:.cpp=.d) 
