#include "ADSR.h"


bool ADSR::Context_Menus::check_left_click_activate(const int x, const int y)
{
  static bool first=true;

  if (first)
  {
    fprintf(stderr, "x=%d,y=%d,w=%d,h=%d\n", attack_context.menu.single_item_rect.x,
      attack_context.menu.single_item_rect.y,
      attack_context.menu.single_item_rect.w,
      attack_context.menu.single_item_rect.h);
  }

  if (attack_context.menu.check_left_click_activate(x, y))
    return true;
  if (decay_context.menu.check_left_click_activate(x, y))
    return true;
  if (sustain_level_context.menu.check_left_click_activate(x, y))
    return true;
  if (sustain_release_context.menu.check_left_click_activate(x, y))
    return true;

  return false;
}

int ADSR::Context_Menus::receive_event(const SDL_Event &ev)
{
  int r;
  if ((r=attack_context.menu.receive_event(ev)))
  {
    if (r == Expanding_List::EVENT_MENU)
      return ATTACK_CHANGED;
    return EVENT_ACTIVE;
  }
  if ((r=decay_context.menu.receive_event(ev)))
  {
    if (r == Expanding_List::EVENT_MENU)
      return DECAY_CHANGED;
    return EVENT_ACTIVE;
  }
  if ((r=sustain_level_context.menu.receive_event(ev)))
  {
    if (r == Expanding_List::EVENT_MENU)
      return SUSTAIN_LEVEL_CHANGED;
    return EVENT_ACTIVE;
  }
  if ((r=sustain_release_context.menu.receive_event(ev)))
  {
    if (r == Expanding_List::EVENT_MENU)
     return SUSTAIN_RELEASE_CHANGED;
    return EVENT_ACTIVE;
  }

  return EVENT_INACTIVE;
}
void ADSR::Context_Menus::update(Uint8 adsr1, Uint8 adsr2)
{
  attack_context.menu.update_current_item(ADSR::get_attack_index(adsr1));
  sustain_level_context.menu.update_current_item(7-ADSR::get_sustain_level_index(adsr2));
  decay_context.menu.update_current_item(ADSR::get_decay_index(adsr1));
    decay_context.update();
  sustain_release_context.menu.update_current_item(ADSR::get_sustain_release_index(adsr2));
    sustain_release_context.update();
}

void ADSR::Context_Menus::draw(SDL_Surface *screen)
{
  attack_context.menu.draw(screen);
  decay_context.menu.draw(screen);
  sustain_level_context.menu.draw(screen);
  sustain_release_context.menu.draw(screen);
}

const int ADSR::INFINITE = 50.0 * 1000;

const AdsrMap ADSR::attack_map[ATTACK_MAP_SIZE] =
{
  {0,  "4.1  s", 4.1 * 1000},
  {1,  "2.6  s", 2.6 * 1000},
  {2,  "1.5  s", 1.5 * 1000},
  {3,  "1.0  s", 1.0 * 1000},
  {4,  "640  ms", 640},
  {5,  "380  ms", 380},
  {6,  "260  ms", 260},
  {7,  "160  ms", 160},
  {8,  "96   ms",  96},
  {9,  "64   ms",  64},
  {10, "40   ms",  40},
  {11, "24   ms",  24},
  {12, "16   ms",  16},
  {13, "10   ms",  10},
  {14, "6    ms",   6},
  {15, "0    ms",   0}
};
const AdsrMap ADSR::decay_map[SUSTAIN_LEVEL_MAP_SIZE][DECAY_MAP_SIZE] = 
{
  {
    {0, "884 ms", 884},
    {1, "557 ms", 557},
    {2, "340 ms", 340},
    {3, "231 ms", 231},
    {4, "150 ms", 150},
    {5, "95 ms",   95},
    {6, "68 ms",   68},
    {7, "41 ms",   41},
  },
  {
    {0, "626 ms", 626},
    {1, "395 ms", 395},
    {2, "241 ms", 241},
    {3, "164 ms", 164},
    {4, "106 ms", 106},
    {5, "67 ms",   67},
    {6, "48 ms",   48},
    {7, "29 ms",   29},
  },
  {
    {0, "451 ms",451},
    {1, "284 ms",284},
    {2, "173 ms",173},
    {3, "118 ms",118},
    {4, "76 ms",  76},
    {5, "49 ms",  49},
    {6, "35 ms",  35},
    {7, "21 ms",  21},
  },
  {
    {0, "323 ms", 323},
    {1, "204 ms", 204},
    {2, "124 ms", 124},
    {3, "84 ms", 84},
    {4, "55 ms", 55},
    {5, "35 ms", 35},
    {6, "25 ms", 25},
    {7, "15 ms", 15},
  },
  {
    {0, "219 ms", 219},
    {1, "138 ms", 138},
    {2, "84 ms", 84},
    {3, "57 ms", 57},
    {4, "37 ms", 37},
    {5, "24 ms", 24},
    {6, "17 ms", 17},
    {7, "10 ms", 10},
  },
  {
    {0, "132 ms", 132},
    {1, "83 ms", 83},
    {2, "51 ms", 51},
    {3, "35 ms", 35},
    {4, "22 ms", 22},
    {5, "14 ms", 14},
    {6, "10 ms", 10},
    {7, "6 ms", 6},
  },
  {
    {0, "57 ms", 57},
    {1, "36 ms", 36},
    {2, "22 ms", 22},
    {3, "15 ms", 15},
    {4, "10 ms", 10},
    {5, "6 ms", 6},
    {6, "4 ms", 4},
    {7, "3 ms", 3},
  },
  {
    {0, "0 ms", 0},
    {1, "0 ms", 0},
    {2, "0 ms", 0},
    {3, "0 ms", 0},
    {4, "0 ms", 0},
    {5, "0 ms", 0},
    {6, "0 ms", 0},
    {7, "0 ms", 0},
  }
};
const AdsrMap ADSR::sustain_level_map[SUSTAIN_LEVEL_MAP_SIZE] =
{
  {0, "1/8", (1.0/8.0)},
  {1, "2/8", (2.0/8.0)},
  {2, "3/8", (3.0/8.0)},
  {3, "4/8", (4.0/8.0)},
  {4, "5/8", (5.0/8.0)},
  {5, "6/8", (6.0/8.0)},
  {6, "7/8", (7.0/8.0)},
  {7, "No Decay", (8/8)}
};
const AdsrMap ADSR::sustain_release_map[SUSTAIN_LEVEL_MAP_SIZE][SUSTAIN_RELEASE_MAP_SIZE] =
{
  {
    { 0, "Infinite", INFINITE},
    { 1, "16.4 s", 16.4 * 1000},
    { 2, "12.3 s", 12.3 * 1000},
    { 3, "10.2 s", 10.2 * 1000},
    { 4,  "8.2 s",  8.2 * 1000},
    { 5,  "6.2 s",  6.2 * 1000},
    { 6,  "5.1 s",  5.1 * 1000},
    { 7,  "4.1 s",  4.1 * 1000},
    { 8,  "3.1 s",  3.1 * 1000},
    { 9,  "2.6 s",  2.6 * 1000},
    {10,  "2.1 s",  2.1 * 1000},
    {11,  "1.5 s",  1.5 * 1000},
    {12,  "1.3 s",  1.3 * 1000},
    {13,  "1.0 s",  1.0 * 1000},
    {14, "776 ms", 776},
    {15, "648 ms", 648},
    {16, "520 ms", 520},
    {17, "392 ms", 392},
    {18, "328 ms", 328},
    {19, "264 ms", 264},
    {20, "200 ms", 200},
    {21, "168 ms", 168},
    {22, "136 ms", 136},
    {23, "104 ms", 104},
    {24,  "88 ms", 88},
    {25,  "72 ms", 72},
    {26,  "56 ms", 56},
    {27,  "48 ms", 48},
    {28,  "40 ms", 40},
    {29,  "32 ms", 32},
    {30,  "24 ms", 24},
    {31,  "16 ms", 16},
  },
  {
    {0, "Infinite", INFINITE},
    {1, "24.5 s", 24.5 * 1000},
    {2, "18.4 s", 18.4 * 1000},
    {3, "15.3 s", 15.3 * 1000},
    {4, "12.3 s", 12.3 * 1000},
    {5, "9.2 s", 9.2 * 1000},
    {6, "7.7 s", 7.7 * 1000},
    {7, "6.1 s", 6.1 * 1000},
    {8, "4.6 s", 4.6 * 1000},
    {9, "3.8 s", 3.8 * 1000},
    {10, "3.1 s", 3.1 * 1000},
    {11, "2.3 s", 2.3 * 1000},
    {12, "1.9 s", 1.9 * 1000},
    {13, "1.5 s", 1.5 * 1000},
    {14, "1.2 s", 1.2 * 1000},
    {15, "969 ms", 969},
    {16, "778 ms", 778},
    {17, "586 ms", 586},
    {18, "491 ms", 491},
    {19, "395 ms", 395},
    {20, "299 ms", 299},
    {21, "251 ms", 251},
    {22, "203 ms", 203},
    {23, "156 ms", 156},
    {24, "132 ms", 132},
    {25, "108 ms", 108},
    {26, "84 ms", 84},
    {27, "72 ms", 72},
    {28, "60 ms", 60},
    {29, "48 ms", 48},
    {30, "36 ms", 36},
    {31, "24 ms", 24},
  },
  {
    {0, "Infinite", INFINITE},
    {1, "30.0 s", 30.0 * 1000},
    {2, "22.5 s", 22.5 * 1000},
    {3, "18.8 s", 18.8 * 1000},
    {4, "15.0 s", 15.0 * 1000},
    {5, "11.3 s", 11.3 * 1000},
    {6, "9.4 s", 9.4 * 1000},
    {7, "7.5 s", 7.5 * 1000},
    {8, "5.6 s", 5.6 * 1000},
    {9, "4.7 s", 4.7 * 1000},
    {10, "3.8 s", 3.8 * 1000},
    {11, "2.8 s", 2.8 * 1000},
    {12, "2.4 s", 2.4 * 1000},
    {13, "1.9 s", 1.9 * 1000},
    {14, "1.4 s", 1.4 * 1000},
    {15, "1.2 s", 1.2 * 1000},
    {16, "953 ms", 953},
    {17, "718 ms", 718},
    {18, "601 ms", 601},
    {19, "484 ms", 484},
    {20, "366 ms", 366},
    {21, "308 ms", 308},
    {22, "249 ms", 249},
    {23, "191 ms", 191},
    {24, "161 ms", 161},
    {25, "132 ms", 132},
    {26, "103 ms", 103},
    {27, "88 ms", 88},
    {28, "73 ms", 73},
    {29, "59 ms", 59},
    {30, "44 ms", 44},
    {31, "29 ms", 29},
  },
  {
    {0, "Infinite", INFINITE},
    {1, "34.1 s", 34.1 * 1000},
    {2, "25.6 s", 25.6 * 1000},
    {3, "21.3 s", 21.3 * 1000},
    {4, "17.0 s", 17.0 * 1000},
    {5, "12.8 s", 12.8 * 1000},
    {6, "10.7 s", 10.7 * 1000},
    {7, "8.5 s", 8.5 * 1000},
    {8, "6.4 s", 6.4 * 1000},
    {9, "5.3 s", 5.3 * 1000},
    {10, "4.3 s", 4.3 * 1000},
    {11, "3.2 s", 3.2 * 1000},
    {12, "2.7 s", 2.7 * 1000},
    {13, "2.1 s", 2.1 * 1000},
    {14, "1.6 s", 1.6 * 1000},
    {15, "1.3 s", 1.3 * 1000},
    {16, "1.1 s", 1.1 * 1000},
    {17, "815 ms", 815},
    {18, "682 ms", 682},
    {19, "549 ms", 549},
    {20, "416 ms", 416},
    {21, "349 ms", 349},
    {22, "283 ms", 283},
    {23, "216 ms", 216},
    {24, "183 ms", 183},
    {25, "150 ms", 150},
    {26, "116 ms", 116},
    {27, "100 ms", 100},
    {28, "83 ms", 83},
    {29, "67 ms", 67},
    {30, "50 ms", 50},
    {31, "33 ms", 33},
  },
  {
    {0, "Infinite", INFINITE},
    {1, "37.3 s", 37.3 * 1000},
    {2, "28.0 s", 28.0 * 1000},
    {3, "23.3 s", 23.3 * 1000},
    {4, "18.7 s", 18.7 * 1000},
    {5, "14.0 s", 14.0 * 1000},
    {6, "11.7 s", 11.7 * 1000},
    {7, "9.3 s", 9.3 * 1000},
    {8, "7.0 s", 7.0 * 1000},
    {9, "5.8 s", 5.8 * 1000},
    {10, "4.7 s", 4.7 * 1000},
    {11, "3.5 s", 3.5 * 1000},
    {12, "2.9 s", 2.9 * 1000},
    {13, "2.4 s", 2.4 * 1000},
    {14, "1.8 s", 1.8 * 1000},
    {15, "1.5 s", 1.5 * 1000},
    {16, "1.2 s", 1.2 * 1000},
    {17, "893 ms", 893},
    {18, "747 ms", 747},
    {19, "601 ms", 601},
    {20, "455 ms", 455},
    {21, "383 ms", 383},
    {22, "310 ms", 310},
    {23, "237 ms", 237},
    {24, "200 ms", 200},
    {25, "164 ms", 164},
    {26, "128 ms", 128},
    {27, "109 ms", 109},
    {28, "91 ms", 91},
    {29, "73 ms", 73},
    {30, "55 ms", 55},
    {31, "36 ms", 36},
  },
  {
    {0, "Infinite", INFINITE},
    {1, "40.1 s", 40.1 * 1000},
    {2, "30.1 s", 30.1 * 1000},
    {3, "25.1 s", 25.1 * 1000},
    {4, "20.1 s", 20.1 * 1000},
    {5, "15.0 s", 15.0 * 1000},
    {6, "12.5 s", 12.5 * 1000},
    {7, "10.0 s", 10.0 * 1000},
    {8, "7.5 s", 7.5 * 1000},
    {9, "6.3 s", 6.3 * 1000},
    {10, "5.0 s", 5.0 * 1000},
    {11, "3.8 s", 3.8 * 1000},
    {12, "3.1 s", 3.1 * 1000},
    {13, "2.5 s", 2.5 * 1000},
    {14, "1.9 s", 1.9 * 1000},
    {15, "1.6 s", 1.6 * 1000},
    {16, "1.3 s", 1.3 * 1000},
    {17, "959 ms", 959},
    {18, "802 ms", 802},
    {19, "646 ms", 646},
    {20, "489 ms", 489},
    {21, "411 ms", 411},
    {22, "333 ms", 333},
    {23, "254 ms", 254},
    {24, "215 ms", 215},
    {25, "176 ms", 176},
    {26, "137 ms", 137},
    {27, "117 ms", 117},
    {28, "98 ms", 98},
    {29, "78 ms", 78},
    {30, "59 ms", 59},
    {31, "39 ms", 39},
  },
  {
    {0, "Infinite", INFINITE},
    {1, "42.5 s", 42.5 * 1000},
    {2, "31.8 s", 31.8 * 1000},
    {3, "26.5 s", 26.5 * 1000},
    {4, "21.2 s", 21.2 * 1000},
    {5, "15.9 s", 15.9 * 1000},
    {6, "13.3 s", 13.3 * 1000},
    {7, "10.6 s", 10.6 * 1000},
    {8, "8.0 s", 8.0 * 1000},
    {9, "6.7 s", 6.7 * 1000},
    {10, "5.3 s", 5.3 * 1000},
    {11, "4.0 s", 4.0 * 1000},
    {12, "3.3 s", 3.3 * 1000},
    {13, "2.7 s", 2.7 * 1000},
    {14, "2.0 s", 2.0 * 1000},
    {15, "1.7 s", 1.7 * 1000},
    {16, "1.3 s", 1.3 * 1000},
    {17, "1.0 s", 1.0 * 1000},
    {18, "849 ms", 849},
    {19, "684 ms", 684},
    {20, "518 ms", 518},
    {21, "435 ms", 435},
    {22, "352 ms", 352},
    {23, "269 ms", 269},
    {24, "228 ms", 228},
    {25, "186 ms", 186},
    {26, "145 ms", 145},
    {27, "124 ms", 124},
    {28, "104 ms", 104},
    {29, "83 ms", 83},
    {30, "62 ms", 62},
    {31, "41 ms", 41},
  },
  {
    { 0, "Infinite", INFINITE},
    { 1, "44.2 s",44.2 * 1000},
    { 2, "33.2 s",33.2 * 1000},
    { 3, "27.7 s",27.7 * 1000},
    { 4, "22.1 s",22.1 * 1000},
    { 5, "16.6 s",16.6 * 1000},
    { 6, "13.8 s",13.8 * 1000},
    { 7, "11.1 s",11.1 * 1000},
    { 8, "8.3 s",  8.3 * 1000},
    { 9, "6.9 s",  6.9 * 1000},
    {10, "5.5 s",  5.5 * 1000},
    {11, "4.2 s",  4.2 * 1000},
    {12, "3.5 s",  3.5 * 1000},
    {13, "2.8 s",  2.8 * 1000},
    {14, "2.1 s",  2.1 * 1000},
    {15, "1.7 s",  1.7 * 1000},
    {16, "1.4 s",  1.4 * 1000},
    {17, "1.1 s",  1.1 * 1000},
    {18, "885 ms", 885},
    {19, "713 ms", 713},
    {20, "540 ms", 540},
    {21, "453 ms", 453},
    {22, "367 ms", 367},
    {23, "281 ms", 281},
    {24, "238 ms", 238},
    {25, "194 ms", 194},
    {26, "151 ms", 151},
    {27, "130 ms", 130},
    {28, "108 ms", 108},
    {29, "86 ms", 86},
    {30, "65 ms", 65},
    {31, "43 ms", 43},
  }

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

Uint8 ADSR::reverse_attack_index(Uint8 i)
{
  return i & 0x0f;
}

int ADSR::get_decay_index(Uint8 adsr1)   
{
  return (adsr1 & 0x70) >> 4;
}

Uint8 ADSR::reverse_decay_index(Uint8 i)
{
  return (i << 4) & 0x70;
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
Uint8 ADSR::reverse_sustain_level_index(Uint8 i)
{
  return (i << 5) & 0xe0;
}

int ADSR::get_sustain_release_index(Uint8 adsr2)   
{
  return (adsr2 & 0x1f);
}
Uint8 ADSR::reverse_sustain_release_index(Uint8 i)
{
  return (i & 0x1f);
}

