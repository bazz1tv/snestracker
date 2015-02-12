#include "ADSR.h"



bool ADSR::Context_Menus::check_left_click_activate(int &x, int &y)
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

int ADSR::Context_Menus::receive_event(SDL_Event &ev)
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

const IntToCharStar ADSR::attack_map[ATTACK_MAP_SIZE] = 
{
  {0,  "4.1  s"},
  {1,  "2.6  s"},
  {2,  "1.5  s"},
  {3,  "1.0  s"},
  {4,  "640  ms"},
  {5,  "380  ms"},
  {6,  "260  ms"},
  {7,  "160  ms"},
  {8,  "96   ms"},
  {9,  "64   ms"},
  {10, "40   ms"},
  {11, "24   ms"},
  {12, "16   ms"},
  {13, "10   ms"},
  {14, "6    ms"},
  {15, "0    ms"}
};
const IntToCharStar ADSR::decay_map[SUSTAIN_LEVEL_MAP_SIZE][DECAY_MAP_SIZE] = 
{
  {
    {0, "884 ms"},
    {1, "557 ms"},
    {2, "340 ms"},
    {3, "231 ms"},
    {4, "150 ms"},
    {5, "95 ms"},
    {6, "68 ms"},
    {7, "41 ms"},
  },
  {
    {0, "626 ms"},
    {1, "395 ms"},
    {2, "241 ms"},
    {3, "164 ms"},
    {4, "106 ms"},
    {5, "67 ms"},
    {6, "48 ms"},
    {7, "29 ms"},
  },
  {
    {0, "451 ms"},
    {1, "284 ms"},
    {2, "173 ms"},
    {3, "118 ms"},
    {4, "76 ms"},
    {5, "49 ms"},
    {6, "35 ms"},
    {7, "21 ms"},
  },
  {
    {0, "323 ms"},
    {1, "204 ms"},
    {2, "124 ms"},
    {3, "84 ms"},
    {4, "55 ms"},
    {5, "35 ms"},
    {6, "25 ms"},
    {7, "15 ms"},
  },
  {
    {0, "219 ms"},
    {1, "138 ms"},
    {2, "84 ms"},
    {3, "57 ms"},
    {4, "37 ms"},
    {5, "24 ms"},
    {6, "17 ms"},
    {7, "10 ms"},
  },
  {
    {0, "132 ms"},
    {1, "83 ms"},
    {2, "51 ms"},
    {3, "35 ms"},
    {4, "22 ms"},
    {5, "14 ms"},
    {6, "10 ms"},
    {7, "6 ms"},
  },
  {
    {0, "57 ms"},
    {1, "36 ms"},
    {2, "22 ms"},
    {3, "15 ms"},
    {4, "10 ms"},
    {5, "6 ms"},
    {6, "4 ms"},
    {7, "3 ms"},
  },
  {
    {0, "0 ms"},
    {1, "0 ms"},
    {2, "0 ms"},
    {3, "0 ms"},
    {4, "0 ms"},
    {5, "0 ms"},
    {6, "0 ms"},
    {7, "0 ms"},
  }
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
  {7, "No Decay"}
};
const IntToCharStar ADSR::sustain_release_map[SUSTAIN_LEVEL_MAP_SIZE][SUSTAIN_RELEASE_MAP_SIZE] = 
{
  {
    {0, "Infinite"},
    {1, "16.4 s"},
    {2, "12.3 s"},
    {3, "10.2 s"},
    {4, "8.2 s"},
    {5, "6.2 s"},
    {6, "5.1 s"},
    {7, "4.1 s"},
    {8, "3.1 s"},
    {9, "2.6 s"},
    {10, "2.1 s"},
    {11, "1.5 s"},
    {12, "1.3 s"},
    {13, "1.0 s"},
    {14, "776 ms"},
    {15, "648 ms"},
    {16, "520 ms"},
    {17, "392 ms"},
    {18, "328 ms"},
    {19, "264 ms"},
    {20, "200 ms"},
    {21, "168 ms"},
    {22, "136 ms"},
    {23, "104 ms"},
    {24, "88 ms"},
    {25, "72 ms"},
    {26, "56 ms"},
    {27, "48 ms"},
    {28, "40 ms"},
    {29, "32 ms"},
    {30, "24 ms"},
    {31, "16 ms"},
  },
  {
    {0, "Infinite"},
    {1, "24.5 s"},
    {2, "18.4 s"},
    {3, "15.3 s"},
    {4, "12.3 s"},
    {5, "9.2 s"},
    {6, "7.7 s"},
    {7, "6.1 s"},
    {8, "4.6 s"},
    {9, "3.8 s"},
    {10, "3.1 s"},
    {11, "2.3 s"},
    {12, "1.9 s"},
    {13, "1.5 s"},
    {14, "1.2 s"},
    {15, "969 ms"},
    {16, "778 ms"},
    {17, "586 ms"},
    {18, "491 ms"},
    {19, "395 ms"},
    {20, "299 ms"},
    {21, "251 ms"},
    {22, "203 ms"},
    {23, "156 ms"},
    {24, "132 ms"},
    {25, "108 ms"},
    {26, "84 ms"},
    {27, "72 ms"},
    {28, "60 ms"},
    {29, "48 ms"},
    {30, "36 ms"},
    {31, "24 ms"},
  },
  {
    {0, "Infinite"},
    {1, "30.0 s"},
    {2, "22.5 s"},
    {3, "18.8 s"},
    {4, "15.0 s"},
    {5, "11.3 s"},
    {6, "9.4 s"},
    {7, "7.5 s"},
    {8, "5.6 s"},
    {9, "4.7 s"},
    {10, "3.8 s"},
    {11, "2.8 s"},
    {12, "2.4 s"},
    {13, "1.9 s"},
    {14, "1.4 s"},
    {15, "1.2 s"},
    {16, "953 ms"},
    {17, "718 ms"},
    {18, "601 ms"},
    {19, "484 ms"},
    {20, "366 ms"},
    {21, "308 ms"},
    {22, "249 ms"},
    {23, "191 ms"},
    {24, "161 ms"},
    {25, "132 ms"},
    {26, "103 ms"},
    {27, "88 ms"},
    {28, "73 ms"},
    {29, "59 ms"},
    {30, "44 ms"},
    {31, "29 ms"},
  },
  {
    {0, "Infinite"},
    {1, "34.1 s"},
    {2, "25.6 s"},
    {3, "21.3 s"},
    {4, "17.0 s"},
    {5, "12.8 s"},
    {6, "10.7 s"},
    {7, "8.5 s"},
    {8, "6.4 s"},
    {9, "5.3 s"},
    {10, "4.3 s"},
    {11, "3.2 s"},
    {12, "2.7 s"},
    {13, "2.1 s"},
    {14, "1.6 s"},
    {15, "1.3 s"},
    {16, "1.1 s"},
    {17, "815 ms"},
    {18, "682 ms"},
    {19, "549 ms"},
    {20, "416 ms"},
    {21, "349 ms"},
    {22, "283 ms"},
    {23, "216 ms"},
    {24, "183 ms"},
    {25, "150 ms"},
    {26, "116 ms"},
    {27, "100 ms"},
    {28, "83 ms"},
    {29, "67 ms"},
    {30, "50 ms"},
    {31, "33 ms"},
  },
  {
    {0, "Infinite"},
    {1, "37.3 s"},
    {2, "28.0 s"},
    {3, "23.3 s"},
    {4, "18.7 s"},
    {5, "14.0 s"},
    {6, "11.7 s"},
    {7, "9.3 s"},
    {8, "7.0 s"},
    {9, "5.8 s"},
    {10, "4.7 s"},
    {11, "3.5 s"},
    {12, "2.9 s"},
    {13, "2.4 s"},
    {14, "1.8 s"},
    {15, "1.5 s"},
    {16, "1.2 s"},
    {17, "893 ms"},
    {18, "747 ms"},
    {19, "601 ms"},
    {20, "455 ms"},
    {21, "383 ms"},
    {22, "310 ms"},
    {23, "237 ms"},
    {24, "200 ms"},
    {25, "164 ms"},
    {26, "128 ms"},
    {27, "109 ms"},
    {28, "91 ms"},
    {29, "73 ms"},
    {30, "55 ms"},
    {31, "36 ms"},
  },
  {
    {0, "Infinite"},
    {1, "40.1 s"},
    {2, "30.1 s"},
    {3, "25.1 s"},
    {4, "20.1 s"},
    {5, "15.0 s"},
    {6, "12.5 s"},
    {7, "10.0 s"},
    {8, "7.5 s"},
    {9, "6.3 s"},
    {10, "5.0 s"},
    {11, "3.8 s"},
    {12, "3.1 s"},
    {13, "2.5 s"},
    {14, "1.9 s"},
    {15, "1.6 s"},
    {16, "1.3 s"},
    {17, "959 ms"},
    {18, "802 ms"},
    {19, "646 ms"},
    {20, "489 ms"},
    {21, "411 ms"},
    {22, "333 ms"},
    {23, "254 ms"},
    {24, "215 ms"},
    {25, "176 ms"},
    {26, "137 ms"},
    {27, "117 ms"},
    {28, "98 ms"},
    {29, "78 ms"},
    {30, "59 ms"},
    {31, "39 ms"},
  },
  {
    {0, "Infinite"},
    {1, "42.5 s"},
    {2, "31.8 s"},
    {3, "26.5 s"},
    {4, "21.2 s"},
    {5, "15.9 s"},
    {6, "13.3 s"},
    {7, "10.6 s"},
    {8, "8.0 s"},
    {9, "6.7 s"},
    {10, "5.3 s"},
    {11, "4.0 s"},
    {12, "3.3 s"},
    {13, "2.7 s"},
    {14, "2.0 s"},
    {15, "1.7 s"},
    {16, "1.3 s"},
    {17, "1.0 s"},
    {18, "849 ms"},
    {19, "684 ms"},
    {20, "518 ms"},
    {21, "435 ms"},
    {22, "352 ms"},
    {23, "269 ms"},
    {24, "228 ms"},
    {25, "186 ms"},
    {26, "145 ms"},
    {27, "124 ms"},
    {28, "104 ms"},
    {29, "83 ms"},
    {30, "62 ms"},
    {31, "41 ms"},
  },
  {
    {0, "Infinite"},
    {1, "44.2 s"},
    {2, "33.2 s"},
    {3, "27.7 s"},
    {4, "22.1 s"},
    {5, "16.6 s"},
    {6, "13.8 s"},
    {7, "11.1 s"},
    {8, "8.3 s"},
    {9, "6.9 s"},
    {10, "5.5 s"},
    {11, "4.2 s"},
    {12, "3.5 s"},
    {13, "2.8 s"},
    {14, "2.1 s"},
    {15, "1.7 s"},
    {16, "1.4 s"},
    {17, "1.1 s"},
    {18, "885 ms"},
    {19, "713 ms"},
    {20, "540 ms"},
    {21, "453 ms"},
    {22, "367 ms"},
    {23, "281 ms"},
    {24, "238 ms"},
    {25, "194 ms"},
    {26, "151 ms"},
    {27, "130 ms"},
    {28, "108 ms"},
    {29, "86 ms"},
    {30, "65 ms"},
    {31, "43 ms"},
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

/*const char * ADSR::get_decay_str(Uint8 adsr1)   
{
  Uint8 decay = (adsr1 & 0x70) >> 4;
  return ADSR::decay_map[decay].str;
}*/

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

/*const char * ADSR::get_sustain_release_str(Uint8 adsr2)   
{
  Uint8 sustain_release = (adsr2 & 0x1f);
  return sustain_release_map[sustain_release].str;
}*/
int ADSR::get_sustain_release_index(Uint8 adsr2)   
{
  return (adsr2 & 0x1f);
}
Uint8 ADSR::reverse_sustain_release_index(Uint8 i)
{
  return (i & 0x1f);
}

