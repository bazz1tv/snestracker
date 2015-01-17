#include "ADSR.h"

const IntToCharStar ADSR::attack_map[ATTACK_MAP_SIZE] = 
{
  {0,  "4.1 s"},
  {1,  "2.6 s"},
  {2,  "1.5 s"},
  {3,  "1.0 s"},
  {4,  "640 ms"},
  {5,  "380 ms"},
  {6,  "260 ms"},
  {7,  "160 ms"},
  {8,  "96 ms"},
  {9,  "64 ms"},
  {10, "40 ms"},
  {11, "24 ms"},
  {12, "16 ms"},
  {13, "10 ms"},
  {14, "6 ms"},
  {15, "0 ms"}
};
const IntToCharStar ADSR::decay_map[DECAY_MAP_SIZE] = 
{
  {0, "1.2 s"},
  {1, "740 ms"},
  {2, "440 ms"},
  {3, "290 ms"},
  {4, "180 ms"},
  {5, "110 ms"},
  {6, "74 ms"},
  {7, "37 ms"}
};
const IntToCharStar ADSR::sustain_level_map[SUSTAIN_LEVEL_MAP_SIZE] = 
{
  {0, "1/8"},
  {1, "2/8"},
  {2, "3/8"},
  {3, "4/8"},
  {4, "5/8"},
  {5, "6/8"},
  {6, "7/8"},
  {7, "1"}
};
const IntToCharStar ADSR::sustain_release_map[SUSTAIN_RELEASE_MAP_SIZE] = 
{
  {0,  "Infinite"},
  {1,  "38 s"},
  {2,  "28 s"},
  {3,  "24 s"},
  {4,  "19 s"},
  {5,  "14 s"},
  {6,  "12 s"},
  {7,  "9.4 s"},
  {8,  "7.1 s"},
  {9,  "5.9 s"},
  {10, "4.7 s"},
  {11, "3.5 s"},
  {12, "2.9 s"},
  {13, "2.4 s"},
  {14, "1.8 s"},
  {15, "1.5 s"},
  {16, "1.2 s"},
  {17, "880 ms"},
  {18, "740 ms"},
  {19, "590 ms"},
  {20, "440 ms"},
  {21, "370 ms"},
  {22, "290 ms"},
  {23, "220 ms"},
  {24, "180 ms"},
  {25, "150 ms"},
  {26, "110 ms"},
  {27, "92 ms"},
  {28, "74 ms"},
  {29, "55 ms"},
  {30, "37 ms"},
  {31, "18 ms"}
};

const char * ADSR::get_attack_str(Uint8 adsr1)   
{
  Uint8 attack = adsr1 & 0x0f;
  return attack_map[attack].str;
}
int ADSR::get_attack_index(Uint8 adsr1)   
{
  return adsr1 & 0x0f;
}

const char * ADSR::get_decay_str(Uint8 adsr1)   
{
  Uint8 decay = (adsr1 & 0x70) >> 4;
  return ADSR::decay_map[decay].str;
}
int ADSR::get_decay_index(Uint8 adsr1)   
{
  return (adsr1 & 0x70) >> 4;
}

const char * ADSR::get_sustain_level_str(Uint8 adsr2)   
{
  Uint8 sustain_level = (adsr2 & 0xe0) >> 5;
  return sustain_level_map[sustain_level].str;
}
int ADSR::get_sustain_level_index(Uint8 adsr2)   
{
  return (adsr2 & 0xe0) >> 5;
}

const char * ADSR::get_sustain_release_str(Uint8 adsr2)   
{
  Uint8 sustain_release = (adsr2 & 0x1f);
  return sustain_release_map[sustain_release].str;
}
int ADSR::get_sustain_release_index(Uint8 adsr2)   
{
  return (adsr2 & 0x1f);
}

