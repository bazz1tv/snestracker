#include "ADSR.h"



bool ADSR::Context_Menus::check_left_click_activate(int &x, int &y)
{
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
  decay_context.menu.update_current_item(ADSR::get_decay_index(adsr1));
  sustain_level_context.menu.update_current_item(ADSR::get_sustain_level_index(adsr2));
  sustain_release_context.menu.update_current_item(ADSR::get_sustain_release_index(adsr2));
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
const IntToCharStar ADSR::decay_map[DECAY_MAP_SIZE] = 
{
  {0, "1.2  s"},
  {1, "740  ms"},
  {2, "440  ms"},
  {3, "290  ms"},
  {4, "180  ms"},
  {5, "110  ms"},
  {6, "74   ms"},
  {7, "37   ms"}
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
  {7, "8/8"}
};
const IntToCharStar ADSR::sustain_release_map[SUSTAIN_RELEASE_MAP_SIZE] = 
{
  {0,  "Infinite"},
  {1,  "38   s"},
  {2,  "28   s"},
  {3,  "24   s"},
  {4,  "19   s"},
  {5,  "14   s"},
  {6,  "12   s"},
  {7,  "9.4  s"},
  {8,  "7.1  s"},
  {9,  "5.9  s"},
  {10, "4.7  s"},
  {11, "3.5  s"},
  {12, "2.9  s"},
  {13, "2.4  s"},
  {14, "1.8  s"},
  {15, "1.5  s"},
  {16, "1.2  s"},
  {17, "880  ms"},
  {18, "740  ms"},
  {19, "590  ms"},
  {20, "440  ms"},
  {21, "370  ms"},
  {22, "290  ms"},
  {23, "220  ms"},
  {24, "180  ms"},
  {25, "150  ms"},
  {26, "110  ms"},
  {27, "92   ms"},
  {28, "74   ms"},
  {29, "55   ms"},
  {30, "37   ms"},
  {31, "18   ms"}
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

const char * ADSR::get_decay_str(Uint8 adsr1)   
{
  Uint8 decay = (adsr1 & 0x70) >> 4;
  return ADSR::decay_map[decay].str;
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

const char * ADSR::get_sustain_release_str(Uint8 adsr2)   
{
  Uint8 sustain_release = (adsr2 & 0x1f);
  return sustain_release_map[sustain_release].str;
}
int ADSR::get_sustain_release_index(Uint8 adsr2)   
{
  return (adsr2 & 0x1f);
}
Uint8 ADSR::reverse_sustain_release_index(Uint8 i)
{
  return (i & 0x1f);
}

