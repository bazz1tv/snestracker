#include "shared/gui/Text_Edit_Rect.h"
#include "utility.h"
#include "shared/sdl_userevents.h"
Cursor Text_Edit_Rect::cursor;
char Text_Edit_Rect::markedText[SDL_TEXTEDITINGEVENT_TEXT_SIZE] = "";
int Text_Edit_Rect::comp_start_point = 0;
SDL_Rect Text_Edit_Rect::markedRect;

Text_Edit_Rect::Text_Edit_Rect(int txtwidth/*=0*/, const char *str/*=""*/,
  int strsize/*=0*/) :
        Clickable_Text(str, clicked_callback, this),
        strsize(strsize)
{
  //if (width % CHAR_HEIGHT)
    //width += CHAR_HEIGHT - (width % CHAR_HEIGHT);
  rect.w = txtwidth * CHAR_WIDTH;
  rect.h = CHAR_HEIGHT;
  max_visible_chars = txtwidth;
}



/* utf8 helper funcs from
 * https://hg.libsdl.org/SDL/file/783d1cff9b20/test/testime.c */

size_t utf8_length(unsigned char c)
{
  c = (unsigned char)(0xff & c);
  if (c < 0x80)
    return 1;
  else if ((c >> 5) ==0x6)
    return 2;
  else if ((c >> 4) == 0xe)
    return 3;
  else if ((c >> 3) == 0x1e)
    return 4;
  else
    return 0;
}

char *utf8_next(char *p)
{
  size_t len = utf8_length(*p);
  size_t i = 0;
  if (!len)
    return 0;

  for (; i < len; ++i)
  {
    ++p;
    if (!*p)
      return 0;
  }
  return p;
}

char *utf8_advance(char *p, size_t distance)
{
  size_t i = 0;
  for (; i < distance && p; ++i)
  {
    p = utf8_next(p);
  }
  return p;
}


static inline int check_dblclick(const SDL_Event &ev, Text_Edit_Rect *ter)
{
  switch (ev.type)
  {
    case SDL_USEREVENT:
      if (ev.user.code == UserEvents::mouse_react)
      {
        SDL_Event *te = (SDL_Event *)ev.user.data1; // the mouse coordinates at time of double click
        if (ter->check_mouse_and_execute(te->button.x, te->button.y))
          return 1;
      }
      break;
    default:break;
  }
}
/* Let's try putting the edit logic into the window's logic handler. */
int handle_text_edit_rect_event(const SDL_Event &ev, Text_Edit_Rect *ter)
{
  check_dblclick(ev, ter);

  if (!ter->editing)
    return 0;

  switch (ev.type)
  {
    case SDL_KEYDOWN: {
      switch (ev.key.keysym.sym)
      {
        case SDLK_RETURN:
          SDL_StopTextInput();
          Text_Edit_Rect::cursor.stop_timer();
          ter->needs_redraw = true;
          ter->editing = false;
          break;
        case SDLK_BACKSPACE:
          {
            int textlen=strlen(ter->str);
            ter->needs_redraw = true;

            do {
              if (textlen==0)
              {
                break;
              }
              if ((ter->str[textlen-1] & 0x80) == 0x00)
              {
                /* One byte */
                ((char *)ter->str)[textlen-1]=0x00;
                break;
              }
              if ((ter->str[textlen-1] & 0xC0) == 0x80)
              {
                /* Byte from the multibyte sequence */
                ((char *)ter->str)[textlen-1]=0x00;
                textlen--;
              }
              if ((ter->str[textlen-1] & 0xC0) == 0xC0)
              {
                /* First byte of multibyte sequence */
                ((char *)ter->str)[textlen-1]=0x00;
                break;
              }
            } while(1);
          }
          break;
      }
      break;
    case SDL_TEXTINPUT:
    {
      if (strlen(ev.text.text) == 0 || ev.text.text[0] == '\n')
        break;

      //fprintf(stderr, "sizeof(ter->str) = %d\n", ter->strsize);
      //fprintf(stderr, "strlen evtxt = %ld\n", strlen(ev.text.text));
      /*const char *c = ev.text.text;
      do {
        fprintf(stderr, "%02x ", *c);
        c++;
      } while (*c != 0);*/

      if (strlen(ter->str) + strlen(ev.text.text) < ter->strsize)
      {
        //fprintf(stderr, "YAY\n");
        SDL_strlcat((char *)ter->str, ev.text.text, ter->strsize);
        ter->needs_redraw = true;
      }
      break;
    }

    case SDL_TEXTEDITING:
      /*fprintf(stderr, "text editing \"%s\", selected range (%d, %d)\n",
          ev.edit.text, ev.edit.start, ev.edit.length);*/
      strcpy(Text_Edit_Rect::markedText, ev.edit.text);
      Text_Edit_Rect::comp_start_point = ev.edit.start;
      break; 
    }
  }
}

// We need a callback that handles what happens when this rect has been
// clicked, with state control (editing). proto code follows
//
// Add code to change the cursor to I when a Text_Edit_Rect rect is
// hovered over. 
int Text_Edit_Rect::clicked_callback(void *data)
{
  Text_Edit_Rect *ter = (Text_Edit_Rect *)data;
  // the rect has been clicked (it is clickable_text, after all.
  /* Different handling based on editing */
  if (!ter->editing)
  {
    /* if we re not editing, then we will start editing. Position the
     * cursor at the char nearest where the click occurred.*/

    // start the cursor
    // Provide the cursor with its location: For now, just put it at the
    // end of the string in the rect
    //fprintf(stderr, "WOOGOO\n");
    SDL_Rect *r = &Text_Edit_Rect::cursor.rect;
    *r = ter->rect;
    r->x += strlen(ter->str) * CHAR_WIDTH;
    Text_Edit_Rect::cursor.start_timer();
    SDL_StartTextInput();
  }
  else
  {
    //fprintf(stderr, "WOOGOO2\n");
    /* IF a click happened inside the rect while we were editing, it
     * ccould have been a number of things:
     *
     * . The user is repositioning the cursor.
     * . is highlighting text for a selection. We can determine this if
     * the button is continueously held down.
     * . ?*/

    // Don't deal those complicated rules yet. Just stop editing if we
    // clicked again
    SDL_StopTextInput();
    Text_Edit_Rect::cursor.stop_timer();
  }

  ter->editing = !ter->editing;
  //fprintf(stderr, "editing = %d\n", ter->editing);
}

void Text_Edit_Rect::one_time_draw(SDL_Surface *screen)
{
  // If there's any issues mixing renderer code with screen code in this
  // function, just use 2 SDL_FillRects, with an adjusted rect for the
  // border.
  // fill the BG color rect, then a border around it
  SDL_Rect outer = rect;
  outer.x -= 2;
  outer.w += 4;
  outer.y -= 2;
  outer.h += 2;
  SDL_Rect io = rect;
  io.x -= 1;
  io.w += 2;
  io.y -= 1;

 /* Utility::set_render_color_rgb(::render->sdlRenderer,
      Colors::Interface::color[Colors::Interface::Type::text_bg]);*/
  //SDL_RenderFillRect(::render->sdlRenderer, &rect);
  SDL_FillRect(screen, &outer, Colors::white);
  SDL_FillRect(screen, &io, Colors::Interface::color[Colors::Interface::Type::text_bg]);
  // Draw border
  //Utility::set_render_color_rgb(::render->sdlRenderer,
  //    Colors::white);
  //SDL_RenderDrawRect(::render->sdlRenderer, &rect);
}

void Text_Edit_Rect::draw(Uint32 color, bool prefill/*=true*/,
            bool Vflip/*=false*/,
            bool Hflip/*=false*/, SDL_Surface *screen/*=::render->screen*/)
{
  if (!cursor.toggle)
    needs_redraw = true;
  if (needs_redraw)
  {
    one_time_draw(screen);
    needs_redraw = false;
    size_t mtlen = strlen(markedText);
    markedRect = rect;
    markedRect.x = rect.x + (strlen(str) * CHAR_WIDTH);
    markedRect.w = mtlen * CHAR_WIDTH;
    cursor.rect.x = markedRect.x;
    //const char *c = str;
    //fprintf(stderr, "REDRAW\n");
    /*fprintf(stderr, "str = %s; strlen = %d\n", str, strlen(str));
    do {
      fprintf(stderr, "%02x ", *c);
      c++;
    } while (*c != 0);*/
    sdlfont_drawString(screen, rect.x, rect.y, str, color,
        Colors::Interface::color[Colors::Interface::Type::text_bg], prefill, Vflip, Hflip);

    if (markedText[0])
    {
      SDL_Rect underlineRect;

      if (comp_start_point)
      {
        char *p = utf8_advance(markedText, comp_start_point);
        char c = 0;
        if (!p)
          p = &markedText[mtlen];

        c = *p;
        *p = 0;
        // Calculate the size of text. For us that's easy
        cursor.rect.x += (mtlen * CHAR_WIDTH);
        *p = c;
      }
      // draw the marked text
      sdlfont_drawString(screen, markedRect.x, markedRect.y, markedText, color,
          Colors::Interface::color[Colors::Interface::Type::text_bg], prefill, Vflip, Hflip);

      underlineRect = markedRect;
      underlineRect.y += (markedRect.h - 2);
      underlineRect.h = 2;
      underlineRect.w = (mtlen * CHAR_WIDTH);

      //SDL_SetRenderDrawColor(r, 0,0,0,0);
      SDL_FillRect(screen, &markedRect, Colors::Interface::color[Colors::Interface::Type::text_fg]);

      SDL_SetTextInputRect(&markedRect);
    }
  }
  if (editing)
  {// no worries. the cursor is only drawn based on internal logic
    //fprintf(stderr, "drawing cursor at (%d,%d)\n", cursor.rect.x, cursor.rect.y);
    cursor.draw(screen, cursor.rect.x, cursor.rect.y,
                Colors::green);
  }
}
