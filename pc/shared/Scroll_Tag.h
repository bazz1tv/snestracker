#include "SDL.h"
#include "shared/sdlfont.h"

struct Scroll_Tag
{
  static void compute(const char *str, Scroll_Tag *st, Uint32 extra_wait=1000);
  void scroll_draw();

  static const int MAX_NO_SCROLL_LEN = 22;
  Uint32 cur_ticks_compare;
  Uint32 extra_wait=1000;
  bool need_to_scroll=false;
  int times_to_scroll=0;
  const char *str;
  int str_len=0;
  int cur_index=0;
  char direction  = +1;
  SDL_Rect rect = {0,0,22*CHAR_WIDTH, CHAR_HEIGHT};
};
