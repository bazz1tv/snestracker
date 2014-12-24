
#include <stdio.h>

#include "snes9x.h"
#include "port.h"
#include "apu.h"
#include "soundux.h"
#include "libspc.h"

#define RATE 100

SAPURegisters BackupAPURegisters;
unsigned char BackupAPURAM[65536];
unsigned char BackupAPUExtraRAM[64];
unsigned char BackupDSPRAM[128];

int samples_per_mix;


// The callback

static void DoTimer (void)
{
  APURegisters.PC = IAPU.PC - IAPU.RAM;
//   S9xAPUPackStatus ();

//   if (IAPU.APUExecuting)
//     APU.Cycles -= 90;//Settings.H_Max;
//   else
//     APU.Cycles = 0;

  if (APU.TimerEnabled [2]) {
#if 0 // WinSPC
      APU.Timer [2] ++;
#else // snes9x
      APU.Timer [2] += 4;
#endif
      if (APU.Timer [2] >= APU.TimerTarget [2]) {
	  IAPU.RAM [0xff] = (IAPU.RAM [0xff] + 1) & 0xf;
#if 0  // WinSPC
 	  APU.Timer [2] = 0;
#else  // snes9x
	  APU.Timer [2] -= APU.TimerTarget [2];
#endif
#ifdef SPC700_SHUTDOWN          
	  IAPU.WaitCounter++;
	  IAPU.APUExecuting = TRUE;
#endif          

	}
    }
  if (IAPU.TimerErrorCounter >= 8)
    {
      IAPU.TimerErrorCounter = 0;
      if (APU.TimerEnabled [0])
	{
	  APU.Timer [0]++;
	  if (APU.Timer [0] >= APU.TimerTarget [0])
	    {
	      IAPU.RAM [0xfd] = (IAPU.RAM [0xfd] + 1) & 0xf;
	      APU.Timer [0] = 0;
#ifdef SPC700_SHUTDOWN          
	      IAPU.WaitCounter++;
	      IAPU.APUExecuting = TRUE;
#endif          

	    }
	}
      if (APU.TimerEnabled [1])
	{
	  APU.Timer [1]++;
	  if (APU.Timer [1] >= APU.TimerTarget [1])
	    {
	      IAPU.RAM [0xfe] = (IAPU.RAM [0xfe] + 1) & 0xf;
	      APU.Timer [1] = 0;
#ifdef SPC700_SHUTDOWN          
	      IAPU.WaitCounter++;
	      IAPU.APUExecuting = TRUE;
#endif          
	    }
	}
    }
}

/* ================================================================ */

START_EXTERN_C

int SPC_init(SPC_Config *cfg)
{
    return SPC_set_state(cfg);
}

void SPC_close(void)
{
    S9xDeinitAPU();
}

int SPC_set_state(SPC_Config *cfg)
{
    int i;

    Settings.APUEnabled = TRUE;
    Settings.InterpolatedSound = (cfg->is_interpolation) ? TRUE : FALSE;
    Settings.SoundEnvelopeHeightReading = TRUE;
    Settings.DisableSoundEcho = (cfg->is_echo) ? FALSE : TRUE;
    //   Settings.EnableExtraNoise = TRUE;
	Settings.ReverseStereo = 0;
	Settings.AltSampleDecode = 0;

    // SPC mixer information
    //samples_per_mix = cfg->sampling_rate / RATE * cfg->channels;
    samples_per_mix = cfg->sampling_rate / RATE * cfg->channels;
    so.playback_rate = cfg->sampling_rate;
    so.err_rate = (uint32)(SNES_SCANLINE_TIME * 0x10000UL / (1.0 / (double) so.playback_rate));
    S9xSetEchoDelay(APU.DSP [APU_EDL] & 0xf);
    for (i = 0; i < 8; i++)
	S9xSetSoundFrequency(i, SoundData.channels [i].hertz);
    so.buffer_size = samples_per_mix;
    so.stereo = (cfg->channels == 2) ? TRUE : FALSE;

    if (cfg->resolution == 16){
        so.buffer_size *= 2;
        so.sixteen_bit = TRUE;
    } else
        so.sixteen_bit = FALSE;
    
    so.encoded = FALSE;
    so.mute_sound = FALSE;

    return so.buffer_size;
}

/* get samples
   ---------------------------------------------------------------- */
void SPC_update(unsigned char *buf)
{
    // APU_LOOP
    int c, ic;

#if 1
    for (c = 0; c < 2048000 / 32 / RATE; c ++) {
        for (ic = 0; ic < 32; ic ++)
            APU_EXECUTE1(); IAPU.TimerErrorCounter ++; DoTimer();
    }
#else
    for (APU.Cycles = 0; APU.Cycles < 204800 / RATE; APU.Cycles ++) {
        APU_EXECUTE1();
        ++ IAPU.TimerErrorCounter;
        if ((IAPU.TimerErrorCounter & 31) == 0)
            DoTimer();
        APURegisters.PC = IAPU.PC - IAPU.RAM;
        S9xAPUPackStatus();
    }
#endif

    S9xMixSamples ((unsigned char *)buf, samples_per_mix);
}

/* Restore SPC state
   ---------------------------------------------------------------- */
static void RestoreSPC()
{
    int i;

    APURegisters.PC = BackupAPURegisters.PC;
    APURegisters.YA.B.A = BackupAPURegisters.YA.B.A;
    APURegisters.X = BackupAPURegisters.X;
    APURegisters.YA.B.Y = BackupAPURegisters.YA.B.Y;
    APURegisters.P = BackupAPURegisters.P;
    APURegisters.S = BackupAPURegisters.S;
    memcpy (IAPU.RAM, BackupAPURAM, 65536);
    memcpy (APU.ExtraRAM, BackupAPUExtraRAM, 64);
    memcpy (APU.DSP, BackupDSPRAM, 128);

    for (i = 0; i < 4; i ++)
    {
        APU.OutPorts[i] = IAPU.RAM[0xf4 + i];
    }
    IAPU.TimerErrorCounter = 0;

    for (i = 0; i < 3; i ++)
    {
        if (IAPU.RAM[0xfa + i] == 0)
            APU.TimerTarget[i] = 0x100;
        else
            APU.TimerTarget[i] = IAPU.RAM[0xfa + i];
    }

    S9xSetAPUControl (IAPU.RAM[0xf1]);

  /* from snaporig.cpp (ReadOrigSnapshot) */
    S9xSetSoundMute (FALSE);
    IAPU.PC = IAPU.RAM + APURegisters.PC;
    S9xAPUUnpackStatus ();
    if (APUCheckDirectPage ())
        IAPU.DirectPage = IAPU.RAM + 0x100;
    else
        IAPU.DirectPage = IAPU.RAM;
    Settings.APUEnabled = TRUE;
    IAPU.APUExecuting = TRUE;

    S9xFixSoundAfterSnapshotLoad ();

    S9xSetFrequencyModulationEnable (APU.DSP[APU_PMON]);
    S9xSetMasterVolume (APU.DSP[APU_MVOL_LEFT], APU.DSP[APU_MVOL_RIGHT]);
    S9xSetEchoVolume (APU.DSP[APU_EVOL_LEFT], APU.DSP[APU_EVOL_RIGHT]);

    uint8 mask = 1;
    int type;
    for (i = 0; i < 8; i++, mask <<= 1) {
        Channel *ch = &SoundData.channels[i];
      
        S9xFixEnvelope (i,
                        APU.DSP[APU_GAIN + (i << 4)],
                        APU.DSP[APU_ADSR1 + (i << 4)],
                        APU.DSP[APU_ADSR2 + (i << 4)]);
        S9xSetSoundVolume (i,
                           APU.DSP[APU_VOL_LEFT + (i << 4)],
                           APU.DSP[APU_VOL_RIGHT + (i << 4)]);
        S9xSetSoundFrequency (i, ((APU.DSP[APU_P_LOW + (i << 4)]
                                   + (APU.DSP[APU_P_HIGH + (i << 4)] << 8))
                                  & FREQUENCY_MASK) * 8);
        if (APU.DSP [APU_NON] & mask)
            type = SOUND_NOISE;
        else
            type = SOUND_SAMPLE;
        S9xSetSoundType (i, type);
        if ((APU.DSP[APU_KON] & mask) != 0)
	{
            APU.KeyedChannels |= mask;
            S9xPlaySample (i);
	}
    }

#if 0
    unsigned char temp=IAPU.RAM[0xf2];
    for(i=0;i<128;i++){
        IAPU.RAM[0xf2]=i;
        S9xSetAPUDSP(APU.DSP[i]);
    }
    IAPU.RAM[0xf2]=temp;
#endif
}

/* Load SPC file [CLEANUP]
   ---------------------------------------------------------------- */
int SPC_load (const char *fname)
{
    FILE *fp;
    char temp[64];

    S9xInitAPU();

    fp = fopen (fname, "rb");
    if (!fp)
        return FALSE;
 
	if (!SPC_loadFP(fp)) { 
    	fclose(fp);
		return FALSE;
	}
	
    fclose(fp);
    return TRUE;
}

int SPC_loadFP(FILE *fp)
{
    char temp[64];
	unsigned char a, b;

    S9xInitAPU();

    if (!fp)
        return FALSE;
  
    S9xResetAPU();

    fseek(fp, 0x25, SEEK_SET);
   
	fread(&a, 1, 1, fp);
	fread(&b, 1, 1, fp);
	BackupAPURegisters.PC = (b << 8) | a;
//	fread(&BackupAPURegisters.PC, 1, 2, fp);
	
    fread(&BackupAPURegisters.YA.B.A, 1, 1, fp);
    fread(&BackupAPURegisters.X, 1, 1, fp);
    fread(&BackupAPURegisters.YA.B.Y, 1, 1, fp);
    fread(&BackupAPURegisters.P, 1, 1, fp);
    fread(&BackupAPURegisters.S, 1, 1, fp);

    fseek(fp, 0x100, SEEK_SET);
    fread(BackupAPURAM, 1, 0x10000, fp);
    fread(BackupDSPRAM, 1, 128, fp);
    fread(temp, 1, 64, fp);
    fread(BackupAPUExtraRAM, 1, 64, fp);

    RestoreSPC();
    IAPU.OneCycle = ONE_APU_CYCLE;

    return TRUE;
}


/* ID666
   ---------------------------------------------------------------- */
SPC_ID666 *SPC_get_id666 (const char *filename)
{
  FILE *fp;
  SPC_ID666 *id666;

  fp = fopen(filename, "rb");
  if (fp == NULL) {
      return NULL;
  }

  id666 = SPC_get_id666FP(fp);

  fclose(fp);

  return id666;
}

/* Binary tags only!!! */
SPC_ID666 *SPC_get_id666FP (FILE *fp)
{
  SPC_ID666 *id;
  unsigned char playtime_str[4] = { 0, 0, 0, 0 };
  unsigned char fadetime_str[5] = { 0, 0, 0, 0, 0 };
 
  
  id = (SPC_ID666 *)malloc(sizeof(*id));
  if (id == NULL)
    return NULL;
  
  fseek(fp, 0x23, SEEK_SET);
  if (fgetc(fp) == 27) {
      //fclose(fp);
      free(id);
      return NULL;
  }

  fseek(fp, 0x2E, SEEK_SET);
  fread(id->songname, 1, 32, fp);
  id->songname[33] = '\0';

  fread(id->gametitle, 1, 32, fp);
  id->gametitle[33] = '\0';

  fread(id->dumper, 1, 16, fp);
  id->dumper[17] = '\0';

  fread(id->comments, 1, 32, fp);
  id->comments[33] = '\0';

  fseek(fp, 0xA9, SEEK_SET);
  fread(playtime_str, 1, 3, fp);
  id->playtime = atoi((char*)playtime_str);
 
  fseek(fp, 0xD1, SEEK_SET);
  switch (fgetc (fp)) {
  case 1:
      id->emulator = SPC_EMULATOR_ZSNES;
      break;
  case 2:
      id->emulator = SPC_EMULATOR_SNES9X;
      break;
  case 0:
  default:
      id->emulator = SPC_EMULATOR_UNKNOWN;
      break;
  }

  fseek(fp, 0xB0, SEEK_SET);
  fread(id->author, 1, 32, fp);
  id->author[33] = '\0';

  return id;
}  

int SPC_write_id666 (SPC_ID666 *id, const char *filename)
{
  FILE *fp;
  int spc_size;
  unsigned char *spc_buf;

  if (id == NULL)
    return FALSE;

  fp = fopen (filename, "rb");
  if (fp == NULL)
    return FALSE;

  fseek(fp, 0, SEEK_END);
  spc_size = ftell(fp);

  spc_buf = (unsigned char *)malloc(spc_size);
  if (spc_buf == NULL) {
      fclose (fp);
      return FALSE;
  }

  fread(spc_buf, 1, spc_size, fp);
  fclose(fp);

  if (*(spc_buf + 0x23) == 27)
    {
      free(spc_buf);
      return FALSE;
    }
  
  memset(spc_buf + 0x2E, 0, 119);
  memset(spc_buf + 0xA9, 0, 38);
  memset(spc_buf + 0x2E, 0, 36);
  
  memcpy(spc_buf + 0x2E, id->songname, 32);
  memcpy(spc_buf + 0x4E, id->gametitle, 32);
  memcpy(spc_buf + 0x6E, id->dumper, 16);
  memcpy(spc_buf + 0x7E, id->comments, 32);
  memcpy(spc_buf + 0xB0, id->author, 32);

  spc_buf[0xD0] = 0;
  
  switch (id->emulator) {
  case SPC_EMULATOR_UNKNOWN:
      *(spc_buf + 0xD1) = 0;
      break;
  case SPC_EMULATOR_ZSNES:
      *(spc_buf + 0xD1) = 1;
      break;
  case SPC_EMULATOR_SNES9X:
      *(spc_buf + 0xD1) = 2;
      break;
  }
  
  fp = fopen(filename, "wb");
  if (fp == NULL) {
      free(spc_buf);
      return FALSE;
  }

  fwrite(spc_buf, 1, spc_size, fp);
  fclose(fp);

  return TRUE;
}

END_EXTERN_C
