# ROM TITLE
ROMTITLE = Tracker.smc

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



CPU_OFILES =  cpu/2input.o cpu/emptyhandler.o cpu/fadein.o cpu/initsnes.o cpu/loadgraphics.o cpu/main.o cpu/plotter.o cpu/quicksetup.o cpu/spcProtocol.o cpu/strings.o cpu/vblank.o
APU_OFILES =  apu/commands.o apu/PatternPlayback.o apu/spc.o

OBJDIR = obj



# Object Information
CPU_OBJDIR = $(OBJDIR)/cpu
CPU_OBJS = $(addprefix $(OBJDIR)/,$(CPU_OFILES))
APU_OBJDIR = $(OBJDIR)/apu
APU_OBJS = $(addprefix $(OBJDIR)/,$(APU_OFILES))

OBJS = $(CPU_OBJS)
OBJS += $(APU_OBJS)
# --

VPATH = $(OBJDIR):gfx:gfx/ascii


# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# MAIN GOALS / RECIPES
# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
$(ROMTITLE): $(CPU_OBJS) Makefile
	echo "" > linkfile.tmp
	echo "[objects]" >> linkfile.tmp
	echo "$(CPU_OBJS)" >> linkfile.tmp
	sed y/" "/"\n"/ linkfile.tmp > linkfile
	echo "" >> linkfile
	rm linkfile.tmp
	$(LD) $(LDFLAGS) linkfile $(ROMTITLE)
	rm linkfile

apu/spc.bin: $(APU_OBJS) Makefile
	echo "" > linkfile_spc.tmp
	echo "[objects]" >> linkfile_spc.tmp
	echo "$(APU_OBJS)" >> linkfile_spc.tmp
	sed y/" "/"\n"/ linkfile_spc.tmp > linkfile_spc
	echo "" >> linkfile_spc
	rm linkfile_spc.tmp
	$(LD) $(APU_LDFLAGS) linkfile_spc $(OBJDIR)/$@
	cp $(OBJDIR)/$@ $@
	mv $(APU_OBJDIR)/spc.sym .
	bazz-size $@
	rm linkfile_spc

$(OBJDIR)/cpu/%.o: cpu/%.s
	mkdir -p $(@D)
	$(CPU_CC) $(CPU_CFLAGS) $< $@

$(OBJDIR)/apu/%.o: apu/%.s
	mkdir -p $(@D)
	$(APU_CC) $(APU_CFLAGS) $< $@
# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#------------------------------
#APU SIDE
#------------------------------

obj/apu/commands.o: apu/commands.s \
	apu/memorymap.i \
	apu/enums.i \
	apu/commands.i

obj/apu/PatternPlayback.o: apu/PatternPlayback.s \
	apu/memorymap.i \
	apu/enums.i \
	apu/commands.i

obj/apu/spc.o: apu/spc.s \
	apu/memorymap.i \
	apu/enums.i \
	apu/commands.i

#------------------------------
#CPU SIDE
#------------------------------

obj/cpu/2input.o: cpu/2input.s \
	cpu/memorymap.i \
	cpu/enums.i

obj/cpu/emptyhandler.o: cpu/emptyhandler.s \
	cpu/memorymap.i

obj/cpu/fadein.o: cpu/fadein.s \
	cpu/memorymap.i

obj/cpu/initsnes.o: cpu/initsnes.s \
	cpu/memorymap.i

obj/cpu/loadgraphics.o: cpu/loadgraphics.s \
	cpu/memorymap.i \
	cpu/loadgraphics.i

obj/cpu/main.o: cpu/main.s \
	cpu/memorymap.i \
	cpu/initsnes.i \
	cpu/loadgraphics.i \
	cpu/enums.i \
	apu/spc.bin \
	samples/piano.brr \
	samples/piano.brr \
	mainlogo/mainlogo.map \
	mainlogo/mainlogo.pic \
	mainlogo/mainlogo.clr

obj/cpu/plotter.o: cpu/plotter.s \
	cpu/memorymap.i \
	cpu/loadgraphics.i \
	cpu/initsnes.i \
	cpu/strings.i \
	cpu/fadein.i \
	cpu/enums.i

obj/cpu/quicksetup.o: cpu/quicksetup.s \
	cpu/memorymap.i \
	cpu/loadgraphics.i \
	cpu/enums.i \
	ascii.clr \
	ascii.pic

obj/cpu/spcProtocol.o: cpu/spcProtocol.s \
	cpu/memorymap.i \
	cpu/SNESRegs.i \
	apu/commands.i \
	cpu/enums.i

obj/cpu/strings.o: cpu/strings.s \
	cpu/memorymap.i \
	cpu/enums.i

obj/cpu/vblank.o: cpu/vblank.s \
	cpu/memorymap.i



.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(OFILES) core *~ $(ROMTITLE) *.lib obj/*.o *.sym apu/spc.bin
	find gfx -name "*.pic" -o -name "*.clr" -o -name "*.map" | xargs rm -f

include Makefile.gfx

