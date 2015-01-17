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

  static const int ATTACK_MAP_SIZE = 16;
  static const int DECAY_MAP_SIZE = 8;
  static const int SUSTAIN_LEVEL_MAP_SIZE = 8;
  static const int SUSTAIN_RELEASE_MAP_SIZE = 0x20;

  static const IntToCharStar attack_map[ATTACK_MAP_SIZE];
  static const IntToCharStar decay_map[DECAY_MAP_SIZE];
  static const IntToCharStar sustain_level_map[SUSTAIN_LEVEL_MAP_SIZE];
  static const IntToCharStar sustain_release_map[SUSTAIN_RELEASE_MAP_SIZE];
};