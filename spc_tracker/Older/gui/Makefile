
CC = wla-65816
CFLAGS = -vo
LD = wlalink
LDFLAGS = -vS

SFILES = main.s loadgraphics.s initsnes.s 2input.s strings.s quicksetup.s emptyhandler.s
IFILES = header.i
OFILES = main.o loadgraphics.o initsnes.o 2input.o strings.o quicksetup.o emptyhandler.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile tracker.smc
	rm *.o
	
spc.bin:
	tasm -07 -i -b spc.s spc.bin
	size spc.bin
	
main.o: main.s loadgraphics.o initsnes.o 2input.o strings.o quicksetup.o loadgraphicsmacros.i initsnesmacro.i plotter.o spc.bin
	$(CC) $(CFLAGS) main.s
	
plotter.o: plotter.s initsnesmacro.i loadgraphicsmacros.i stringsmacros.i 2input.o
	$(CC) $(CFLAGS) plotter.s plotter.o

loadgraphics.o: loadgraphics.s header.i
	$(CC) $(CFLAGS) loadgraphics.s

initsnes.o: initsnes.s header.i
	$(CC) $(CFLAGS) initsnes.s initsnes.o
	
2input.o: 2input.s header.i
	$(CC) $(CFLAGS) 2input.s 2input.o
	
strings.o: strings.s header.i
	$(CC) $(CFLAGS) strings.s strings.o
	
quicksetup.o: quicksetup.s header.i loadgraphicsmacros.i
	$(CC) $(CFLAGS) quicksetup.s quicksetup.o

	
emptyhandler.o: emptyhandler.s header.i
	$(CC) $(CFLAGS) emptyhandler.s emptyhandler.o

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.smc *.lib
