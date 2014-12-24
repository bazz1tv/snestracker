CC=gcc
CPP=g++
LD=$(CPP)

#CFLAGS=-g -Wall -I../../src -fPIC `sdl-config --cflags`
CFLAGS=-O3 -funroll-loops -Wall -I../../src `sdl-config --cflags` -Wall
LDFLAGS=-g `sdl-config --libs` -lm

PROG=vspcplay

OBJS = apu.o globals.o libspc.o soundux.o spc700.o main.o font.o sdlfont.o id666.o

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(OBJS) -o $(PROG) $(LDFLAGS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp %.h
	$(CPP) $(CFLAGS) -c $<

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<
	

clean:
	rm -f *.o $(PROG)
