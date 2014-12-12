# ROM TITLE
ROMTITLE = Tracker.fig
# Wla-65816 Compiler information (CPU SIDE)
CPU_CC = wla-65816
CPU_CFLAGS = -vo
LD = wlalink
LDFLAGS = -vS
#--

# Wla-spc700 Compiler information (APU SIDE)
APU_CC = wla-spc700
APU_CFLAGS = -ov
APU_LDFLAGS = -bvS



# It is important that all object files are listed in OFILES
CPU_OFILES =  cpu/2input.o cpu/SelectionRect.o cpu/Underline.o cpu/fade.o cpu/loadgraphics.o cpu/main.o cpu/memorymap.o cpu/modes.o cpu/quicksetup.o cpu/ramsections.o cpu/spcProtocol.o cpu/strings.o cpu/vblank.o cpu/PatternTable/input.o cpu/PatternTable/main.o cpu/PatternTable/vblank.o cpu/TimerOptions/input.o cpu/TimerOptions/main.o
APU_OFILES =  apu/PatternPlayback.o apu/commands.o apu/main.o apu/samples.o
# Object Information
OBJDIR = obj

CPU_OBJDIR = $(OBJDIR)/cpu
CPU_OBJS = $(addprefix $(OBJDIR)/,$(CPU_OFILES))
APU_OBJDIR = $(OBJDIR)/apu
APU_OBJS = $(addprefix $(OBJDIR)/,$(APU_OFILES))

OFILES = $(CPU_OFILES)
OFILES += $(APU_OFILES)

OBJS = $(CPU_OBJS)
OBJS += $(APU_OBJS)
# --
VPATH = $(OBJDIR):gfx:gfx/ascii

# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# MAIN GOALS / RECIPES
# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
$(ROMTITLE): $(CPU_OFILES) Makefile
	echo "" > linkfile.tmp
	echo "[objects]" >> linkfile.tmp
	echo "$(CPU_OBJS)" >> linkfile.tmp
#	echo "$(APU_OBJS)" >> linkfile.tmp
	sed y/" "/"\n"/ linkfile.tmp > linkfile
	echo "" >> linkfile
	rm linkfile.tmp
	$(LD) $(LDFLAGS) linkfile $(ROMTITLE)
	rm linkfile
#	./fix $(ROMTITLE)
#	mv fix.fig $(ROMTITLE)
	
apu/spc.bin: $(APU_OFILES) Makefile
	echo "" > linkfile_spc.tmp
	echo "[objects]" >> linkfile_spc.tmp
	echo "$(APU_OBJS)" >> linkfile_spc.tmp
	sed y/" "/"\n"/ linkfile_spc.tmp > linkfile_spc
	echo "" >> linkfile_spc
	rm linkfile_spc.tmp
	$(LD) $(APU_LDFLAGS) linkfile_spc $(OBJDIR)/$@
	cp $(OBJDIR)/$@ $@
	mv $(APU_OBJDIR)/spc.sym .
	size $@
	rm linkfile_spc
# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#------------------------------
# UTILITY Directory Creators	
$(OBJDIR):
	mkdir -p $(OBJDIR)
	
$(CPU_OBJDIR):
	mkdir -p $(CPU_OBJDIR)
	mkdir -p $(CPU_OBJDIR)/TimerOptions
	mkdir -p $(CPU_OBJDIR)/PatternTable
$(APU_OBJDIR):
	mkdir -p $(APU_OBJDIR)
#------------------------------

#------------------------------
#APU SIDE
#------------------------------

# apu/PatternPlayback.s 
apu/PatternPlayback.o: apu/PatternPlayback.s \
	apu/memorymap.i \
	apu/enums.i \
	apu/SPCCommands.i | $(APU_OBJDIR)
		$(APU_CC) $(APU_CFLAGS) $< $(OBJDIR)/$@
# apu/commands.s 
apu/commands.o: apu/commands.s \
	apu/memorymap.i \
	apu/enums.i \
	apu/SPCCommands.i | $(APU_OBJDIR)
		$(APU_CC) $(APU_CFLAGS) $< $(OBJDIR)/$@
apu/samples.o: apu/samples.s \
	apu/memorymap.i \
	apu/enums.i | $(APU_OBJDIR)
		$(APU_CC) $(APU_CFLAGS) $< $(OBJDIR)/$@
# apu/main.s 
apu/main.o: apu/main.s apu/commands.s apu/PatternPlayback.s apu/samples.s \
	apu/memorymap.i \
	apu/enums.i \
	apu/SPCCommands.i | $(APU_OBJDIR)
		$(APU_CC) $(APU_CFLAGS) $< $(OBJDIR)/$@
#------------------------------
#CPU SIDE
#------------------------------

# cpu/2input.s 
cpu/2input.o: cpu/2input.s \
	cpu/memorymap.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/SelectionRect.s 
cpu/SelectionRect.o: cpu/SelectionRect.s \
	cpu/memorymap.i \
	cpu/TimerOptions/enums.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/Underline.s 
cpu/Underline.o: cpu/Underline.s \
	cpu/memorymap.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/fade.s 
cpu/fade.o: cpu/fade.s \
	cpu/memorymap.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/loadgraphics.s 
cpu/loadgraphics.o: cpu/loadgraphics.s \
	cpu/memorymap.i \
	cpu/LoadGraphics.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/main.s 
cpu/main.o: cpu/main.s \
	cpu/memorymap.i \
	cpu/initsnes.i \
	cpu/loadgraphics.i apu/spc.bin \
	| $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/memorymap.s 
cpu/memorymap.o: cpu/memorymap.s \
	cpu/memorymap.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/modes.s 
cpu/modes.o: cpu/modes.s \
	cpu/memorymap.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/quicksetup.s 
cpu/quicksetup.o: cpu/quicksetup.s \
	cpu/memorymap.i \
	cpu/LoadGraphics.i \
	ascii.clr \
	ascii.pic | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/ramsections.s 
cpu/ramsections.o: cpu/ramsections.s \
	cpu/memorymap.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/spcProtocol.s 
cpu/spcProtocol.o: cpu/spcProtocol.s \
	cpu/memorymap.i \
	cpu/SNESRegs.i \
	apu/enums.i \
	apu/SPCCommands.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/strings.s 
cpu/strings.o: cpu/strings.s \
	cpu/memorymap.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/vblank.s 
cpu/vblank.o: cpu/vblank.s \
	cpu/memorymap.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/PatternTable/input.s 
cpu/PatternTable/input.o: cpu/PatternTable/input.s \
	cpu/memorymap.i \
	apu/enums.i \
	apu/SPCCommands.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/PatternTable/main.s 
cpu/PatternTable/main.o: cpu/PatternTable/main.s \
	cpu/memorymap.i \
	cpu/loadgraphics.i \
	cpu/initsnes.i \
	cpu/strings.i \
	cpu/fade.i \
	cpu/PatternTable/RAM.i \
	apu/enums.i \
	apu/SPCCommands.i \
	cpu/SNESRegs.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/PatternTable/vblank.s 
cpu/PatternTable/vblank.o: cpu/PatternTable/vblank.s \
	cpu/memorymap.i \
	cpu/LoadGraphics.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/TimerOptions/input.s 
cpu/TimerOptions/input.o: cpu/TimerOptions/input.s \
	cpu/memorymap.i \
	cpu/TimerOptions/enums.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@
# cpu/TimerOptions/main.s 
cpu/TimerOptions/main.o: cpu/TimerOptions/main.s \
	cpu/memorymap.i \
	cpu/loadgraphics.i \
	cpu/initsnes.i \
	cpu/strings.i \
	cpu/fade.i \
	apu/enums.i \
	apu/SPCCommands.i \
	cpu/TimerOptions/enums.i \
	cpu/SNESRegs.i | $(CPU_OBJDIR)
		$(CPU_CC) $(CPU_CFLAGS) $< $(OBJDIR)/$@

clean:
	rm -rf $(OBJDIR) $(OFILES) core *~ tracker.smc *.lib obj/*.o *.sym apu/spc.bin
	cd apu; make clean