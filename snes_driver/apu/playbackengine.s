; WARNING : Be careful about the DIV YA,X opcode. See FULLSNES.htm.
; Ensure results will be < 0x200
.include "apu/memorymap.i"
.include "apu/regdefs.i"
.include "apu/enums.i"
.include "apu/playbackengine.i"
; Zero page variables!
.RAMSECTION "gp-dp0" BANK 0 SLOT SPC_DP0_SLOT
spddec db ; copy of spd to dec from
inc_to_patlen db ; " patternlen_rows

pattern_ptr dw  ; ptr to the current pattern, maybe only l ram needed)
patternlen_rows db ; 00 == 256

ptrack_ptrs dsw 8 ; pointers to each track data for a pattern
ptrack_ptr dw
rlecounters dsb 8

konbuf db
koffbuf db ; not sure if needed
echobuf db

; WARNING : keep the public stuff out of address 0000 and 0001 because
; that is the jump address stored from IPL ROM
;curtrack db ; plenty of wasted bits that could be used as bit flags here
/* "Public" RAM made aware to PC tracker */
PUBLIC_START  dsb 0
ticks         db    ; 125 uS increments to timer 0
spd           db
dspdir_i      db

instrtable_ptr dw

sequencer_ptr dw  ; points to sequencer table
sequencer_i db ; index into the sequencer table

patterntable_ptr  dw ; pattern table turns pattern index into pattern address

extflags db
startPatRow db        ; Only used when StartFromPlayhead extflag is active

; SongSettings vals
mvol_val db
evol_val db
esa_val db
edl_val db

efb_val db
; keep these 8 together (iterated)
c0_val db
c1_val db
c2_val db
c3_val db
c4_val db
c5_val db
c6_val db
c7_val db
PUBLIC_END    dsb 0

activeInstrument  dsb 8

;CurTrackData
note_pitch          dw  ; workable copy of the actual pitch value to be stored to DSP
note_idx            db  ; tracker's note index
note_octave         db
note_idx_mod12      db  ; tracker's note % 12
;EndCurTrackData

.ENDS

.RAMSECTION "math-dp0" BANK 0 SLOT SPC_DP0_SLOT
multiplicand  dsb 3 ; 8.16 format
multiplier    dsb 3 ; 8.16 format
result        dsb 5 ; 40-bits
shiftbuf      dsb 5 ; multiplicand shift buffer (detailed in HOW IT WORKS section below)
.ENDS

.BANK 0 SLOT SPC_CODE_SLOT
.ORG 0
.SECTION "Playback Engine Code" SEMIFREE

ReloadSpdDec:
  mov a, spd
  mov spddec, a
  ret

; IN: (X) current track idx
; CLOBS: (A)
Loadptrack_ptr:
  push x
    mov a, x  ; move track# into A as arg
    asl a
    clrc
    adc a, #ptrack_ptrs
    mov x,a
    mov a,(X)+
    mov ptrack_ptr, a
    mov a,(X)+
    mov ptrack_ptr+1, a
  pop x
  ret

.define rlecounter rlecounters
; Expects <pattern_ptr> to be loaded and valid
; IN: NONE
; OUT: YA = address of next ptrack
; CLOBBERS: A,X,Y
;           ptrack_ptr
;           l
;           rlecounter(s)
QuickReadPTrack:
  mov rlecounter, #0
  mov x, patternlen_rows
  ;--- check if we are in an rlecount
@loop:
  mov y, #0
  mov a, rlecounter
  beq @norle
  dec rlecounter
  bpl @done_read_row
@norle:
  mov a, [ptrack_ptr] + y
  mov l, a
  ; if negative, then we have compression on this row
  bpl @nocomp ; iow, all "normal" rows will be read
  inc y

@@test_cbnote:
  bbc l.CBIT_NOTE, @@test_cbinstr
  inc y
@@test_cbinstr:
  bbc l.CBIT_INSTR, @@test_cbvol
  inc y
@@test_cbvol:
  bbc l.CBIT_VOL, @@test_cbfx
  inc y
@@test_cbfx:
  bbc l.CBIT_FX, @@test_cbfxparam
  inc y
@@test_cbfxparam:
  bbc l.CBIT_FXPARAM, @@test_rle
  inc y
@@test_rle:
  bbc l.CBIT_RLE_ONLY1, @@@test_rle_g2
  mov a, #1
  bbc l.CBIT_RLE, @@@store_rle
  inc a
  bra @@@store_rle
@@@test_rle_g2: ; RLE > 2
  bbc l.CBIT_RLE, @done_read_row
  mov a, [ptrack_ptr] + y
  inc y
@@@store_rle:
  mov rlecounter, a
  bra @done_read_row
@nocomp:
  ; read note
  inc y
  ; read inst
  inc y
  ; read vol
  inc y
  ; read fx
  inc y
  ; read fxparam
  inc y
@done_read_row:
  ; to avoid Y from overflowing, rewrite the pointer. We could have
  ; increased performance by doing this conditionally when Y approaches
  ; overflow, but size constraint is important on this platform, so keep
  ; instruction size to a minimum
  mov a,y
  mov y, #0
  addw ya, ptrack_ptr
  movw ptrack_ptr, ya
  dec x
  bne @loop
_ret:
  ; Once we are done it's time to copy the pointer over. That is done from
  ; outside
  ret
.undef rlecounter

; Expects <pattern_ptr> to be loaded and valid
; IN: NONE
; OUT: ptrack_ptrs (8 words)
;      patternlen_rows
; CLOBBERS: A,X,Y
;           ptrack_ptr
Load_ptrack_ptrs:
; Loadpatternlen_rows
  ;mov curtrack, #0
  mov x, #ptrack_ptrs ; track idx MAYBE
  mov y, #0
  mov inc_to_patlen, y
  mov a, [pattern_ptr] + y
  mov patternlen_rows, a

  ; the first track is at pattern_ptr + 1, skipping the row length byte
  mov a, #1
  addw ya, pattern_ptr
  movw ptrack_ptr, ya
@write_ptrack_ptr_loop:
  ; write the location of the next track pattern data (starts at track 0)
  mov (x)+, a
  mov a,y
  mov (x)+, a
  cmp x, #ptrack_ptrs + (8*2) ; all 8 words have been written for 8 tracks
  beq _ret
; could save 2 bytes of codespace here by putting the cmp at the bra
; below, this will cause the last track to be iterated unnecessarily, as
; the only price to pay.
  push x ; save the ptrack_ptrs indexed ptr (points to the current track + 1)
    call !QuickReadPTrack ; YA has curtrack + 1 ptrack ptr
  pop x ; curtrack +1
  bra @write_ptrack_ptr_loop

; Clobbers A,Y
; OUT: A = pattern number
GetPatternNumberFromsequencer_i:
  mov y, sequencer_i
  mov a, [sequencer_ptr] + y
  bpl @not_eos
; restart sequence
  mov y, #0
  mov a, [sequencer_ptr] + y
  mov sequencer_i, y
@not_eos:
  ret

; PREREQS: patterntable_ptr
; IN: A (pattern number)
; CLOBBERS Y
; OUT: pattern_ptr
; use the pattern number in A, to do a lookup from the patterntable_ptr
Load_pattern_ptr:
  asl a
  mov y, a
  mov a, [patterntable_ptr] + y
  mov pattern_ptr, a ; LO byte of pattern addr
  inc y
  mov a, [patterntable_ptr] + y
  mov pattern_ptr+1, a ; HI byte of pattern addr
  ret

; A precursor to LoadPattern (unused so far)
NextPattern:
  bbs extflags.REPEATPATTERN, LoadPattern
  inc sequencer_i
LoadPattern:
  call !ReloadSpdDec

  ; assume sequencer_i has been loaded from Tracker
  call !GetPatternNumberFromsequencer_i
  ; A = pattern number
  call !Load_pattern_ptr

  ; Now we read through the pattern data to load up the Track pattern
  ; addresses
  call !Load_ptrack_ptrs

  ; Readahead-check when crossing between patterns
  mov x, #7
- call !Loadptrack_ptr
  call !readahead ; will "plant" a future tick KOFF if note encountered
                  ; reset the 8 voice rle counters (stale values can cause playback order issues)
  mov a, #0
  mov rlecounters + x, a
  dec x
  bpl -

  ; New Feature : StartFromPlayhead
  bbc extflags.START_FROM_PLAYHEAD, @noStartFromPlayhead
  ; Read pattern rows until we arrive at the desired pattern
  mov dspaddr, #koff
  mov dspdata, #$FF
- cmp startPatRow, inc_to_patlen
  beq +
  call !ReadPTracks ; The idea is to speed through playback without actually writing to DSP
  bra -
+ ;clr1 extflags.START_FROM_PLAYHEAD
  mov konbuf, #0
  mov dspaddr, #koff
  mov dspdata, #$FF
  bbs extflags.REPEATPATTERN, +
  clr1 extflags.START_FROM_PLAYHEAD ; If we are not repeating the pattern, unset this flag
                                    ; so the rest of the song continues as normal (upon pattern reload)
+
@noStartFromPlayhead

__ret:
  ret

ContinueSong:
  ; We need to check the timer for a start of row. iow, first check that a
  ; timer tick has occurred (and how many). decrement that value from the
  ; spddec until hits zero (or negative if u want to check at top of
  ; loop); I like the negative one because it safeguards against potential
  ; overflows.
  mov a, spddec
  setc
  sbc a, t0out
  ; has even 1 tick passed?
  cmp a, spddec
  beq __ret

; 1 tick HAS passed
  mov spddec, a
  dec a
  bpl +  ; this essentially gives if (a < 0) mark next row for processing
  set1 flags.bNextRow ; set the flag indicating we will process the next row
  bra @onlyfx
+ ; at this point, a tick has been fired
  ; TODO: impl fx and stuff
  mov a, koffbuf
  beq @no_koff
  call !calc_kofftick
  cmp a, spddec; , kofftick
  bne @no_koff
@koff:
  mov dspaddr, #koff
  mov a, koffbuf
  mov dspdata, a
  mov koffbuf, #0
@no_koff:

@onlyfx:
  ; do FX processing here on every tick

  bbc flags.bNextRow, __ret2
@nextrow:
  clr1 flags.bNextRow
  call !ReloadSpdDec
  ; If it's the start of a new row call the routine to read the
  ; next row man!
  mov konbuf, #0
  call !ReadPTracks

HackishPlayInstrumentFunctionality:
  mov a, echobuf
  mov dspaddr, #eon
  mov dspdata, a

  mov a, konbuf
  beq @no_kon
  mov dspaddr, #koff
  mov dspdata, #0
  mov dspaddr, #kon
  mov dspdata, a
  ; TODO: KOFF
@no_kon:
__ret2:
  ret

; IN: X=vnum (0-7)
; OUT: A=vbit (1<<[0-7])
; CLOBBERS: A
VoiceNumToVoiceBit:
  push x
    mov a, #0
    setc
@bitshift:
    rol a
    dec x
    bpl @bitshift
  pop x
  ret


/* 12-Tone Equal Temperament (12-TET) (ET) is the most popular method of
intervalizing an octave. The octave is split into twelve equally distant intervals.
This system is a simple compromise between the ability to modulate keys vs. having perfect
tuning, though our ears have grown quite accustomed to the sound.
An example of instruments using ET is the piano and guitar.

ET establishes a constant ratio between all 12 steps of octave. Because musical
notes are built by a ratio, the distance between each note expands as you move
upward and contracts as you move downward. This means the math used to make adjustments
upon any note cannot be linear (eg. fixed offsets).

Every increasing octave has double the resolution of the last.
The distance between each octave doubles!  0-1--2---3----4-----5------6-------7
Be sure to understand that this distance growth is not only per octave, the growth
is happening per note!

ET table can be uncovered most simply by obtaining the twelth root of 2. 2^(1/12)

For an individual octave:
00 1.0
01 1.059463094359295
02 1.122462048309372
03 1.18920711500272 
04 1.259921049894872
05 1.334839854170033
06 1.414213562373093
07 1.498307076876679
08 1.587401051968196
09 1.681792830507425
10 1.781797436280674
11 1.887748625363382
12 2

*/

/*
Fixed Point Math
----------------
Starting to wrap my head around the 8.16 16.16 fixed point math. Here goes an explanation.

To scale decimal values from 0.0 to 1.0 in a 16-bit window, you use all 16-bits,
that's 65536 values to represent 0 to 1.
this means that
0x8000 is .5 and 0x0001 is 1/65636 == 0.000015258789063

The values useful for semi-tones and cents are
semi-tone (st): the twelvth root of 2 or 2^(1/12) == 1.059463094359295
cent       (c): the 100th root of the twelve root of 2
      2^(1/12 * 1/100) => 2^(1/1200) => 1.000577789506555

To express these numbers in .16 fixed point:

semi-tone: st / ( 1 / 65536 ) => 69432.97335193075712 => 69433 => 0x10f39
cent:       c / ( 1 / 65536 ) => 65573.86601310158848 => 65574 => 0x10026

Let's get the rest of the semi-tone steps 2-11:
*/
ET12MultiplicandLUT:
.dw 0
/*note[1]:  1.059463094359295 : HEX(.16 fixed): $10f39 */ .dw $0f39
/*note[2]:  1.122462048309373 : HEX(.16 fixed): $11f5a */ .dw $1f5a
/*note[3]:  1.189207115002721 : HEX(.16 fixed): $13070 */ .dw $3070
/*note[4]:  1.259921049894873 : HEX(.16 fixed): $1428a */ .dw $428a
/*note[5]:  1.334839854170035 : HEX(.16 fixed): $155b8 */ .dw $55b8
/*note[6]:  1.414213562373095 : HEX(.16 fixed): $16a0a */ .dw $6a0a
/*note[7]:  1.498307076876682 : HEX(.16 fixed): $17f91 */ .dw $7f91
/*note[8]:  1.5874010519682   : HEX(.16 fixed): $19660 */ .dw $9660
/*note[9]:  1.68179283050743  : HEX(.16 fixed): $1ae8a */ .dw $ae8a
/*note[10]: 1.781797436280679 : HEX(.16 fixed): $1c824 */ .dw $c824
/*note[11]: 1.887748625363388 : HEX(.16 fixed): $1e343 */ .dw $e343

CentMultiplicandLUT:
.dw 0
/*cent[1]:  1.000577789506555 : HEX(.16 fixed): 0x10026 */ .dw $0026
/*cent[2]:  1.001155912853824 : HEX(.16 fixed): 0x1004c */ .dw $004c
/*cent[3]:  1.001734370234696 : HEX(.16 fixed): 0x10072 */ .dw $0072
/*cent[4]:  1.002313161842173 : HEX(.16 fixed): 0x10098 */ .dw $0098
/*cent[5]:  1.002892287869367 : HEX(.16 fixed): 0x100be */ .dw $00be
/*cent[6]:  1.003471748509503 : HEX(.16 fixed): 0x100e4 */ .dw $00e4
/*cent[7]:  1.004051543955916 : HEX(.16 fixed): 0x1010a */ .dw $010a
/*cent[8]:  1.004631674402054 : HEX(.16 fixed): 0x10130 */ .dw $0130
/*cent[9]:  1.005212140041476 : HEX(.16 fixed): 0x10156 */ .dw $0156
/*cent[10]: 1.005792941067853 : HEX(.16 fixed): 0x1017c */ .dw $017c
/*cent[11]: 1.006374077674969 : HEX(.16 fixed): 0x101a2 */ .dw $01a2
/*cent[12]: 1.006955550056719 : HEX(.16 fixed): 0x101c8 */ .dw $01c8
/*cent[13]: 1.007537358407109 : HEX(.16 fixed): 0x101ee */ .dw $01ee
/*cent[14]: 1.008119502920258 : HEX(.16 fixed): 0x10214 */ .dw $0214
/*cent[15]: 1.008701983790399 : HEX(.16 fixed): 0x1023a */ .dw $023a
/*cent[16]: 1.009284801211874 : HEX(.16 fixed): 0x10260 */ .dw $0260
/*cent[17]: 1.00986795537914  : HEX(.16 fixed): 0x10287 */ .dw $0287
/*cent[18]: 1.010451446486764 : HEX(.16 fixed): 0x102ad */ .dw $02ad
/*cent[19]: 1.011035274729427 : HEX(.16 fixed): 0x102d3 */ .dw $02d3
/*cent[20]: 1.011619440301922 : HEX(.16 fixed): 0x102f9 */ .dw $02f9
/*cent[21]: 1.012203943399156 : HEX(.16 fixed): 0x10320 */ .dw $0320
/*cent[22]: 1.012788784216145 : HEX(.16 fixed): 0x10346 */ .dw $0346
/*cent[23]: 1.013373962948022 : HEX(.16 fixed): 0x1036c */ .dw $036c
/*cent[24]: 1.013959479790029 : HEX(.16 fixed): 0x10393 */ .dw $0393
/*cent[25]: 1.014545334937523 : HEX(.16 fixed): 0x103b9 */ .dw $03b9
/*cent[26]: 1.015131528585974 : HEX(.16 fixed): 0x103e0 */ .dw $03e0
/*cent[27]: 1.015718060930964 : HEX(.16 fixed): 0x10406 */ .dw $0406
/*cent[28]: 1.016304932168189 : HEX(.16 fixed): 0x1042d */ .dw $042d
/*cent[29]: 1.016892142493455 : HEX(.16 fixed): 0x10453 */ .dw $0453
/*cent[30]: 1.017479692102686 : HEX(.16 fixed): 0x1047a */ .dw $047a
/*cent[31]: 1.018067581191916 : HEX(.16 fixed): 0x104a0 */ .dw $04a0
/*cent[32]: 1.018655809957292 : HEX(.16 fixed): 0x104c7 */ .dw $04c7
/*cent[33]: 1.019244378595076 : HEX(.16 fixed): 0x104ed */ .dw $04ed
/*cent[34]: 1.019833287301644 : HEX(.16 fixed): 0x10514 */ .dw $0514
/*cent[35]: 1.020422536273482 : HEX(.16 fixed): 0x1053a */ .dw $053a
/*cent[36]: 1.021012125707193 : HEX(.16 fixed): 0x10561 */ .dw $0561
/*cent[37]: 1.021602055799492 : HEX(.16 fixed): 0x10588 */ .dw $0588
/*cent[38]: 1.022192326747208 : HEX(.16 fixed): 0x105ae */ .dw $05ae
/*cent[39]: 1.022782938747283 : HEX(.16 fixed): 0x105d5 */ .dw $05d5
/*cent[40]: 1.023373891996775 : HEX(.16 fixed): 0x105fc */ .dw $05fc
/*cent[41]: 1.023965186692853 : HEX(.16 fixed): 0x10623 */ .dw $0623
/*cent[42]: 1.024556823032801 : HEX(.16 fixed): 0x10649 */ .dw $0649
/*cent[43]: 1.025148801214019 : HEX(.16 fixed): 0x10670 */ .dw $0670
/*cent[44]: 1.025741121434017 : HEX(.16 fixed): 0x10697 */ .dw $0697
/*cent[45]: 1.026333783890424 : HEX(.16 fixed): 0x106be */ .dw $06be
/*cent[46]: 1.026926788780978 : HEX(.16 fixed): 0x106e5 */ .dw $06e5
/*cent[47]: 1.027520136303536 : HEX(.16 fixed): 0x1070c */ .dw $070c
/*cent[48]: 1.028113826656066 : HEX(.16 fixed): 0x10732 */ .dw $0732
/*cent[49]: 1.028707860036652 : HEX(.16 fixed): 0x10759 */ .dw $0759
/*cent[50]: 1.029302236643492 : HEX(.16 fixed): 0x10780 */ .dw $0780
/*cent[51]: 1.029896956674898 : HEX(.16 fixed): 0x107a7 */ .dw $07a7
/*cent[52]: 1.030492020329297 : HEX(.16 fixed): 0x107ce */ .dw $07ce
/*cent[53]: 1.031087427805232 : HEX(.16 fixed): 0x107f5 */ .dw $07f5
/*cent[54]: 1.031683179301358 : HEX(.16 fixed): 0x1081c */ .dw $081c
/*cent[55]: 1.032279275016448 : HEX(.16 fixed): 0x10843 */ .dw $0843
/*cent[56]: 1.032875715149386 : HEX(.16 fixed): 0x1086b */ .dw $086b
/*cent[57]: 1.033472499899175 : HEX(.16 fixed): 0x10892 */ .dw $0892
/*cent[58]: 1.03406962946493  : HEX(.16 fixed): 0x108b9 */ .dw $08b9
/*cent[59]: 1.034667104045882 : HEX(.16 fixed): 0x108e0 */ .dw $08e0
/*cent[60]: 1.035264923841377 : HEX(.16 fixed): 0x10907 */ .dw $0907
/*cent[61]: 1.035863089050877 : HEX(.16 fixed): 0x1092e */ .dw $092e
/*cent[62]: 1.036461599873958 : HEX(.16 fixed): 0x10956 */ .dw $0956
/*cent[63]: 1.037060456510312 : HEX(.16 fixed): 0x1097d */ .dw $097d
/*cent[64]: 1.037659659159747 : HEX(.16 fixed): 0x109a4 */ .dw $09a4
/*cent[65]: 1.038259208022184 : HEX(.16 fixed): 0x109cb */ .dw $09cb
/*cent[66]: 1.038859103297664 : HEX(.16 fixed): 0x109f3 */ .dw $09f3
/*cent[67]: 1.039459345186338 : HEX(.16 fixed): 0x10a1a */ .dw $0a1a
/*cent[68]: 1.040059933888477 : HEX(.16 fixed): 0x10a41 */ .dw $0a41
/*cent[69]: 1.040660869604466 : HEX(.16 fixed): 0x10a69 */ .dw $0a69
/*cent[70]: 1.041262152534805 : HEX(.16 fixed): 0x10a90 */ .dw $0a90
/*cent[71]: 1.041863782880113 : HEX(.16 fixed): 0x10ab8 */ .dw $0ab8
/*cent[72]: 1.04246576084112  : HEX(.16 fixed): 0x10adf */ .dw $0adf
/*cent[73]: 1.043068086618677 : HEX(.16 fixed): 0x10b07 */ .dw $0b07
/*cent[74]: 1.043670760413748 : HEX(.16 fixed): 0x10b2e */ .dw $0b2e
/*cent[75]: 1.044273782427413 : HEX(.16 fixed): 0x10b56 */ .dw $0b56
/*cent[76]: 1.04487715286087  : HEX(.16 fixed): 0x10b7d */ .dw $0b7d
/*cent[77]: 1.045480871915432 : HEX(.16 fixed): 0x10ba5 */ .dw $0ba5
/*cent[78]: 1.046084939792528 : HEX(.16 fixed): 0x10bcc */ .dw $0bcc
/*cent[79]: 1.046689356693706 : HEX(.16 fixed): 0x10bf4 */ .dw $0bf4
/*cent[80]: 1.047294122820626 : HEX(.16 fixed): 0x10c1b */ .dw $0c1b
/*cent[81]: 1.047899238375068 : HEX(.16 fixed): 0x10c43 */ .dw $0c43
/*cent[82]: 1.048504703558928 : HEX(.16 fixed): 0x10c6b */ .dw $0c6b
/*cent[83]: 1.049110518574218 : HEX(.16 fixed): 0x10c93 */ .dw $0c93
/*cent[84]: 1.049716683623066 : HEX(.16 fixed): 0x10cba */ .dw $0cba
/*cent[85]: 1.050323198907719 : HEX(.16 fixed): 0x10ce2 */ .dw $0ce2
/*cent[86]: 1.050930064630539 : HEX(.16 fixed): 0x10d0a */ .dw $0d0a
/*cent[87]: 1.051537280994006 : HEX(.16 fixed): 0x10d32 */ .dw $0d32
/*cent[88]: 1.052144848200715 : HEX(.16 fixed): 0x10d59 */ .dw $0d59
/*cent[89]: 1.052752766453382 : HEX(.16 fixed): 0x10d81 */ .dw $0d81
/*cent[90]: 1.053361035954835 : HEX(.16 fixed): 0x10da9 */ .dw $0da9
/*cent[91]: 1.053969656908023 : HEX(.16 fixed): 0x10dd1 */ .dw $0dd1
/*cent[92]: 1.054578629516012 : HEX(.16 fixed): 0x10df9 */ .dw $0df9
/*cent[93]: 1.055187953981983 : HEX(.16 fixed): 0x10e21 */ .dw $0e21
/*cent[94]: 1.055797630509237 : HEX(.16 fixed): 0x10e49 */ .dw $0e49
/*cent[95]: 1.056407659301191 : HEX(.16 fixed): 0x10e71 */ .dw $0e71
/*cent[96]: 1.057018040561379 : HEX(.16 fixed): 0x10e99 */ .dw $0e99
/*cent[97]: 1.057628774493455 : HEX(.16 fixed): 0x10ec1 */ .dw $0ec1
/*cent[98]: 1.058239861301187 : HEX(.16 fixed): 0x10ee9 */ .dw $0ee9
/*cent[99]: 1.058851301188465 : HEX(.16 fixed): 0x10f11 */ .dw $0f11

/*
It is necessary to use a LUT for all of these values, because they require many fractional
multiplications to be calculated. The CPU will already be occupied with the multiplications
I want to do based on those values, let alone calculating those too!

note: Since the integer number 1 is in every one of these entries, we can remove it from the
actual table entries and specify it only once from code.

Next comes actually computing semi-tone note values, and final note values from a finetune setting.
Thanks to the LUT, the hardest part of the math is removed.

For example let's say we wanted to calculate C#4. First we need to know the base value of that octave, $1000.
And specify it in fixed format: I can specify this as 0x10.0000 (8.16).

The multiplication becomes 0x100000 * 0x010f39. Now normally this would mean (ignoring more LUT optimizations)
ultimately adding 0x10f39 0x100000 times over. But, since the multiplier has no fraction, we can once
again optimize. Just multiply by 0x10 and then there's no need to shift back.

NOTE: The only difference to get a proper multiplication (not optimized due to non-fractional multiplier),
is to change the carry-over addition to go all the way up to the max-bit-range you'll need to cover the
raw math. You need to descern the range of your input multiplicands and multipliers to determine the max
range you'll need to use in your calculations. It could be as high as 64-bits, or higher.

Since all of my needed multiplications are based between 1 and 2, I only need 40-bits,
which can hold two 4.16 numbers multiplied together. To figure that out, simply multiply
together the fixed point representations of the two largest numbers you need to support.
In my case, I knew it was made up of (highest semi-tone * highest cent * highest octave)
First, check the first two:
 0x1e343 * 0x10f11 => 0x1FFB40473 (33 bits)
And then using the >>16 result in the next biggest calculation:
 0x1ffb4 * 0x4000  => (smaller)



NOTE: You can save on bitshifts when multiplying only an integer (no frac).
the (bit-width of the multiplier - 8) is how much will have to shifted right
after the result is calculated.
examples: 0x010f39 * 0x10 => no shift    ==    ( ( 0x010f39 *   0x100000 ) >> 16 )
          0x010f39 * 0x8000 => RSH 8     ==    ( ( 0x010f39 * 0x80000000 ) >> 16 )
          0x010f39 * 0x018000 => RSH 16
Of course, rather than all that shifting manually, since it is a multiple of 8,
just load from the higher RAM address or register.
*/

.equ INT 2    ; byte offset Where the integer starts in the 8.16 number

; == Helper routines ==
;IN: x = number of times to add the shiftbuf to result
addShiftBuf:
@@add
  movw ya, shiftbuf         ; YA = original multiplicand value (decimal)
;-- Decimal Math --
  addw ya, result           ; YA = YA + lowmid
  movw result, ya           ; update the value
;-- Integer Math --
  adc result + 2, shiftbuf + 2 ; A = integer of multiplicand
                            ; Adds integer including carry over from decimal math
                            ; (if it was 16.16 we would need to do this with #0 for higher byte too)
  adc result + 3, shiftbuf + 3
  adc result + 4, shiftbuf + 4

  dec x                       ; Redo the addition "multiplier" amount of times
  bne @@add
  ret

RorNibble5:
  ; Pre-emptively ROR a nibble (4 bits)
  mov x, #4
- clrc
  ror shiftbuf + 4
  ror shiftbuf + 3
  ror shiftbuf + 2
  ror shiftbuf + 1
  ror shiftbuf + 0
  dec x
  bne -
  ret

/*
  Multiplies a 4.16 x 4.16. Note that this equates to a pre-shifted expanded result of
  8.32. This means you could use this routine to do a 7.16 * 1.16, or a
  5.16 * 3.16. I imagine similar flexibility is possible at the fractional level
  but I've never tried it.

  When extending support, please see all relative
  subroutine's adc, incs, and rors, the size of the variables, etc.
  IN: x: multiplier unsigned int8
      ya: multiplier fractional .16
      multiplicand + (0-2): multiplicand 1.16
  OUT: 8.16 rounded result X:YA
*/
Multiply4_16by4_16:
; prep work
  movw multiplier, ya       ; load the provided multiplier into ram
  mov multiplier + INT, x

  mov a, #0                 ; clear the results and multiplicand-shift-buffer
  mov y, a
  movw result, ya           ; clear
  movw result + 2, ya       ; clear
  mov result + 4, a         ; clear
  movw shiftbuf, ya         ; clear

  movw ya, multiplicand     ; YA = original multiplicand value (decimal)
  movw shiftbuf + 2, ya     ; << 16
  mov a, multiplicand + INT
  mov shiftbuf + 4, a       ; Shiftbuf = (Multiplicand << 16)

/* HOW IT WORKS

Fun fact: The original version of the routine would add the multiplicand up 'multiplier' times,
and it was so slow that it would take several seconds.

This next implementation is the heart of why the math is speedy.

Each nibble of the multiplier is looked at, and a bit-shifted representation of
the multiplicand is added [nibble] times to the result. Here's an
example:

(Multiplicand)   (multiplier)        (result)             X:YA
   0x12345     *   0x12345      =  0x14B65F099 >> 16 => 0x14B65

I discovered for myself that left-shifted versions of the multiplier can be used
to bulk up on additions of the multiplicand. It works like this. Every nibble
represents a unit, much like in decimal there is the 10's place, 100's place, etc.
In this case, each unit represents a shift amount.

The 1 in 0x12345 is representing the 0x10000's place, << 16
The 2 in 0x12345 is representing the 0x01000's place, << 12
The 3 in 0x12345 is representing the 0x00100's place, << 8
The 4 in 0x12345 is representing the 0x00010's place, << 4
The 5 in 0x12345 is representing the 0x00001's place, << 0

So that describes the places. Now, iterate through these nibbles like so:

First, have a copy of the multiplicand to do all the bitshifting on. It
must be wide enough for the preshifted result (in my case 40 bits). Let's call it
shiftbuf. Then, begin analyzing the nibbles of the multiplier.

I opted to start from the largest unit starting with a << 16 copy of the multiplicand in my shiftbuf.
(Note: There were no cycles wasted to preshift 16 as you can simply store to higher ram location).
Though I must say, I see no advantage to having done that.

0x1 in 0x12345 is not 0, so add shiftbuf to result 0x1 times.
Shiftbuf >>= 4 (to be consistent with the next nibble)
0x2 in 0x12345 is not 0, so add shiftbuf to result 0x2 times.
Shiftbuf >>= 4
0x3 in 0x12345 is not 0, so add shiftbuf to result 0x3 times.
Shiftbuf >>= 4
0x4 in 0x12345 is not 0, so add shiftbuf to result 0x4 times.
shiftbuf >>= 4
0x5 in 0x12345 is not 0, so add shiftbuf to result 0x5 times.

Then the result will yield 0x14B65F099 (preshifted result). And you can now
do what you need with it. Round it etc.

*/


  mov a, multiplier+2
  and a, #$0f
  beq +                               ; If nibble 0x0F0000 is set, << 16 and add as many times as the nibble is
  mov x, a                            ; X = how many times to add shiftbuf to result
  call !addShiftBuf
+

  call !RorNibble5
  mov a, multiplier+1
  and a, #$f0
  beq @highbyte_highnibble_clear      ; If nibble 0x00F000 is set, << 12 and add as many times as the nibble is
  xcn a                               ; swap nibble
  mov x, a                            ; X = how many times to add shiftbuf to result
  call !addShiftBuf
@highbyte_highnibble_clear
  call !RorNibble5
  mov a, multiplier+1
  and a, #$0f
  beq @highbyte_lownibble_clear       ; If nibble 0x000F00 is set, << 8 and add as many times as the nibble is
  mov x, a                            ; X = how many times to add shiftbuf to result
  call !addShiftBuf
@highbyte_lownibble_clear
  call !RorNibble5
  mov a, multiplier
  and a, #$f0
  beq @lowbyte_highnibble_clear       ; If nibble 0x0000F0 is set, << 4 and add as many times as the nibble is
  xcn a
  mov x, a                            ; X = how many times to add shiftbuf to result
  call !addShiftBuf
@lowbyte_highnibble_clear
  call !RorNibble5
  mov a, multiplier
  and a, #$0f
  beq @lowbyte_lownibble_clear        ; If nibble 0x00000F is set, << 0 and add as many times as the nibble is
  mov x, a                            ; X = how many times to add shiftbuf to result
  call !addShiftBuf
@lowbyte_lownibble_clear
;-- Rounding -- (Could be moved to separate subroutine)
  movw ya, result
  bpl @no_roundup_needed    ; if < 0x80 (0.5), do not round (round down). Else, round up
  incw result + 2            ; Roundup, and carry over if necessary
  bne @@no_carry
  inc result + 4
@@no_carry
@no_roundup_needed          ; rounded result
@done:
  movw ya, result + 2       ; .16
  mov x, result + 4         ; 8i
  ret

;.undef INT


; IN: x = curtrack 
;     note
; CLOBBERS: A, Y
;           note
;           s.0
;           BC (final scaled pitch offset)
;           DE (instrument pointer)
DoFinetune:
  push x  ; curtrack
    ; Load instrument pointer in advance
    mov a, activeInstrument + X
    call !loadInstrPtr  ; then, [DE] -> Instrument

    mov a, note_idx_mod12
    asl a
    mov x, a

    ; Load up the FineTune Value
    mov y, #Instrument.finetune           ; Y: finetune idx
    mov a, [de] + y ; A: finetune value
    bpl @positive
@negative:      ; Use next note down, with reverse-index into centLUT
                ; IOW C-4[-1] == B-3[99]
    dec x       ; next note down
    dec x
    bpl @@no_wrap
    mov x, #11 * 2  ; go from bottom note of octave to top note of lower octave
    dec note_octave ; WARNING: modifying the original note octave reference
@@no_wrap:
    clrc        ; count negatively from 100
    adc a, #100 ; reverse-index into centLUT (-1 Accum => 99 lut index)
@positive:
    
    ; Store the multiplier
    asl a       ; word cent index
    push a
      ; Store the multiplicand
      mov a, !ET12MultiplicandLUT + 1 + x
      mov y, a
      mov a, !ET12MultiplicandLUT + x
      movw multiplicand, ya
      mov multiplicand + INT, #1
    pop x       ; word cent index

    mov a, !CentMultiplicandLUT + 1 + x
    mov y, a
    mov a, !CentMultiplicandLUT + x
    mov x, #1

    call !Multiply4_16by4_16
    movw multiplicand, ya     ; X:YA => 12TET * Finetune. Store as multiplicand for next call
    mov multiplicand + INT, x

    ;get octave multiplier from octave -1 - 6
    /* Eventually SNES Tracker will index its notes values from 0 the negative
     * octaves, which will help octave traversal with a simple LUT */
    mov a, #0
    mov x, note_octave
    bpl @octavePositive
    mov a, #$80
    mov y, #0
    mov x, a
    bra @callMultiply
@octavePositive:
    setc
-   rol a
    dec x
    bpl -
    inc x ; 0
    mov y, a
    mov a, #0 ; X:YA = 0.octave
              ; Although it is an integer, by putting it in the fraction I am effectively
              ; preshifting the result.
@callMultiply
    call !Multiply4_16by4_16
  ; ---- check the pitch isn't beyond allowable range 3FFF ----
    cmp y, #$40
    bmi @val_ok
    mov y, #$3f
    mov a, #$ff
@val_ok  
    ; It is time to update the pitch values
    movw note_pitch, ya    ; store pitch(LO)
  pop x ; curtrack
  ret

; IN: A = note index
;     X = curtrack
; OUT: updated Y index, Curtrack DSP PITCH written
; CLOBBERS: A, de
ReadNote:
  push x
  push y
    mov y, a
    ; Update the key-on buffer
    call !VoiceNumToVoiceBit
    or a, konbuf
    mov konbuf, a           ; A has bit that this voice represents
    ; -------------------------
    mov a, activeInstrument + x ; The only reason I read the instrument here is
    call !ReadInstr             ; to keep it updated with Tracker interface changes
    ; -------------------------
    mov a, y ; the NOTE number
    mov note_idx, a
    mov x, #12              ; divide by octave
    mov y, #0
    div ya, x
                            ; A = octave. Y = note
    movw note_octave, ya    ; store word to RAM note_octave and note_idx_mod12 (note_octave + 1)
  pop y
  pop x
  ret

; IN: X: curtrack
; OUT: A = DSP base addr for this voice
getDSPIDX:
  mov a, x ; convert curtrack into DSP offset
asl_a_x4:
.rept 4
  asl a  ; curtrack x 0x10
.endr
  ret

; IN:
;     A = instrument number
; OUT: de = pointer to instrument
; CLOBBERS: A, <de>
loadInstrPtr:
  push y
  asl a
  mov y, a  ; Y: idx into instrument table
  mov a, [instrtable_ptr] + y
  mov e, a
  inc y
  mov a, [instrtable_ptr] + y
  mov d, a ; DE -> instrument
  pop y
  ret

; IN: A = instrument number
;     X = curtrack
; OUT: Curtrack DSP written
; CLOBBERS: A, de, c.1, b, fg
ReadInstr:
  push x
  push y
    mov activeInstrument + X, a ; record this track's active instrument number
    ; instrument number MUST BE in A now
    call !loadInstrPtr
    /* TODO: optimize by having a temp-track variable that has this bit value
     * */
    call !VoiceNumToVoiceBit
    mov g, a
    call !getDSPIDX
    push a  ; Dsp Idx
      ; TODO: Implement PAN around here
      mov y, #0 ; reset index for indexing instrument
      mov a, [de] + y ; vol
      ;mov trackVol + x, a
      mov x, a        ; vol backup
      push a          ; vol backup
        clr1 c.1  ; remember if pan was positive or negative. just need a scratch bit
        mov y, #Instrument.pan
        mov a, [de] + y
        bpl +
        set1 c.1
        eor a, #$ff
        inc a
+       mov y, a
        mov a, x    ; Y:A = pan:vol
    ; Calculate DSP volume From the given Pan and Vol settings
    /* putting PAN here will disrupt things when the user has a volume effect column value present
    but I gotta get started somewhere!! */

    /*
      1/64 = 0.015625
      0.015625÷(1÷65536)  = 1024 expressed in .16 fixedpoint

      Effectively doing
              vol        pan  => vol * 0x400 * pan
          ----------  *
              64

      To enjoy the benefits of the mul ya instruction, first do `vol * pan`
      since they are each 8-bits.
    */
                            ;vol * pan
        mul ya
        mov b, y
        asl a
        rol b
        asl a
        rol b
        cmp a, #$80
        bcc @round_down
        inc b
@round_down
      pop a
      mov y, a    ; vol backup xD
      setc
      sbc a, b    ; vol - subtraction value
      bcs +
      mov a, y    ; we overflowed, so put the original volume for an effect of 0 volume
+
    pop x ; NOTE X overwritten here with DSP addr (pushed tho)
    /* Rather than write the volume directly to DSP, save it */
    mov dspaddr, x
    mov dspdata, y
    bbs c.1, +       ; if pan is negative don't write the subtracted value there
    mov dspdata, a
+   inc x           ; vol R
    mov dspaddr, X
    mov dspdata, y
    bbc c.1, +       ; if pan is negative don't write the subtracted value there
    mov dspdata, a
+   inc x           ; pitch (lo) idx
    inc x           ; p-hi
    inc x           ; srcn
    mov dspaddr, x ; srcn
    mov y, #Instrument.srcn
    mov a, [de] + y
    mov dspdata, a ; srcn

    inc x
    mov dspaddr, x ;adsr1
    inc y
    mov a, [de] + y ; adsr1
    mov dspdata, a
    
    inc x
    mov dspaddr, x ;adsr2
    inc y
    mov a, [de] + y ; adsr2
    mov dspdata, a

    inc y           ; echo enable
    mov a, [de] + y
    and a, #INSTR_FLAG_ECHO
    bne +
    mov a, #$FF
    eor a, g
    and a, echobuf
    mov echobuf, a
    bra ++
+   or echobuf, g
++ 

  pop y
  pop x ; track idx
  ret



; IN: <ptrack_ptr>,
;     Y = idx into ptrack_ptr,
;     X = curtrack
; OUT: updated Y index, Curtrack DSP Vol re-written
; TODO: ticks vol FX
; CLOBBERS: A
ReadVol:
  push x
    call !getDSPIDX 
    mov x,a ; NOTE X overwritten here with DSP addr (pushed tho)
    mov a, [ptrack_ptr] + y ; A == vol byte
    inc y
    
    mov dspaddr, x ; Vol_l
    mov dspdata, a
    inc x
    mov dspaddr, x ; vol_r
    mov dspdata, a ; mimic vol to vol_r (pan not yet impl'd)
  pop x
  ret



ReadFx:
  ; TODO
  inc y
  ret
ReadFxParam:
  ; TODO
  inc y
  ret

; IN: X: curtrack
;     [ptrack_ptr]
;     rlecounters
;     inc_to_patlen
;     patternlen_rows
; CLOBBERS: A, Y
readahead:
  mov a, inc_to_patlen
  inc a
  cbne patternlen_rows, +
  ret
+
  mov y, #0
; do a readahead to see if we need to mark KOFF
  mov a, rlecounters + x
  bne @@next_row_is_blank
  mov a, [ptrack_ptr] + y
  bpl @@comp_note ; no compression, go to same routine that has a note
@@comp:
  and a, #1<<CBIT_NOTE
  beq @@next_row_is_blank ; no note, so don't mark koff
@@comp_note:
  ; Time to mark for KOFF
  call !VoiceNumToVoiceBit
  or a, koffbuf
  mov koffbuf, a ; A has bit that this voice represents
@@next_row_is_blank:
@ret
  ret


; Expects <ptrack_ptrs> to be loaded and valid
; IN: inc_to_patlen: start row
;     patternlen_rows: the length of the pattern
; OUT: YA = address of next ptrack
; CLOBBERS: A,X,Y
;           ptrack_ptr
;           l
;           rlecounters
;           patternlen_rows
ReadPTracks:
  mov x, #7 ; put the curtrack into x
  ;--- check if we are in an rlecount
@next_track:
  mov y, #0
  call !Loadptrack_ptr
  mov a, rlecounters + x
  beq @no_active_rle
  dec rlecounters + x
  bpl @track_done
@no_active_rle:
  mov a, [ptrack_ptr] + y
  ; if negative, then we have compression on this row
  bmi @comp
  ; These are the rules that an uncompressed row would follow.
  mov a, #(1<<CBIT) | (1<<CBIT_NOTE) | (1<<CBIT_INSTR) | (1<<CBIT_VOL) | (1<<CBIT_FX) | (1<<CBIT_FXPARAM)
  dec y ; account for lack of compression byte
@comp:
  inc y
  mov l, a
; WARNING: Don't let any of these calls tamper with "l", otherwise
; rewrite all the code
@@test_cbnote:
  bbc l.CBIT_NOTE, @@test_cbinstr
  mov a, [ptrack_ptr] + y     ; instrument number
  inc y
  call !ReadNote
@@test_cbinstr:
  bbc l.CBIT_INSTR, @@test_cbvol
  mov a, [ptrack_ptr] + y     ; instrument number
  inc y
  call !ReadInstr
@@test_cbvol:
  bbc l.CBIT_VOL, @@test_cbfx
  call !ReadVol
@@test_cbfx:
  bbc l.CBIT_FX, @@test_cbfxparam
  call !ReadFx
@@test_cbfxparam:
  bbc l.CBIT_FXPARAM, @@test_rle
  call !ReadFxParam
@@test_rle:
  bbc l.CBIT_RLE_ONLY1, @@@test_rle_g2
  mov a, #1
  bbc l.CBIT_RLE, @@@store_rle
  inc a
  bra @@@store_rle
@@@test_rle_g2: ; RLE > 2
  bbc l.CBIT_RLE, @done_read_row
  mov a, [ptrack_ptr] + y
  inc y
@@@store_rle:
  mov rlecounters + x, a
@done_read_row:
  push x ; save the curtrack
  ; to avoid Y from overflowing, rewrite the pointer. We could have
  ; increased performance by doing this conditionally when Y approaches
  ; overflow, but size constraint is important on this platform, so keep
  ; instruction size to a minimum
    mov a, x
    asl a
    mov x, a
    mov a,y
    mov y, #0
    addw ya, ptrack_ptr
    mov ptrack_ptrs + x, a
    mov ptrack_ptrs + 1 + x, y
  pop x

  call !TrackRowPostProcess
@track_done:
  ; if this is the last row, do not do a readahead (it will be done when
  ; the next pattern is loaded)
  call !readahead
  dec x
  bpl @next_track
@row_done:
  mov a, inc_to_patlen
  inc a
  cbne patternlen_rows, @ret
  call !@report ; report the last row of this pattern
  jmp !NextPattern
@ret:
  mov inc_to_patlen, a
@report:
  dec a ; the tracker should highlight the row that has just been
  ; processed, rather than the next row yet to be processed
  bbc extflags.START_FROM_PLAYHEAD, +
  cmp startPatRow, inc_to_patlen
  bcs @dont_update_tracker_pos
+ mov reportTrackerArg, a
  mov reportTrackerCmd, #reportTrackerCmd_SetRow
  mov a, sequencer_i
  mov reportTrackerArg, a
  mov reportTrackerCmd, #reportTrackerCmd_SetPattern
@dont_update_tracker_pos
  ret

; IN: x = curtrack
;     konbuf = which notes to trigger
; CLOBBERS: a,x,y and check DoFineTune especially
TrackRowPostProcess:
  call !VoiceNumToVoiceBit
  and a, konbuf
  beq @No_Note_For_ThisTrack
  ; Do processing here. Such as finetune or effects
  call !DoFinetune
  ; once processing is done. Load the note into DSP
  call !getDSPIDX
  clrc
  adc a, #plo
  mov dspaddr, a
  mov dspdata, note_pitch
  inc dspaddr
  mov dspdata, note_pitch + 1
@No_Note_For_ThisTrack
  ret

.ends
