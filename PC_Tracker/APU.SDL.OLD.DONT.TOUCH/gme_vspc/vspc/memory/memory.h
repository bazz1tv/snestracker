#include "SDL.h"
#include "gme/player/Music_Player.h"

class Memory
{
  Memory(Music_Player *player, SDL_Surface *screen);
  static Music_Player *player;
  static SDL_Surface *screen;
  static uint8_t *IAPURAM;
  static Uint16 address; //=0x0000;
  static Uint16 addr_being_edited;
};