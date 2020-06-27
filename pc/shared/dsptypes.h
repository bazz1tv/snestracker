#pragma once
#include <stdint.h>

#define SNES_MAX_VOL 0x7f

/* Represents volume level of sample, that is also the raw volume written
 * to SNES APU DSP register */
struct DspVol
{
  uint8_t vol  :7;
  uint8_t phase :1; // sign
} __attribute__((packed));

/* Used for variables that will involve their value written to the SNES
 * APU DSP Pitch register. Even if certain variables are not directly
 * written to the register, it helps to keep them in the same format. */
union DspPitch
{
  uint16_t pitch :14;
  struct {
    uint8_t pitch_l;
    uint8_t pitch_h :6;
  };
} __attribute__((packed));

/* Used for instrument ADSR settings. This data structure is the same
 * format as will be written to the two SNES APU DSP ADSR registers */
union Adsr
{
  // raw adsr reg values
  struct {
    uint8_t sustain_rate :4;
    uint8_t sustain_level :4;
    uint8_t attack_rate :4;
    uint8_t decay_rate :3;
    uint8_t adsr_active :1;
  }__attribute__((packed));
  struct {
    uint8_t adsr1, adsr2;
  }__attribute__((packed));
  uint16_t adsr;
} __attribute__((packed));
