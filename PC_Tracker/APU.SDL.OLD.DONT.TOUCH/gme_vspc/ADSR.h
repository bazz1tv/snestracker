#pragma once
#include "SDL.h"
#include "utility.h"
#include "types.h"

// ADSR Stuffz
struct ADSR
{
  const char * get_attack_str(Uint8 adsr1);
  int get_attack_index(Uint8 adsr1);
  const char * get_decay_str(Uint8 adsr1);   
  int get_decay_index(Uint8 adsr1);
  const char * get_sustain_level_str(Uint8 adsr2);   
  int get_sustain_level_index(Uint8 adsr2); 
  const char * get_sustain_release_str(Uint8 adsr2);  
  int get_sustain_release_index(Uint8 adsr2);

  static const int ATTACK_SIZE = 16;

  static const IntToCharStar attack_map[ATTACK_SIZE];
  static const IntToCharStar decay_map[8];
  static const IntToCharStar sustain_level_map[8];
  static const IntToCharStar sustain_release_map[0x20];
};