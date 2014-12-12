
CC = wla-spc700
CFLAGS = -ov
LD = wlalink
LDFLAGS = -bvS

SFILES = spc.s 
IFILES = defines.i
OFILES = spc.o
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(OFILES))

VPATH = $(OBJDIR)
ROMTITLE = spc.bin	

spc.bin: $(OFILES) Makefile
	echo "" > linkfile
	echo "[objects]" >> linkfile
	echo "$(OBJS)" >> linkfile
	sed y/" "/"\n"/ linkfile > linkfile2
	echo "" >> linkfile2
	rm linkfile
	$(LD) $(LDFLAGS) linkfile2 $(ROMTITLE)
	
spc.o: spc.s defines.i
	$(CC) $(CFLAGS) $< $(OBJDIR)/$@


#$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ result.rom result.sym spc.bin
