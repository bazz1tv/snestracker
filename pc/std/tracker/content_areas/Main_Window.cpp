#include "Main_Window.h"
#include <getopt.h>
#include "utility.h"
#include "Render.h"
#include "Menu_Bar.h"
#include "Screen.h"
#include "Tracker.h"

#define L_FLAG 0
#define R_FLAG 1

#define PC_STR "PC: $%04x  "
#define PC_X MEMORY_VIEW_X+8*12
#define PC_Y MEMORY_VIEW_Y-10

Main_Window::Main_Window(int &argc, char **argv, Tracker *tracker) :
  song_title_label("Song Title:"),
  song_title(22, song_title_str, sizeof(song_title_str)),
  tracker(tracker),
  instrpanel(tracker->instruments)
{
  int x,y,xx,yy;

  if (::render->screen == NULL)
  {
    fprintf(stderr, "Debugger::MainWindows::::render->screen is Null\n");
    exit(1);
  }
  
  song_title_str[0] = 0;

  x = xx = 150; //(SCREEN_WIDTH / 2) - ((strlen("Song Title") * CHAR_WIDTH) / 2 );
  y = yy = 50;

  song_title_label.rect.x = x;
  song_title_label.rect.y = y;

  y += CHAR_HEIGHT + 2;
  song_title.rect.x = x;
  song_title.rect.y = y;

  instrpanel.set_coords(x + song_title.rect.w + (CHAR_WIDTH * 2), yy);
}

int Main_Window::Gain::change(void *dblnewgain)
{
  ::player->gain_has_changed = true;
  return 0;
}

void Main_Window::draw_memory_outline()
{
  tmprect.x = MEMORY_VIEW_X-1;
  tmprect.y = MEMORY_VIEW_Y-1;
  tmprect.w = 512+2;
  tmprect.h = 512+2;
  SDL_SetRenderDrawColor(::render->sdlRenderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(::render->sdlRenderer, &tmprect);
}

void Main_Window::one_time_draw()
{
  // draw one-time stuff
  SDL_FillRect(::render->screen, NULL, Colors::Interface::color[Colors::Interface::Type::bg]);
  song_title_label.draw(::render->screen);

  instrpanel.one_time_draw();
}

void Main_Window::draw()
{
  time_cur = SDL_GetTicks();

  //sdlfont_drawString(::render->screen, MEMORY_VIEW_X, MEMORY_VIEW_Y-10, "spc memory:");

  // The following are correlated from i and tmp. DO NOT MESS WITH THAT
  // base height
  i = 32 + SCREEN_Y_OFFSET;  
  if (player->has_no_song) 
  {
    //fprintf(stderr, "HERE!\n");
    song_title.draw(Colors::Interface::color[Colors::Interface::Type::text_fg]);
    instrpanel.draw(::render->screen);
    SDL_UpdateTexture(::render->sdlTexture, NULL, ::render->screen->pixels, ::render->screen->pitch);
    SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, 0);
    SDL_RenderClear(::render->sdlRenderer);
    SDL_RenderCopy(::render->sdlRenderer, ::render->sdlTexture, NULL, NULL);
    //draw_memory_outline();
    SDL_RenderPresent(::render->sdlRenderer);
    return; 
  }  

  draw_voices_pitchs(); // tmp is fucked with inside this function. DONT MESS
  draw_voices_volumes();
  draw_main_volume();
  draw_echo_volume();

  SDL_UpdateTexture(::render->sdlTexture, NULL, ::render->screen->pixels, ::render->screen->pitch);
  SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, 0);
  SDL_RenderClear(::render->sdlRenderer);
  SDL_RenderCopy(::render->sdlRenderer, ::render->sdlTexture, NULL, NULL);
  // stuff that renders direct to renderer must happen after the ::render->screen copy
  draw_memory_outline();

  gain.slider->draw();

  SDL_RenderPresent(::render->sdlRenderer);
  time_last = time_cur;
  
  is_first_run = false;
}

void Main_Window::check_quit(SDL_Event &ev)
{
  switch (ev.type)
  {
    case SDL_QUIT:
    quitting = true;
    break;

    case SDL_KEYDOWN:
    if (ev.key.keysym.sym == SDLK_ESCAPE)
    {
      quitting = true;
    }
  }
}

int Main_Window::receive_event(SDL_Event &ev)
{
  check_quit(ev);

  handle_text_edit_rect_event(ev, &song_title);
  instrpanel.event_handler(ev);

  // DIRTY :( ITS IMPORTANT THAT WE CHECK THE DBLCLICK EVENTS AFTER THE ABOVE
  dblclick::check_event(&ev);
  /*if (gain.slider)
  {
    bool a=gain.slider->receive_event(ev);
    if (a) return;
  }*/
  
  switch (ev.type)
  {
  case SDL_QUIT:
    quitting = true;
    break;
  case SDL_MOUSEMOTION:
    {
      mouse::x = ev.motion.x; mouse::y = ev.motion.y;

      if (ev.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        //::voice_control.checkmouse_mute((Uint16&)ev.motion.x, (Uint16&)ev.motion.y);
      }
      if (mode == MODE_NAV || mode == MODE_EDIT_MOUSE_HEXDUMP)
      {
        if (  ev.motion.x >= MEMORY_VIEW_X && 
            ev.motion.x < MEMORY_VIEW_X + 512 &&
            ev.motion.y >= MEMORY_VIEW_Y &&
            ev.motion.y < MEMORY_VIEW_Y + 512)
        {
        }
      }
    
    }
    break;
  case SDL_KEYDOWN:
    {
      int scancode = ev.key.keysym.sym;
      int mod = ev.key.keysym.mod;

      switch(scancode)
      {
        case SDLK_s:
        break;
        case SDLK_UP:
          break;
        case SDLK_DOWN:
        break;
        default:break;
      }

      if (mode == MODE_NAV)
      {
        switch (scancode)
        {
          case SDLK_d:
          case SDLK_SLASH:
          break;

          case SDLK_TAB:
            if (mod & KMOD_SHIFT)
            {
            }
            else 
            {
            }
          break;
        }
      }
             
    } break;
    case SDL_USEREVENT:
    {
      if (ev.user.code == UserEvents::mouse_react)
      {
        SDL_Event *te = (SDL_Event *)ev.user.data1; // the mouse coordinates at time of double click
      }
    } break;
    case SDL_MOUSEWHEEL:
    {
    } break;
    case SDL_MOUSEBUTTONDOWN:           
    {
 
    } break;
  default:
    break;
  }
}

void Main_Window::run()
{
  
}

void Main_Window::draw_voices_pitchs()
{
  tmp = i+10; // y 
  sdlfont_drawString2(::render->screen, MEMORY_VIEW_X+520, tmp, "Voices pitches:"); // Colors::white);
  tmp += 9;
  
  tmprect.x=MEMORY_VIEW_X+520;
  tmprect.y=tmp;
  tmprect.w=::render->screen->w-tmprect.x;
  tmprect.h=8*8;
  SDL_FillRect(::render->screen, &tmprect, Colors::Interface::color[Colors::Interface::Type::bg]);
  tmprect.w=5;
  tmprect.h = 5;
  for (i=0; i<8; i++)
  {
    
    unsigned short pitch = (player->spc_read_dsp(2+(i*0x10)) | (player->spc_read_dsp(3+(i*0x10))<<8)) & 0x3fff; 
    // I believe pitch is max 0x3fff but kirby is using higher values for some unknown reason...
    //Uint32 gray = Colors::gray;
    Uint32 *cur_color= &Colors::gray;

    uint8_t voice_base_addr = (i*0x10);
    uint8_t outx = player->spc_read_dsp(voice_base_addr+0x09);
    uint8_t envx = player->spc_read_dsp(voice_base_addr+0x08);

   

    if (outx || envx) 
    {
      cur_color = &Colors::voice[i];
    }


    if (::voice_control.is_muted(i))
    {
      /* I would use sub color here to
      get a view of the activity but the mute happens at the DSP level
      it invokes KOFF on the muted channel making it impossible to get its
      real status without hacking it at deep level */
      /* i will leave out the pointers in the color logic still ... altho
      the pointers are faster and can be report::used if the pointed-to-value is
      not changed
      :P pointers are back */
      cur_color = &Colors::nearblack;
    }

    int x =MEMORY_VIEW_X+520;
    int y = tmp+ (i*8);
    sprintf(tmpbuf,"%d:",i);
    sdlfont_drawString(::render->screen, x, y, tmpbuf, *cur_color);
    if (is_first_run && i == 0)
    {
      Screen::voice0pitch.x = x;
      Screen::voice0pitch.y = y;
    }
    
    tmprect.y= tmp+(i*8)+2;
    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.x += pitch*(::render->screen->w-tmprect.x-20)/((0x10000)>>2);
    if (::voice_control.is_muted(i))
      SDL_FillRect(::render->screen, &tmprect, Colors::nearblack);
    else
      SDL_FillRect(::render->screen, &tmprect, Colors::Interface::color[Colors::Interface::Type::text_fg]);
    
  }
}

void Main_Window::draw_voices_volumes()
{
  tmp += 9*8;

  sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520, tmp, "Voices volumes:");
  sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520+(16*8), tmp, "Left", Colors::Interface::color[Colors::Interface::Type::voice_volume_left]);      

  sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520+(20*8)+4, tmp, "Right", Colors::Interface::color[Colors::Interface::Type::voice_volume_right]);
  sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520+(26*8), tmp, "Gain", Colors::Interface::color[Colors::Interface::Type::voice_gain]);
  tmp += 9;

  tmprect.x=MEMORY_VIEW_X+520;
  tmprect.y=tmp;
  tmprect.w=::render->screen->w-tmprect.x;
  tmprect.h=10*11;
  SDL_FillRect(::render->screen, &tmprect, Colors::Interface::color[Colors::Interface::Type::bg]);   
  tmprect.w=2;
  tmprect.h=2;

  for (i=0; i<8; i++)
  {
    Uint32 c1 = Colors::white, c2 = Colors::gray, c3 = Colors::magenta;
    Uint32 *Color1=&c1, *Color2 = &c1;
    
    unsigned char is_inverted=0;
    unsigned char left_vol = player->spc_read_dsp(0+(i*0x10));
    unsigned char right_vol = player->spc_read_dsp(1+(i*0x10));
    unsigned char adsr1 = player->spc_read_dsp(5+(i*0x10));
    unsigned char gain;
    if (adsr1 & 0x80) gain=0;
    else gain = player->spc_read_dsp(7+(i*0x10));

    
    
    //Value -128 can be safely report::used
    // for lefty vol
    if (left_vol >= 0x80)
    {
      left_vol = (left_vol ^ 0xff) + 1;
      is_inverted = 1 << L_FLAG;
      Color1 = &c2;
    }
    // for right vol
    if (right_vol >= 0x80)
    {
      right_vol = (right_vol ^ 0xff) + 1;
      is_inverted |= 1 << R_FLAG;
      Color2 = &c2;
    }
    sprintf(tmpbuf,"%d", i);
    int x = MEMORY_VIEW_X+520;
    int y = tmp + (i*10);
    Uint32 *color;

    if (::voice_control.is_muted(i))
      color = &Colors::nearblack;
    else 
      color = &Colors::voice[i];

    sdlfont_drawString(::render->screen, x, y, tmpbuf, *color);
    if (is_first_run && i == 0)
    {
      Screen::voice0vol.x = x;
      Screen::voice0vol.y = y;  
    }
    
    sprintf(tmpbuf,"\x1");
    if (::voice_control.is_muted(i))
    {
      Colors::subtractp(Color1,0x60);
      Colors::subtractp(Color2,0x60);
    }
    sdlfont_drawString2c(::render->screen, MEMORY_VIEW_X+520 + 8, tmp + (i*10), tmpbuf, *Color1, *Color2);
    

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10);
    tmprect.w = left_vol*(::render->screen->w-tmprect.x-20)/255;

    // L volume
    if (::voice_control.is_muted(i))
      color = &Colors::dark_yellow;
    else color = &Colors::yellow;
    SDL_FillRect(::render->screen, &tmprect, *color);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(::render->screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+3;
    
    if (::voice_control.is_muted(i))
      color = &Colors::dark_cyan;
    else color = &Colors::cyan;
    SDL_FillRect(::render->screen, &tmprect, *color);

    // Gain needs customization
    if (!(gain & 0x80))
    {
      // direct mode
      gain = gain & 0x7f;
    }
    else
    {
      gain = gain & 0x1f;
    }


    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = gain * (::render->screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+6;
    if (::voice_control.is_muted(i))
    {
      Uint8 r1,g1,b1;
      SDL_GetRGB(c3, ::render->screen->format, &r1, &b1, &g1);
      c3 = SDL_MapRGB(::render->screen->format,r1-0x60,g1-0x60,b1-0x60);
    }

    SDL_FillRect(::render->screen, &tmprect, c3);
  }
}

void Main_Window::draw_main_volume()
{
  i=9;
  // 
  {
    Uint32 *Color1=&Colors::white, *Color2 = &Colors::white;
    Uint32 *thecolor = &Colors::gray;
    unsigned char is_inverted=0;
    unsigned char left_vol = player->spc_read_dsp(dsp_reg::mvol_l);
    unsigned char right_vol = player->spc_read_dsp(dsp_reg::mvol_r);
    //unsigned char gain = player->spc_read_dsp(7+(i*0x10));

    //Value -128 canNOT be safely report::used
    // for left vol
    if (left_vol >= 0x80)
    {
      left_vol = (left_vol ^ 0xff) + 1;
      is_inverted = 1 << L_FLAG;
    }
    // for right vol
    if (right_vol >= 0x80)
    {
      right_vol = (right_vol ^ 0xff) + 1;
      is_inverted |= 1 << R_FLAG;
    }

    if (is_inverted & (1 << L_FLAG) )
    {
      if (left_vol == 0x80)
        Color1 = &Colors::red; // this is bad value according to FullSNES
      else Color1 = thecolor;
    }
    if (is_inverted & (1 << R_FLAG) )
    {
      if (right_vol == 0x80)
        Color2 = &Colors::red; // this is bad value according to FullSNES
      Color2 = thecolor;
    }

    sprintf(tmpbuf,"M");
    sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520, tmp + (i*10), tmpbuf);
    sprintf(tmpbuf,"\x1");
    sdlfont_drawString2c(::render->screen, MEMORY_VIEW_X+520+8, tmp + (i*10), tmpbuf, *Color1, *Color2);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10)+1;
    tmprect.w = left_vol*(::render->screen->w-tmprect.x-20)/255;

    SDL_FillRect(::render->screen, &tmprect, Colors::yellow);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(::render->screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+4;
    
    SDL_FillRect(::render->screen, &tmprect, Colors::cyan);
  }
}

void Main_Window::draw_echo_volume()
{
  i++;
  {
    Uint32 Color1=Colors::white;
    Uint32 Color2=Colors::white;// = &Colors::white;
    Uint32 *leftbarvol_col=&Colors::yellow;
    Uint32 *rightbarvol_col=&Colors::cyan;
    //Uint32 thecolor = &
    unsigned char is_inverted=0;
    unsigned char left_vol = player->spc_read_dsp(dsp_reg::evol_l);
    unsigned char right_vol = player->spc_read_dsp(dsp_reg::evol_r);
    
    //Value -128 can be safely report::used
    // for left vol
    if (left_vol >= 0x80)
    {
      left_vol = (left_vol ^ 0xff) + 1;
      is_inverted = 1 << L_FLAG;
    }
    // for right vol
    if (right_vol >= 0x80)
    {
      right_vol = (right_vol ^ 0xff) + 1;
      is_inverted |= 1 << R_FLAG;
    }

    if (is_inverted & (1 << L_FLAG) )
    {
      Color1 = Colors::gray;
    }
    if (is_inverted & (1 << R_FLAG) )
    {
      Color2 = Colors::gray;
    }

    //
    Uint32 *c;
    sprintf(tmpbuf,"E");
    if (is_first_run)
    {
      Screen::echoE.y = tmp+(i*10);
    }

    if (player->spc_emu()->is_echoing())
    {
      c = &Colors::white;
    }
    else
    {
      c = &Colors::nearblack;
      Colors::subtractp(&Color1, 0x60);
      Colors::subtractp(&Color2, 0x60);
      leftbarvol_col=&Colors::dark_yellow;
      rightbarvol_col=&Colors::dark_cyan;
    }
    sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520, tmp + (i*10), tmpbuf, *c);
    sprintf(tmpbuf,"\x1");
    sdlfont_drawString2c(::render->screen, MEMORY_VIEW_X+520+8, tmp + (i*10), tmpbuf, Color1, Color2);
    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10)+1;
    tmprect.w = left_vol*(::render->screen->w-tmprect.x-20)/255;

    SDL_FillRect(::render->screen, &tmprect, *leftbarvol_col);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(::render->screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+4;
    
    SDL_FillRect(::render->screen, &tmprect, *rightbarvol_col);
  }
}

#undef L_FLAG
#undef R_FLAG
