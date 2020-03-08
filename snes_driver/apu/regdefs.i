.enum 0
  ctrlbit_t0 db
  ctrlbit_t1 db
  ctrlbit_t2 db
  ctrlbit_unused db
  ctrlbit_port01 db
  ctrlbit_port23 db
  ctrlbit_unused2 db
  ctrlbit_ramrom db
.ende

.enum $f1
  control  db ; TIMER CONTROL, SNES REG CLEAR
  dspaddr	 db
  dspdata  db
  ;;;;;;;;;;;;;; $f4
  spcport0 db
  spcport1 db
  spcport2 db
  spcport3 db
.ende

.enum $fa
  t0div   db
  t1div   db
  t2div   db

  t0out db
  t1out db
  t2out db
.ende

; =DSP=
; index into any voice dsp reg
.enum 0
  vol_l db
  vol_r db
  plo   db
  phi   db
  srcn  db
  adsr1 db
  adsr2 db
  gain  db
  envx  db
  outx  db
.ende

.enum 0
  voice0_vol_l db
  voice0_vol_r db
  voice0_pitch_lo db
  voice0_pitch_hi db
  voice0_srcn db
  voice0_adsr1 db
  voice0_adsr2 db
  voice0_gain db
  voice0_envx db
  voice0_outx db
  ; a b c d e f
  voice0_pad dsb 6

  voice1_vol_l db
  voice1_vol_r db
  voice1_pitch_lo db
  voice1_pitch_hi db
  voice1_srcn db
  voice1_adsr1 db
  voice1_adsr2 db
  voice1_gain db
  voice1_envx db
  voice1_outx db
  ; a b c d e f
  voice1_pad dsb 6

  voice2_vol_l db
  voice2_vol_r db
  voice2_pitch_lo db
  voice2_pitch_hi db
  voice2_srcn db
  voice2_adsr1 db
  voice2_adsr2 db
  voice2_gain db
  voice2_envx db
  voice2_outx db
  ; a b c d e f
  voice2_pad dsb 6

  voice3_vol_l db
  voice3_vol_r db
  voice3_pitch_lo db
  voice3_pitch_hi db
  voice3_srcn db
  voice3_adsr1 db
  voice3_adsr2 db
  voice3_gain db
  voice3_envx db
  voice3_outx db
  voice3_pad dsb 6

  voice4_vol_l db
  voice4_vol_r db
  voice4_pitch_lo db
  voice4_pitch_hi db
  voice4_srcn db
  voice4_adsr1 db
  voice4_adsr2 db
  voice4_gain db
  voice4_envx db
  voice4_outx db
  voice4_pad dsb 6

  voice5_vol_l db
  voice5_vol_r db
  voice5_pitch_lo db
  voice5_pitch_hi db
  voice5_srcn db
  voice5_adsr1 db
  voice5_adsr2 db
  voice5_gain db
  voice5_envx db
  voice5_outx db
  voice5_pad dsb 6

  voice6_vol_l db
  voice6_vol_r db
  voice6_pitch_lo db
  voice6_pitch_hi db
  voice6_srcn db
  voice6_adsr1 db
  voice6_adsr2 db
  voice6_gain db
  voice6_envx db
  voice6_outx db
  voice6_pad dsb 6

  voice7_vol_l db
  voice7_vol_r db
  voice7_pitch_lo db
  voice7_pitch_hi db
  voice7_srcn db
  voice7_adsr1 db
  voice7_adsr2 db
  voice7_gain db
  voice7_envx db
  voice7_outx db
  voice7_pad dsb 6
.ende

.equ  mvol_l $0c
.equ  mvol_r $1c
.equ  evol_l $2c
.equ  evol_r $3c
.equ  kon $4c
.equ  koff $5c
.equ  flg $6c
.equ  endx $7c

.equ  efb $0d

.equ  pmon $2d
.equ  non $3d
.equ  eon $4d
.equ  dir $5d
.equ  esa $6d
.equ  edl $7d

.equ  c0 $0f
.equ  c1 $1f
.equ  c2 $2f
.equ  c3 $3f
.equ  c4 $4f
.equ  c5 $5f
.equ  c6 $6f
.equ  c7 $7f
