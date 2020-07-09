; Commands
.enum 0
	NopCmd			    db	; Literally Do Nothing Cmd
	
	ReadRamCmd			db
	WriteRamByteCmd	db
	
	PlaySongCmd			db
	StopSongCmd			db

	SetPatternCmd		db

  PlayInstrumentCmd  db
  CmdEnd          db
.ende

.PRINT "#define SPCCMD_NOP 0x", HEX NopCmd, "\n"
.PRINT "#define SPCCMD_READRAM 0x", HEX ReadRamCmd, "\n"
.PRINT "#define SPCCMD_WRITERAMBYTE 0x", HEX WriteRamByteCmd, "\n"
.PRINT "#define SPCCMD_PLAYSONG 0x", HEX PlaySongCmd, "\n"
.PRINT "#define SPCCMD_STOPSONG 0x", HEX StopSongCmd, "\n"
.PRINT "#define SPCCMD_SETPATTERN 0x", HEX SetPatternCmd, "\n"
.PRINT "#define SPCCMD_PLAYINSTRUMENT 0x", HEX PlayInstrumentCmd, "\n"
