#include "gui/Text.h"
#include "Colors.h"
#include "sdlfont.h"
Text::Text()
{
  color = Colors::Interface::color[Colors::Interface::Type::text_fg];
}
Text::Text(const char *str) : str(str) {}

void Text::draw(Uint32 color/*=color*/, bool prefill/*=true*/,
  bool Vflip/*=false*/, bool Hflip/*=false*/, SDL_Surface *screen/*=RenderContext::screen*/)
{
  sdlfont_drawString(screen, rect.x, rect.y, str, color,
    Colors::Interface::color[Colors::Interface::Type::text_bg], prefill, Vflip, Hflip);
}

void Text::draw(bool prefill/*=true*/,
    bool Vflip/*=false*/, bool Hflip/*=false*/, SDL_Surface *screen/*=RenderContext::screen*/)
{
  sdlfont_drawString(screen, rect.x, rect.y, str, color,
      Colors::Interface::color[Colors::Interface::Type::text_bg], prefill, Vflip, Hflip);
}
