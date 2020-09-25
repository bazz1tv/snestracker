#include "gui/Text.h"
#include "Colors.h"
#include "sdlfont.h"

Text::Text(const char *str/*=""*/) : str(str)
{
  init_width_height();
}

void Text::init_width_height()
{
  rect.w = strlen(str)*CHAR_WIDTH; // could add padding
  rect.h = CHAR_HEIGHT;
}

void Text::draw(SDL_Surface *screen/*=RenderContext::screen*/,
  Uint32 fg_color/*=Colors::Interface::color[Colors::Interface::Type::text_fg]*/,
  bool prefill/*=true*/, bool Vflip/*=false*/, bool Hflip/*=false*/,
  Uint32 bg_color/*=Colors::Interface::color[Colors::Interface::Type::text_bg]*/
)
{
  sdlfont_drawString(screen, rect.x, rect.y, str, fg_color,
    bg_color, prefill, Vflip, Hflip);
}
