perl -e 'print "\x00"x0x200' > /tmp/derp
cat snes-side/blarg.brr >> /tmp/derp
perl -e 'print "\x00"x0x9c8' >> /tmp/derp
cat snes-side/SPC.OBJ >> /tmp/derp
perl -e 'print "\x00"x0xef9d' >> /tmp/derp
perl -e 'print "\x00"x0x10' >> /tmp/derp
perl -e 'print "\x00"x4' >> /tmp/derp
perl -e 'print "\x00\x10"' >> /tmp/derp  # PC
perl -e 'print "\x00"x3' >> /tmp/derp   # A/X/Y
perl -e 'print "\x00"' >> /tmp/derp   # PSW
perl -e 'print "\xFF"' >> /tmp/derp   # SP
perl -e 'print "\x00"x5' >> /tmp/derp   # Internal? 
perl -e 'print "\x00"x0x80' >> /tmp/derp   #  DSP registers