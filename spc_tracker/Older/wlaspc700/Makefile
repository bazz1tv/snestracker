

CC = wla-65816
CFLAGS = -vo
LD = wlalink
LDFLAGS = -vS

ROMTITLE = TimerOptions.sfc
SFILES = main.s loadgraphics.s initsnes.s 2input.s strings.s quicksetup.s emptyhandler.s plotter.s fadein.s \
	TimerOptionsScreenMain.s TimerOptionsScreenInput.s vblank.s
#IFILES = header.i
# It is important that all object files are listed in OFILES
OFILES = main.o loadgraphics.o initsnes.o 2input.o strings.o quicksetup.o emptyhandler.o plotter.o fadein.o \
	TimerOptionsScreenInput.o TimerOptionsScreenMain.o vblank.o DisplayUnderlines.o
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(OFILES))

VPATH = apu:apu/$(OBJDIR):$(OBJDIR):gfx:gfx/ascii

$(ROMTITLE): $(OFILES) Makefile
	echo "" > linkfile
	echo "[objects]" >> linkfile
	echo "$(OBJS)" >> linkfile
	sed y/" "/"\n"/ linkfile > link
	echo "" >> link
	rm linkfile
#	sed s/"[a-z0-9]\{1,\}\.o"/"obj\/&"/g link > linkfile
	$(LD) $(LDFLAGS) link $(ROMTITLE)
	
	
spc.bin: spc.o
	cd $(<D); make spc.bin
spc.o: spc.s
	cd $(<D); make spc.o
	
2input.o: 2input.s \
	header.i \
	enums.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
emptyhandler.o: emptyhandler.s \
	header.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
fadein.o: fadein.s \
	header.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
initsnes.o: initsnes.s \
	header.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
loadgraphics.o: loadgraphics.s \
	header.i \
	LoadGraphics.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
main.o: main.s \
	header.i \
	initsnes.i \
	loadgraphics.i \
	enums.i \
	spc.bin \
	samples/piano.brr \
	samples/piano.brr \
	mainlogo/mainlogo.map \
	mainlogo/mainlogo.pic \
	mainlogo/mainlogo.clr
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
plotter.o: plotter.s \
	header.i \
	loadgraphics.i \
	initsnes.i \
	strings.i \
	fadein.i \
	enums.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
quicksetup.o: quicksetup.s \
	header.i \
	LoadGraphics.i \
	enums.i \
	ascii.clr \
	ascii.pic
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
strings.o: strings.s \
	header.i \
	enums.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
TimerOptionsScreenInput.o: TimerOptionsScreenInput.s \
	header.i \
	TimerOptionsScreen.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
TimerOptionsScreenMain.o: TimerOptionsScreenMain.s \
	header.i \
	loadgraphics.i \
	initsnes.i \
	strings.i \
	fadein.i \
	enums.i \
	TimerOptionsScreen.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
		
vblank.o: vblank.s \
	header.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@
	
DisplayUnderlines.o: DisplayUnderlines.s \
	header.i
		$(CC) $(CFLAGS) $< $(OBJDIR)/$@


clean:
	rm -f $(OFILES) core *~ tracker.smc *.lib obj/*.o *.sym
