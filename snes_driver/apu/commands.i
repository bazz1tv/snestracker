; Commands
.enum 0
	NopCmd			    db	; Literally Do Nothing Cmd
	
	ReadRamCmd			db
	WriteRamByteCmd	db
	
	PlaySongCmd			db
	StopSongCmd			db
  CmdEnd          db
.ende
