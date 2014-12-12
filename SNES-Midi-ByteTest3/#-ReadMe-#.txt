====== SNES - Starter Bundle for Programmers ====== 
 
EDIT: January 8, 2002
Found some code errors ... while I was at it, I included the most
current WLA DX files.

Was told to rename output files to .FIG instead of .SMC because
my "ROM images" didn't have a SMC header ... works either way,
but I agree.  The files should be labelled correctly.

(Technically, .FIG should have a header too ... but: 
"The ROM images produced are identified as Pro FIGhter by 
utilities like uCon64, though, so I'd recommend using that 
extension." - Fancia from the zsnes boards.)


October 31, 2002

I put this together in a hurry, so I'll probably forget a thing
or two.  I appologize in advance.

-neviksti



Okay, a quick rundown.

Most files were created with wordpad, no wrap.
The text may not 'line-up' correctly/nicely with other programs,
so I suggest you use that if its available to you.

-------- Example Programs --------

I included three example programs to get you started.

You can check out the already built ROM images:
Hello.fig
Show.fig
Walker.fig

And then look at the source code:

Hello.asm 
	- This is the standard "Hello World!" application.  
	- It's nice to use this as skeleton code for quick 'test' 
	programs to try out stuff on the SNES.

Show.asm
	- A slide show for the SNES.
	(pictures chosen by Fancia from the zsnes message boards)
	- I included this as it shows off the amazing power of
	WLA's macros.  Notice how easy it is to add more pictures to
	the program.  All you have to do is add a 'IncludePicture FILE'
	and 'IncludePictInfo FILE' line to the current set of pictures.

Walker.asm
	- Just a little guy you can make walk around the world.
	- Just some more graphical example code for you to play with.


------  How to Assemble ----------

First, run "convert_pics.bat" which will use my pcx2snes.exe tool to 
convert the required pictures into the correct format.  Now you're ready
to go.

Example: to compile walker.asm 
At a command prompt, goto the source directory, run the command:
wla walker

This should create "Walker.fig" which you can run with an emulator
or run on the real SNES.


------ WLA DX - Ville Helin's amazing assembler ------

WLA DX homepage:
http://www.hut.fi/~vhelin/wla.html
I compiled and included the latest beta release available
at the time of writing this.  Get newer versions here, or
report bugs/suggest features, etc.  The author is really nice
and quite responsive to feed back.

This is actually a compiler/linker pair and these examples 
don't use it to its potential. 	You can make libraries, etc 
with it and use it to help manage big projects.  Also, the WLA
DX set includes an assembler for the SPC700 so you can write
code for the SNES sound processor.  Check it out if you're
curious.

"wla.txt" - The WLA readme from his site which I've included.
Check this out to see what other features this program has.

- For ROM hackers -
WLA DX can also assemble 'into' a ROM, so you don't
have to copy and paste assembled code.


------- My Tools --------

I included a couple of my tools for you guys:

Pcx2Snes
PcxFix

Both have readme files which I hope are fairly easy to understand.
Check those out before asking questions.

------- A Few More Notes ----------

The PCX files can be changed to change the graphics in the examples.

Just run convert_pics.bat which will use my pcx2snes.exe tool to 
convert the pictures into the correct format.

No emulator is included in this.  I like ZSNES because the authors 
really know what they're doing, and its easy to turn on/off individual 
sound channels, layers, etc. for testing purposes.  Also, the DOS 
version includes a real nice debugger.  Get the windows and DOS 
version here:  www.zsnes.com


I think that's it ... if you have questions, just ask at zsnes.
Its the most efficient way of contacting me.

Have fun,
-kevin
(neviksti)