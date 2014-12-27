CC=g++
CPP=g++
LD=$(CPP)

#CFLAGS=-g -Wall -I../../src -fPIC `sdl-config --cflags`
CFLAGS=-O3 -funroll-loops -Wall -I. -I.. -I../../src `sdl-config --cflags` -Wall
LDFLAGS=-g `sdl-config --libs` -lm

PROG=vspcplay

OBJS = apu.o globals.o libspc.o soundux.o spc700.o main.o font.o sdlfont.o id666.o

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(OBJS) -o $(PROG) $(LDFLAGS) 

main.o: main.cpp mouse_hexdump.h gui/cursor.h gui/porttool.h mouse_hexdump_events.h sdl_dblclick.h sdl_dblclick_events.h sdl_userevents.h\
	spc_structs.h mode.h
	$(CPP) $(CFLAGS) -c main.cpp

# Order matters.. do the ones with headers first
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<


%.o: %.cpp %.h
	$(CPP) $(CFLAGS) -c $<

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<
	

clean:
	rm -f *.o $(PROG)
