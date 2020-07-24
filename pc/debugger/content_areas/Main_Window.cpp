#include "Main_Window.h"
#include <getopt.h>
#include "utility.h"
#include "Render.h"
#include "Menu_Bar.h"

#define L_FLAG 0
#define R_FLAG 1

#define PC_STR "PC: $%04x  "
#define PC_X MEMORY_VIEW_X+8*12
#define PC_Y MEMORY_VIEW_Y-10

int Main_Window::Gain::change(void *dblnewgain)
{
  ::player->gain_has_changed = true;
  return 0;
}
int Main_Window::Tempo::change(void *dblnewtempo)
{
  double v = *(double*)dblnewtempo;
  DEBUGLOG("tempo = %f", v);
  ::player->set_tempo(v);
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

void Main_Window::draw()
{

  if (!g_cfg.novideo)
  {
    time_cur = SDL_GetTicks();

    do_scroller(time_cur - time_last);
    sdlfont_drawString(::render->screen, MEMORY_VIEW_X, MEMORY_VIEW_Y-10, "spc memory:");

    fade_arrays();      
    
    // draw the memory read/write display area
    report::memsurface.draw(::render->screen);

    draw_block_usage_bar();

    // The following are correlated from i and tmp. DO NOT MESS WITH THAT
    // base height
    i = 32 + SCREEN_Y_OFFSET;  
    if (player->has_no_song) 
    {
      draw_menu_bar();
      SDL_UpdateTexture(::render->sdlTexture, NULL, ::render->screen->pixels, ::render->screen->pitch);
      SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, 0);
      SDL_RenderClear(::render->sdlRenderer);
      SDL_RenderCopy(::render->sdlRenderer, ::render->sdlTexture, NULL, NULL);
      draw_memory_outline();
      return; 
    }  

    draw_mouse_address();
    draw_program_counter();
    draw_voices_pitchs(); // tmp is fucked with inside this function. DONT MESS
    draw_voices_volumes();
    draw_main_volume();
    draw_echo_volume();
    draw_mouseover_hexdump();
    draw_porttool();
    draw_time_and_echo_status();

    scroll_track_tags();
    
    if (mode == MODE_EDIT_MOUSE_HEXDUMP)
    {
      mouseover_hexdump_area.draw_cursor(::render->screen, Colors::green);
    }
    else if (mode == MODE_EDIT_APU_PORT)
    {
      port_tool.draw_cursor(::render->screen, Colors::green);
    }

    // toggle should be 0 ALWAYS when deactivated
    if (main_memory_area.memcursor.is_active())
    {
      if (main_memory_area.memcursor.is_toggled())
      {
        report_cursor(mouseover_hexdump_area.addr_being_edited);
      }
      else report::restore_color(mouseover_hexdump_area.addr_being_edited);
    }

    if (mouse::show)
    {
      if (mouse::x < (::render->screen->w-40) && mouse::y < (::render->screen->h - 8))
      { 
        sprintf(tmpbuf, "(%d,%d)", mouse::x, mouse::y);
        sdlfont_drawString(::render->screen, mouse::x, mouse::y, tmpbuf);
      }
    }

    if (::brrcontext.menu.is_active)
      ::brrcontext.menu.draw(::render->screen);

		draw_menu_bar();
    
    SDL_UpdateTexture(::render->sdlTexture, NULL, ::render->screen->pixels, ::render->screen->pitch);
    SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, 0);
    SDL_RenderClear(::render->sdlRenderer);
    SDL_RenderCopy(::render->sdlRenderer, ::render->sdlTexture, NULL, NULL);
    // stuff that renders direct to renderer must happen after the ::render->screen copy
    draw_memory_outline();

    gain.slider->draw();
    tempo.slider->draw();

    time_last = time_cur;
    if (g_cfg.nice) {  SDL_Delay(100); }
  }
  else
  {
    draw_mouse_address();
    draw_menu_bar();
    
    
    SDL_UpdateTexture(::render->sdlTexture, NULL, ::render->screen->pixels, ::render->screen->pitch);
    SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, 0x00);
    SDL_RenderClear(::render->sdlRenderer);
    SDL_RenderCopy(::render->sdlRenderer, ::render->sdlTexture, NULL, NULL);
    // stuff that renders direct to renderer must happen after the ::render->screen copy

    gain.slider->draw();
    tempo.slider->draw();
  }
  is_first_run = false;
}

void Main_Window::check_quit(SDL_Event &ev)
{
  switch (ev.type)
  {
    case SDL_QUIT:
    printf ("penis4\n");
    quitting = true;
    break;

    case SDL_KEYDOWN:
    if (ev.key.keysym.sym == SDLK_ESCAPE)
    {
      if (!locked() && mode != MODE_EDIT_APU_PORT)
      {
        quitting = true;
      }
    }
  }
}

int Main_Window::receive_event(SDL_Event &ev)
{
  /* menu bar */
  int r;

	if (Context_Menu::currently_active_context_menu)
		return Context_Menu::currently_active_context_menu->receive_event(ev);

	if ((r=BaseD::menu_bar_events(ev)))
  {
    switch (r)
    {
      default:break;
    }
    return 0;
  }
  check_quit(ev);

  if (player->has_no_song) return 0;

  if (gain.slider && tempo.slider)
  {
    bool a=gain.slider->receive_event(ev);
    bool b = tempo.slider->receive_event(ev);
    if (a || b) return 0;
  }
  
  dblclick::check_event(&ev);

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
          voice_control.checkmouse_mute((Uint16&)ev.motion.x, (Uint16&)ev.motion.y);
        }
        if (mode == MODE_NAV || mode == MODE_EDIT_MOUSE_HEXDUMP)
        {
          if (  ev.motion.x >= MEMORY_VIEW_X && 
              ev.motion.x < MEMORY_VIEW_X + 512 &&
              ev.motion.y >= MEMORY_VIEW_Y &&
              ev.motion.y < MEMORY_VIEW_Y + 512)
          {
            int x, y;
            x = ev.motion.x;
            y = ev.motion.y;

            x-= MEMORY_VIEW_X;
            y -= MEMORY_VIEW_Y;
            x /= 2;
            y /= 2;
            
            main_memory_area.mouse_addr = y*256+x;
            if (!locked()) {
              mouseover_hexdump_area.set_addr(main_memory_area.mouse_addr);
            }
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
          voice_control.checkmouse((Uint16&)mouse::x, (Uint16&)mouse::y, SDL_BUTTON_RIGHT); 
        break;
        case SDLK_UP:
          //BaseD::cursors->next();
          break;
        case SDLK_DOWN:
          //BaseD::cursors->prev();
        break;
        default:break;
      }

      if (scancode == SDLK_m)
      {
        main_memory_area.memcursor.toggle_disable();
      }
      if (scancode == SDLK_k)
      {
      }
      else if (scancode == SDLK_l)
      {
      }
      if (scancode == SDLK_n)
      {
        static int val=0;
        val = !val;
        player->spc_write_dsp(dsp_reg::kon,val);
      }
      else if (scancode == SDLK_m)
      {
        static int val=0;
        val = !val;
        player->spc_write_dsp(dsp_reg::koff,val);
        BaseD::switch_mode(GrandMode::INSTRUMENT);
      }
      if (ev.key.keysym.sym == SDLK_u)
      {
        player->emu()->seek(10000);
      }
      else if (ev.key.keysym.sym == SDLK_i)
      {
        BaseD::switch_to_instrument(NULL);
      }
      else if (scancode == SDLK_o)
      {
        player->filter_is_active = !player->filter_is_active;
        fprintf(stderr, "filter_is_active = %d", player->filter_is_active);
      }
      else if (scancode == SDLK_SPACE)
      {
        BaseD::toggle_pause();
      }
      else if (scancode == SDLK_r)
      {
        restart_current_track();
        // in the program .. this would have to change otherwise
      }
      if (mode == MODE_NAV)
      {
        // VOICES!!! 
        if ((scancode >= '0') && (scancode <= '9'))
        {
          uint8_t i = scancode - '0';
          if (!i)
            voice_control.toggle_mute_all();
          if (i > 0 && i < 9)
            voice_control.toggle_mute(i); // channel num
        }

        switch (scancode)
        {
          case SDLK_d:
          case SDLK_SLASH:
          BaseD::switch_mode(GrandMode::DSP_MAP);
          break;

          case SDLK_TAB:
            if (mod & KMOD_SHIFT)
            {
              prev_track25();
            }
            else 
            {
              next_track25();
            }
            this->reload();
          break;
        }


        if (scancode == SDLK_LEFT)
          prev_track();
        else if (scancode == SDLK_RIGHT)
          next_track();
        else if (scancode == SDLK_c)
        {
          mouse::show = !mouse::show;
        }
        
        else if (ev.key.keysym.sym == SDLK_e)
          player->spc_emu()->toggle_echo();
        else if (ev.key.keysym.sym == SDLK_RETURN && // click in memory view. Toggle lock
          (mouse::x >= MEMORY_VIEW_X && 
              mouse::x < MEMORY_VIEW_X + 512 &&
              mouse::y >= MEMORY_VIEW_Y &&
              mouse::y < MEMORY_VIEW_Y + 512 ) )
        {
          mouseover_hexdump_area.highnibble = 1;
          mouseover_hexdump_area.res_x = 0;
          mouseover_hexdump_area.res_y = 0;

          mode = MODE_EDIT_MOUSE_HEXDUMP;
          mouseover_hexdump_area.submode = Mouse_Hexdump_Area::EASY_EDIT;
          // order matters .. call here: 
          lock();
          
          mouseover_hexdump_area.cursor.start_timer();
        }
        if (ev.key.keysym.sym == SDLK_ESCAPE)
        {
          if (locked())
          {
            DEBUGLOG("eegbeb");
            unlock(); 
          }
        }
      }
      else if (mode == MODE_EDIT_MOUSE_HEXDUMP)
      {
        int scancode = ev.key.keysym.sym;
        if (ev.key.keysym.mod & (CMD_CTRL_KEY))  // GUI in SDL2
        {
          if (scancode == SDLK_LEFT)
            prev_track();
          else if (scancode == SDLK_RIGHT)
            next_track();
        }
        else if (scancode == 'h' || scancode == 'H')
        {
          mouseover_hexdump_area.horizontal = !mouseover_hexdump_area.horizontal;
        }
        else if ( ((scancode >= '0') && (scancode <= '9')) || ((scancode >= 'A') && (scancode <= 'F')) || 
          ((scancode >= 'a') && (scancode <= 'f')) )
        {
          uint32_t i=0;
          Uint16 addr = mouseover_hexdump_area.addr_being_edited;
          
          if ((scancode >= '0') && (scancode <= '9'))
            i = scancode - '0';
          else if ((scancode >= 'A') && (scancode <= 'F'))
            i = (scancode - 'A') + 0x0a;
          else if ((scancode >= 'a') && (scancode <= 'f'))
            i = (scancode - 'a') + 0x0a;  

          // test reg is unimplemented but i'll leave this here..
          if ( IS_SPECIAL_ADDRESSES(addr) )
          {
            // only update the buffer the first time.. if we haven't started writing in a new value
            if (!mouseover_hexdump_area.draw_tmp_ram)
            {
              if (addr == 0xf1 || (addr >= 0xf4 && addr <= 0xf7)  )
                mouseover_hexdump_area.tmp_ram = IAPURAM[addr];
              else mouseover_hexdump_area.tmp_ram = player->spc_read(addr);

              fprintf(stderr, "tmpram = 0x%02x\n", mouseover_hexdump_area.tmp_ram);
            }
          }
          else 
          {
            if (mouseover_hexdump_area.draw_tmp_ram) mouseover_hexdump_area.draw_tmp_ram = 0;
            mouseover_hexdump_area.tmp_ram = player->spc_read(addr);
          }
          if (mouseover_hexdump_area.highnibble)
          {
            i <<= 4;
            i &= 0xf0;
            mouseover_hexdump_area.tmp_ram &= 0x0f;
          }
          else
          {
            i &= 0x0f;
            mouseover_hexdump_area.tmp_ram &= 0xf0;
          }

          mouseover_hexdump_area.tmp_ram |= i;

          maybe_write_to_mem();
          
          if (mouseover_hexdump_area.horizontal) mouseover_hexdump_area.inc_cursor_pos();
          
        }
        else if (scancode == SDLK_TAB)
        {
          mouseover_hexdump_area.inc_cursor_pos();
          mouseover_hexdump_area.inc_cursor_pos();
        }
        else if (scancode == SDLK_BACKSPACE)
        {
          // eh
          int i = mouseover_hexdump_area.address+(mouseover_hexdump_area.res_y*8)+mouseover_hexdump_area.res_x;
          while (i < (0x10000) )
          {
            player->spc_write(i-1, player->spc_read(i));
            i++;
          }
          mouseover_hexdump_area.dec_cursor_pos();
          mouseover_hexdump_area.dec_cursor_pos();
          mouseover_hexdump_area.highnibble=1;
        }
        else if (scancode == SDLK_DELETE)
        {
          // eh
          int i = mouseover_hexdump_area.address+(mouseover_hexdump_area.res_y*8)+mouseover_hexdump_area.res_x;
          while (i < (0x10000) )
          {
            player->spc_write(i, player->spc_read(i+1));
            i++;
          }
          mouseover_hexdump_area.highnibble=1;
        }
        else if (scancode == SDLK_LEFT)
        {
          mouseover_hexdump_area.dec_cursor_pos();
        }
        else if (scancode == SDLK_RIGHT)
        {
          mouseover_hexdump_area.inc_cursor_pos();
        }
        else if (scancode == SDLK_UP)
        {
          mouseover_hexdump_area.dec_cursor_row();
        }
        else if (scancode == SDLK_DOWN)
        {
          mouseover_hexdump_area.inc_cursor_row();
        }

        if (ev.key.keysym.sym == SDLK_ESCAPE)
        {
          exit_edit_mode();
        }
        else if (scancode == SDLK_RETURN)
        {
          maybe_write_to_mem(true);
        }
      }   
      else if (mode == MODE_EDIT_APU_PORT)
      {

        int scancode = ev.key.keysym.sym;

        if (scancode == 'h' || scancode == 'H')
        {
          port_tool.horizontal = !port_tool.horizontal;
        }

        if ( ((scancode >= '0') && (scancode <= '9')) || ((scancode >= 'A') && (scancode <= 'F')) || 
          ((scancode >= 'a') && (scancode <= 'f')))
        {
          Uint8 i;

          i = Utility::scancode_to_hex(scancode); 

          if (port_tool.highnibble)
          {
            i <<= 4;
            i &= 0xf0;
            port_tool.tmp[port_tool.portnum] &= 0x0f;
          }
          else
          {
            i &= 0x0f;
            port_tool.tmp[port_tool.portnum] &= 0xf0;
          }

          port_tool.tmp[port_tool.portnum] |= i;
          
          if (port_tool.horizontal) port_tool.inc_cursor_pos();
        }
        else if (scancode == SDLK_TAB)
        {
          port_tool.inc_cursor_pos();
          port_tool.inc_cursor_pos();
        }
        else if (scancode == SDLK_BACKSPACE)
        {
          if (port_tool.highnibble == 0)
          {
            port_tool.tmp[port_tool.portnum] &= 0x0f;
            port_tool.dec_cursor_pos();
          }
        }
        else if (scancode == SDLK_DELETE)
        {
          if (port_tool.highnibble)
          {
            port_tool.tmp[port_tool.portnum] &= 0x0f;
          }
          else port_tool.tmp[port_tool.portnum] &= 0xf0;
        }
        else if (scancode == SDLK_LEFT)
        {
          port_tool.dec_cursor_pos();
        }
        else if (scancode == SDLK_RIGHT)
        {
          port_tool.inc_cursor_pos();
        }
        else if (scancode == SDLK_UP)
        {
          if (port_tool.highnibble)
          {
            port_tool.tmp[port_tool.portnum] += 0x10;
          }
          else
          {
            Uint8 tmp = port_tool.tmp[port_tool.portnum] + 1;
            port_tool.tmp[port_tool.portnum] = tmp;
          }
        }
        else if (scancode == SDLK_DOWN)
        {
          if (port_tool.highnibble)
          {
            port_tool.tmp[port_tool.portnum] -= 0x10;
          }
          else
          {
            Uint8 tmp = port_tool.tmp[port_tool.portnum] - 1;
            tmp &= 0x0f;
            port_tool.tmp[port_tool.portnum] &= 0xf0;
            port_tool.tmp[port_tool.portnum] |= tmp;
          }
        }

        if (ev.key.keysym.sym == SDLK_ESCAPE)
        {
          mode = MODE_NAV;
          mouseover_hexdump_area.cursor.stop_timer();
          port_tool.reset_port();
        }
        else if (ev.key.keysym.sym == SDLK_RETURN)
        {
          port_tool.write();
        }
      }       
    }
      break;

    case SDL_USEREVENT:
    {
      if (ev.user.code == UserEvents::mouse_react)
      {
        SDL_Event *te = (SDL_Event *)ev.user.data1; // the mouse coordinates at time of double click
        
        if (te->motion.x >= PC_X && te->motion.x < (PC_X + (strlen(PC_STR)*TILE_WIDTH)) &&
         te->motion.y >= PC_Y && te->motion.y < (Sint32)(PC_Y+TILE_HEIGHT) )
        {
          player->pause(1, false);
          mouseover_hexdump_area.submode = Mouse_Hexdump_Area::EASY_EDIT;
          main_memory_area.address = report::last_pc; mode = MODE_EDIT_MOUSE_HEXDUMP;
          lock();
        }

        else if (te->motion.x >= (MOUSE_HEXDUMP_START_X - 2) && te->motion.x <= (MOUSE_HEXDUMP_END_X + 2) &&
          te->motion.y >= Mouse_Hexdump_Area::MOUSE_HEXDUMP_START_Y && te->motion.y <= MOUSE_HEXDUMP_END_Y)
        {
          // editor stuffz
          Uint8 oldmode = mode;
          mode = MODE_EDIT_MOUSE_HEXDUMP;
          

          int res_x, res_y, highnibble;

          const int entry_width = MOUSE_HEXDUMP_ENTRY_X_INCREMENT;
          const int entry_height = MOUSE_HEXDUMP_ENTRY_Y_INCREMENT;

          mouseover_hexdump_area.rel_x = te->motion.x - MOUSE_HEXDUMP_START_X;
          mouseover_hexdump_area.rel_x+=2;
          mouseover_hexdump_area.rel_y = te->motion.y - Mouse_Hexdump_Area::MOUSE_HEXDUMP_START_Y;

          res_x = mouseover_hexdump_area.rel_x / entry_width;
          res_y = mouseover_hexdump_area.rel_y / entry_height;

          int res_half = mouseover_hexdump_area.rel_x % entry_width;
          int tmp = entry_width / 2;

          if (res_half < tmp) highnibble = 1;
          else highnibble = 0;

          if (oldmode == MODE_EDIT_MOUSE_HEXDUMP)
          {
            if (res_x == mouseover_hexdump_area.res_x && res_y == mouseover_hexdump_area.res_y && highnibble == mouseover_hexdump_area.highnibble)
            {
              mode = MODE_NAV;
              mouseover_hexdump_area.cursor.stop_timer();
              unlock();
              break;
            }
          }

          // order matters .. call here: 
          lock(1,0,0,res_x,res_y);
          mouseover_hexdump_area.highnibble = highnibble;

          
          if (mouseover_hexdump_area.res_y == 16) mouseover_hexdump_area.res_y = 15;
        }

        /* porttool */
        else if ( (te->button.x >= PORTTOOL_X + (8*5)) &&
            te->button.y >= PORTTOOL_Y && te->button.y < (PORTTOOL_Y + 16))
        {
          int x, y;
          x = te->button.x - (PORTTOOL_X + (CHAR_WIDTH*5));
          x /= 8;

          // prevent double click on +/- during port edit mode to cause cursor to show up
          // on +/-
          switch (x)
          {
            case 2:
            case 3:
            case 7:
            case 8:
            case 12:
            case 13:
            case 17:
            case 18:
              port_tool.last_x = port_tool.x;
              port_tool.x = ((PORTTOOL_X + (CHAR_WIDTH*5)) + (x*CHAR_WIDTH));
          }
          
          y = te->button.y - PORTTOOL_Y;
          y /= 8;

          if (te->button.button == SDL_BUTTON_LEFT)
          {
            
            if (mode == MODE_EDIT_APU_PORT && port_tool.last_x == port_tool.x)
            {
              mouseover_hexdump_area.cursor.stop_timer();
              exit_edit_mode();
              break;
            }
            switch (x)
            {
              // i think single click takes care of this
              case 2:
              {
                port_tool.set_port(0);

                port_tool.highnibble = 1;
                mode = MODE_EDIT_APU_PORT;
                mouseover_hexdump_area.cursor.start_timer();
              } break;
              case 3:
              {
                port_tool.set_port(0);
                port_tool.highnibble = 0;
                mode = MODE_EDIT_APU_PORT;
                mouseover_hexdump_area.cursor.start_timer();
              } break;
              case 7:
              {
                port_tool.set_port(1);
                port_tool.highnibble = 1;
                mode = MODE_EDIT_APU_PORT;
                mouseover_hexdump_area.cursor.start_timer();
              } break;
              case 8:
              {
                port_tool.set_port(1);
                port_tool.highnibble = 0;
                mode = MODE_EDIT_APU_PORT;
                mouseover_hexdump_area.cursor.start_timer();
              } break;
              case 12:
              {
                port_tool.set_port(2);
                port_tool.highnibble = 1;
                mode = MODE_EDIT_APU_PORT;
                mouseover_hexdump_area.cursor.start_timer();
              } break;
              case 13:
              {
                port_tool.set_port(2);
                port_tool.highnibble = 0;
                mode = MODE_EDIT_APU_PORT;
                mouseover_hexdump_area.cursor.start_timer();
              } break;
              case 17:
              {
                port_tool.set_port(3);
                port_tool.highnibble = 1;
                mode = MODE_EDIT_APU_PORT;
                mouseover_hexdump_area.cursor.start_timer();
              } break;
              case 18:
              {
                port_tool.set_port(3);
                port_tool.highnibble = 0;
                mode = MODE_EDIT_APU_PORT;
                mouseover_hexdump_area.cursor.start_timer();
              } break;
            }
          }
        }
      }
    } break;
    case SDL_MOUSEWHEEL:
    {
      // GLOBAL SHIT
      if (  (mouse::x >= PORTTOOL_X + (8*5)) &&
            mouse::y >= PORTTOOL_Y && mouse::y < (PORTTOOL_Y + 16) )
        {
          int x, y;
          x = mouse::x - (PORTTOOL_X + (8*5));
          x /= 8;
          y = mouse::y - PORTTOOL_Y;
          y /= 8;
          if (ev.wheel.y > 0)
          {
            if (x>1 && x<4) {   port_tool.inc_port(0); }
            if (x>6 && x<9) {   port_tool.inc_port(1); }
            if (x>11 && x<14) { port_tool.inc_port(2); }
            if (x>16 && x<19) { port_tool.inc_port(3); }
          }
          else if (ev.wheel.y < 0)
          {
            if (x>1 && x<4) {   port_tool.dec_port(0); }
            if (x>6 && x<9) {   port_tool.dec_port(1); }
            if (x>11 && x<14) { port_tool.dec_port(2); }
            if (x>16 && x<19) { port_tool.dec_port(3); }
          }
        }
        else
        {
          if (ev.wheel.y > 0)
          {
            mouseover_hexdump_area.add_addr(-0x08);
            break;          
          }
          else
          {
            mouseover_hexdump_area.add_addr(0x08);
            break;
          }
        }
    } break;
    case SDL_MOUSEBUTTONDOWN:           
      {
        voice_control.checkmouse((Uint16&)ev.motion.x, (Uint16&)ev.motion.y, ev.button.button); 

        bool is_in_memory_window= (ev.motion.x >= MEMORY_VIEW_X && 
              ev.motion.x < MEMORY_VIEW_X + 512 &&
              ev.motion.y >= MEMORY_VIEW_Y &&
              ev.motion.y < MEMORY_VIEW_Y + 512) ? true:false;

        if ( is_in_memory_window )
        {
          if (ev.button.button == SDL_BUTTON_RIGHT)
          {
            main_memory_area.spawnbrrcontextmenu(ev.button.x, ev.button.y);
          }
        } 

        if (  ev.motion.x >= Screen::locked.x && 
              ev.motion.x < Screen::locked.x + Screen::locked.w &&
              ev.motion.y >= Screen::locked.y &&
              ev.motion.y < Screen::locked.y + 9 )
        {
          if(locked())
            unlock();
        }
        else if (  ev.motion.x >= Screen::echoE.x && 
              ev.motion.x < Screen::echoE.x + Screen::echoE.w &&
              ev.motion.y >= Screen::echoE.y &&
              ev.motion.y < (Screen::echoE.y + Screen::echoE.h) )
        {
          // toggle_echo()
          //DEBUGLOG("HIE SXY");
          player->spc_emu()->toggle_echo();
        }

        else if (mode == MODE_NAV)
        {
          
          if ( ev.motion.x >= echo_on_x && 
              ev.motion.x < echo_on_x+(3*CHAR_WIDTH) &&
              ev.motion.y >= echo_on_y &&
              ev.motion.y < echo_on_y+CHAR_HEIGHT )
          {
            if (ev.button.button == SDL_BUTTON_LEFT)
              player->spc_emu()->toggle_echo();
          }
          // click in memory view. Toggle lock
          else if ( is_in_memory_window )
          {
            // ORDER IMPORTANT
            if (ev.button.button == SDL_BUTTON_LEFT)
            {
              toggle_lock(ev.motion.x, ev.motion.y);
            }
          }
        }

        else if (mode == MODE_EDIT_MOUSE_HEXDUMP)
        {
          if (is_in_memory_window && ev.button.button == SDL_BUTTON_LEFT)
          {
            exit_edit_mode();
            main_memory_area.set_addr_from_cursor(ev.motion.x, ev.motion.y);
          }
        }
        
        if (ev.motion.x >= (MOUSE_HEXDUMP_START_X - 2) && ev.motion.x <= (MOUSE_HEXDUMP_END_X + 2) &&
          ev.motion.y >= Mouse_Hexdump_Area::MOUSE_HEXDUMP_START_Y && ev.motion.y <= MOUSE_HEXDUMP_END_Y)
        {
          if (ev.button.button == SDL_BUTTON_RIGHT)
          {
            toggle_lock();
          }
        }

        /* porttool */
        else if (  (ev.button.x >= PORTTOOL_X + (8*5)) &&
            ev.button.y >= PORTTOOL_Y && ev.button.y < (PORTTOOL_Y + 16) )
        {
          int x, y;
          x = ev.button.x - (PORTTOOL_X + (8*5));
          x /= 8;
          y = ev.button.y - PORTTOOL_Y;
          y /= 8;
        
          if (ev.button.button == SDL_BUTTON_LEFT)
          {
            switch (x)
            {
              case 1: port_tool.dec_port (0); break;
              case 4: port_tool.inc_port (0); break;
              case 6: port_tool.dec_port (1); break;
              case 9: port_tool.inc_port (1); break;
              case 11: port_tool.dec_port(2); break;
              case 14: port_tool.inc_port(2); break;
              case 16: port_tool.dec_port(3); break;
              case 19: port_tool.inc_port(3); break;
            }
          }
          
        } 
      }
      break;
      default:
      break;
  }
  return 0;
}

void Main_Window::run()
{
  if ( player->has_no_song )
    return;

  if (BaseD::Profile::is_profiling)
  {
    tmp_profile->process();
  }

  pack_mask(packed_mask);
  
  if (g_cfg.statusline) {
    printf("%s / %d         \r", 
        g_cfg.playlist[g_cur_entry],
        song_time);
    fflush(stdout);
  }
  
  BaseD::check_time();
}

void Main_Window::lock(char l/*=1*/, int x/*=0*/, int y/*=0*/, uint8_t rx/*=0*/, uint8_t ry/*=0*/)
{
  main_memory_area.lock(l,x,y,rx,ry);
  if (locked())
  {
    mode = MODE_EDIT_MOUSE_HEXDUMP;
  }
  else
  {
    mode = MODE_NAV;
  }
  
}
void Main_Window::toggle_lock(int x/*=0*/, int y/*=0*/)
{
  lock(!locked(), x, y);
}

void Main_Window::unlock()
{
  lock(0);
}



Main_Window::Main_Window(int &argc, char **argv) : 
main_memory_area(&mouseover_hexdump_area, &dir),
port_tool(&mouseover_hexdump_area.cursor)
{
  int res;
  static struct option long_options[] = {
    {"nosound", 0, 0, 0},
    {"novideo", 0, 0, 1},
    {"update_in_callback", 0, 0, 2},
    {"echo", 0, 0, 3},
    {"interpolation", 0, 0, 4},
    {"savemask", 0, 0, 5},
    {"default_time", 1, 0, 6},
    {"ignore_tag_time", 0, 0, 7},
    {"extra_time", 1, 0, 8},
    {"yield", 0, 0, 9},
    {"auto_write_mask", 0, 0, 10},
    {"status_line", 0, 0, 11},
    {"help", 0, 0, 'h'},
    {"apply_mask_block", 0, 0, 12},
    {"apply_mask_byte", 0, 0, 13},
    {"filler", 1, 0, 14},
    {0,0,0,0}
  };

  if (::render->screen == NULL)
  {
    fprintf(stderr, "Debugger::MainWindows::::render->screen is Null\n"); //::render->screen, Render_Context::::render->screen);
    exit(1);
  }

  while ((res=getopt_long(argc, argv, "h",
        long_options, NULL))!=-1)
  {
    switch(res)
    {
      case 0:
        g_cfg.nosound = 1;
        break;
      case 1:
        g_cfg.novideo = 2;
        break;
      case 2:
        g_cfg.update_in_callback = 1;
        break;
      case 4:
        //spc_config.is_interpolation = 1;
        break;
      case 3:
        //spc_config.is_echo = 1;
        break;
      case 5:
        g_cfg.autowritemask = 1;
        break;
      case 6:
        g_cfg.defaultsongtime = atoi(optarg);
        break;
      case 7:
        g_cfg.ignoretagtime = 1;
        break;
      case 8:
        g_cfg.extratime = atoi(optarg);
        break;
      case 9:
        g_cfg.nice = 1;
        break;
      case 10:
        g_cfg.autowritemask = 1;
        break;
      case 11:
        g_cfg.statusline = 1;
        break;
      case 12:
        g_cfg.apply_block = 1;
        break;
      case 14:
        g_cfg.filler = strtol(optarg, NULL, 0);
        break;
      case 'h':
        printf("Usage: ./vspcplay [options] files...\n");
        printf("\n");
        printf("Valid options:\n");
        printf(" -h, --help     Print help\n");
        printf(" --nosound      Dont output sound\n");
        printf(" --novideo      Dont open video window\n");
        printf(" --update_in_callback   Update spc sound buffer inside\n");
        printf("                        sdl audio callback\n");
        printf(" --interpolation  Use sound interpolatoin\n");
        printf(" --echo           Enable echo\n");
        printf(" --auto_write_mask   Write mask file automatically when a\n");
        printf("                     tune ends due to playtime from tag or\n");
        printf("                     default play time.\n");
        printf(" --default_time t    Set the default play time in seconds\n");
        printf("                     for when there is not id666 tag. (default: %d\n", DEFAULT_SONGTIME);
        printf(" --ignore_tag_time   Ignore the time from the id666 tag and\n");
        printf("                     use default time\n");
        printf(" --extra_time t      Set the number of extra seconds to play (relative to\n");
        printf("                     the tag time or default time).\n");
        printf(" --nice              Try to use less cpu for graphics\n");
        printf(" --status_line       Enable a text mode status line\n");
        printf("\n!!! Careful with those!, they can ruin your sets so backup first!!!\n");
        printf(" --apply_mask_block  Apply the mask to the file (replace unreport::used blocks(256 bytes) with a pattern)\n");
        printf(" --filler val        Set the pattern byte value. Use with the option above. Default 0\n");
        printf("\n");
        printf("The mask will be applied when the tune ends due to playtime from tag\n");
        printf("or default playtime.\n");
        exit(0);
        break;
    }
  }

  g_cfg.num_files = argc-optind;
  g_cfg.playlist = &argv[optind];

  time_cur = time_last = SDL_GetTicks();
  report::memsurface.init();
}







void Main_Window::exit_edit_mode()
{
  mode = MODE_NAV;
  mouseover_hexdump_area.submode = 0;
  mouseover_hexdump_area.draw_tmp_ram = 0;

  unlock();
}

void Main_Window::draw_block_usage_bar()
{
  static bool is_first_run=false;
  // draw the 256 bytes block usage bar
  tmprect.x = MEMORY_VIEW_X-1;
  tmprect.w = 1; tmprect.h = 2;
  int tmp=0;
  for (int i=0; i<256; i++)
  {
    tmprect.y = MEMORY_VIEW_Y + i * 2;
    if (report::used2[i])
    {
      SDL_FillRect(::render->screen, &tmprect, Colors::white); 
      tmp++;
    }
  }
  
  sprintf(tmpbuf, "Blocks report::used: %3d/256 (%.1f%%)  ", tmp, (float)tmp*100.0/256.0);
  sdlfont_drawString(::render->screen, MEMORY_VIEW_X, MEMORY_VIEW_Y + report::memsurface.sdl_surface->h + 2, tmpbuf);


  int x = MEMORY_VIEW_X;
  int y = MEMORY_VIEW_Y + report::memsurface.sdl_surface->h + 2 + 9;
  if (1)
  {
    sprintf(tmpbuf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        packed_mask[0], packed_mask[1], packed_mask[2], packed_mask[3],
        packed_mask[4], packed_mask[5], packed_mask[6], packed_mask[7],
        packed_mask[8], packed_mask[9], packed_mask[10], packed_mask[11],
        packed_mask[12], packed_mask[13], packed_mask[14], packed_mask[15],
        packed_mask[16], packed_mask[17], packed_mask[18], packed_mask[19],
        packed_mask[20], packed_mask[21], packed_mask[22], packed_mask[23],
        packed_mask[24], packed_mask[25], packed_mask[26], packed_mask[27],
        packed_mask[28], packed_mask[29], packed_mask[30], packed_mask[31]);

    sdlfont_drawString(::render->screen, x, y, tmpbuf);
  }
  y+= CHAR_HEIGHT*2;

  if (is_first_run)
  {
  }
}

void Main_Window::draw_mouse_address()
{
  // write the address under mouse cursor
  char addr_mouse_str[] = "Addr mouse: $%04X";
  int x = MEMORY_VIEW_X+8*(23);
  int y = MEMORY_VIEW_Y-10;
  if (mouseover_hexdump_area.address >=0)
  {
    sprintf(tmpbuf, addr_mouse_str, main_memory_area.mouse_addr);
    sdlfont_drawString(::render->screen, x, y, tmpbuf);
  }

  // leeching coordinates ;P 
  int length = strlen(tmpbuf); length+=3;
  x += (length*CHAR_WIDTH);
  sprintf(tmpbuf, "Gain:");
  sdlfont_drawString(::render->screen, x, y, tmpbuf);

  length = strlen(tmpbuf); length+=0;
  x += length*CHAR_WIDTH + 4;
  // draw Slider here..
  
  if (is_first_run)
  {
    int slider_width = 100;
    // here we will allocate slider at these coordinates
    DEBUGLOG("new slider");
    gain.slider = new Slider<double>(&player->new_gain_db,x, y+1, slider_width, 4, 6,6, 
      player->min_gain_db, player->max_gain_db, 0.0, 3, Main_Window::Gain::change, "db", true);
  }

  x+=gain.slider->panel_rect.w + CHAR_WIDTH*2;
  sprintf(tmpbuf, "Tempo:");
  sdlfont_drawString(::render->screen, x, y, tmpbuf);
  if (is_first_run)
  {
    int slider_width = 60;

    x += strlen(tmpbuf)*CHAR_WIDTH + 4;
    tempo.slider = new Slider<double>(&player->tempo,x, y+1, slider_width, 4, 6,6, 0.02, 4.0, 1.0, 1, Main_Window::Tempo::change);
  }
}

void Main_Window::reload()
{
  BaseD::reload();
  draw_track_tag();
}

void Main_Window::one_time_draw()
{
  // draw one-time stuff
    SDL_FillRect(::render->screen, NULL, Colors::Interface::color[Colors::Interface::Type::bg]);
    
    if (player->emu())
      draw_track_tag();

    sdlfont_drawString2(::render->screen, PORTTOOL_X, PORTTOOL_Y, "     - Port tool -");
}


void Main_Window::fade_arrays()
{
  report::memsurface.fade_arrays();
}

void Main_Window::do_scroller(int elaps_milli)
{
  int i;
  char c[2] = { 0, 0 }; 
  static int cs;
  static int p = 0;
  static int cur_len;
  static int cur_min;
  SDL_Rect tmprect;
  static float start_angle = 0.0;
  float angle;
  int off;
  int steps;
  static int keep=0;

  if (!player->emu())
    return;

  keep += elaps_milli;  

  steps = keep*60/1000;
  if (!steps) { return; }

  elaps_milli = keep;
  keep=0;

  if (cur_marquee == NULL) { 
    cur_marquee = marquees[cur_marquee_id]; 
    p = ::render->screen->w;
  }
  
  cur_len = strlen(cur_marquee);
  cur_min = -cur_len*8;

  angle = start_angle;

  cs = player->emu()->tell() / 1000;
  cs %= 12;

  // clear area 
  tmprect.x = 0;
  tmprect.y = 0;
  tmprect.w = ::render->screen->w;
  tmprect.h = 28;
  SDL_FillRect(::render->screen, &tmprect, Colors::Interface::color[Colors::Interface::Type::bg]);
  
  
  for (i=0; i<cur_len; i++)
  {
    off = sin(angle)*8.0;
    c[0] = cur_marquee[i];
    if (  (tmprect.x + i*8 + p > 0) && (tmprect.x + i*8 + p < ::render->screen->w) )
    {
      sdlfont_drawString(::render->screen, tmprect.x + i*8 + p, 12 + off, c, Colors::colorscale[cs]);
    }
    angle-=0.1;
  }
  start_angle += steps * 0.02;
  p-=steps;

  if (p<cur_min) {
    if (marquees[cur_marquee_id+1]!=NULL) {
      cur_marquee = marquees[++cur_marquee_id];
      p = ::render->screen->w;
    }
    else {
      p = ::render->screen->w;
    }
  }
}



void Main_Window::draw_program_counter()
{
  // write the program counter
  if (player->has_no_song) report::last_pc = 0;
  else report::last_pc = (int)player->spc_emu()->pc(); 
  sprintf(tmpbuf, PC_STR, report::last_pc);
  sdlfont_drawString2(::render->screen, PC_X, PC_Y, tmpbuf); // Colors::white);
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


    if (voice_control.is_muted(i))
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
    if (voice_control.is_muted(i))
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

    if (voice_control.is_muted(i))
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
    if (voice_control.is_muted(i))
    {
      Colors::subtractp(Color1,0x60);
      Colors::subtractp(Color2,0x60);
    }
    sdlfont_drawString2c(::render->screen, MEMORY_VIEW_X+520 + 8, tmp + (i*10), tmpbuf, *Color1, *Color2);
    

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10);
    tmprect.w = left_vol*(::render->screen->w-tmprect.x-20)/255;

    // L volume
    if (voice_control.is_muted(i))
      color = &Colors::dark_yellow;
    else color = &Colors::yellow;
    SDL_FillRect(::render->screen, &tmprect, *color);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(::render->screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+3;
    
    if (voice_control.is_muted(i))
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
    if (voice_control.is_muted(i))
    {
      Uint8 r1,g1,b1;
      SDL_GetRGB(c3, ::render->screen->format, &r1, &b1, &g1);
      c3 = SDL_MapRGB(::render->screen->format,r1-0x60,g1-0x60,b1-0x60);
    }

    SDL_FillRect(::render->screen, &tmprect, c3);
  }
}

void Main_Window::draw_global_volumes()
{
  draw_main_volume();
  draw_echo_volume();
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

void Main_Window::draw_mouseover_hexdump()
{
  i++;

  tmp += i*10 + 8;
  Screen::locked.y = tmp;
  sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520, tmp, "  - Mouseover Hexdump -");
  if (locked()) {
    
    sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520+24*8, tmp, LOCKED_STR, Colors::Interface::color[Colors::Interface::Type::lock]);
  } else {
    sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520+24*8, tmp, "      ", Colors::Interface::color[Colors::Interface::Type::lock]);
  }
  
  tmp+=9;
  Mouse_Hexdump_Area::MOUSE_HEXDUMP_START_Y = tmp;
  if (mouseover_hexdump_area.address>=0)
  {
    
    for (i=0; i<128; i+=8)
    {
      unsigned char *st;
      Uint16 cut_addr = (mouseover_hexdump_area.address + i) & 0xffff;
      
      st = &IAPURAM[cut_addr];
      
      int p = MEMORY_VIEW_X+520;
      
      sprintf(tmpbuf, "%04X: ", cut_addr);
      sdlfont_drawString(::render->screen, p, tmp, tmpbuf);
      p += 6*8;

      for (int j=0; j<8; j++) {

        int idx = ((cut_addr+j)&0xff00)<<4; 
        Uint32 color;

        idx += ((cut_addr+j) % 256)<<3;
        color = SDL_MapRGB(::render->screen->format, 
            0x7f + (report::memsurface.data[idx]>>1), 
            0x7f + (report::memsurface.data[idx+1]>>1), 
            0x7f + (report::memsurface.data[idx+2]>>1)
            );
            
        Uint16 cur_addr = cut_addr+j;
        if (cur_addr != mouseover_hexdump_area.addr_being_edited)
        {
            if (cur_addr == 0xf3 )
            {
              sprintf(tmpbuf, "%02X ", player->spc_read_dsp(IAPURAM[0xf2]));
            }
            else if (cur_addr == 0xf0 )
            {
              sprintf(tmpbuf, "%02X ", player->spc_read(0xf0));
            }
            else sprintf(tmpbuf, "%02X ", *st);
        }
        else  /* Some registers like being read directly form RAM.. others like going thru the API */
        {
          if (cur_addr == 0xf3 )
          {
            if (mouseover_hexdump_area.draw_tmp_ram)
              sprintf(tmpbuf, "%02X ", mouseover_hexdump_area.tmp_ram);
            else sprintf(tmpbuf, "%02X ", player->spc_read_dsp(IAPURAM[0xf2]));
          }
          else if (cur_addr == 0xf0 )
          {
            if (mouseover_hexdump_area.draw_tmp_ram)
              sprintf(tmpbuf, "%02X ", mouseover_hexdump_area.tmp_ram);
            else sprintf(tmpbuf, "%02X ", player->spc_read(cur_addr));
          }
          else if (cur_addr == 0xf1 || (cur_addr >= 0xf4 && cur_addr <= 0xf7) || 
            (cur_addr >= 0xfa && cur_addr <= 0xfc))
          {
            if (mouseover_hexdump_area.draw_tmp_ram)
              sprintf(tmpbuf, "%02X ", mouseover_hexdump_area.tmp_ram);
            else sprintf(tmpbuf, "%02X ", *st);
          }
          else sprintf(tmpbuf, "%02X ", *st);
        }


        st++;

        sdlfont_drawString(::render->screen, p, tmp, tmpbuf, color);
        p+= 2*8 + 4;
      }
      
      tmp += 9;
    }

    
  }
}

void Main_Window::draw_porttool()
{
  sdlfont_drawString(::render->screen, PORTTOOL_X, PORTTOOL_Y+8, " APU:");
  sdlfont_drawString(::render->screen, PORTTOOL_X, PORTTOOL_Y+16, "SNES:");

  if (mode == MODE_EDIT_APU_PORT)
    sprintf(tmpbuf, " -%02X+ -%02X+ -%02X+ -%02X+", port_tool.tmp[0], port_tool.tmp[1], port_tool.tmp[2], port_tool.tmp[3]);    
  else 
    sprintf(tmpbuf, " -%02X+ -%02X+ -%02X+ -%02X+", port_tool.portdata[0], port_tool.portdata[1], port_tool.portdata[2], port_tool.portdata[3]);  
  
  sdlfont_drawString(::render->screen, PORTTOOL_X + (8*5), PORTTOOL_Y+8, tmpbuf);
  
  sprintf(tmpbuf, "  %02X   %02X   %02X   %02X", IAPURAM[0xf4], IAPURAM[0xf5], IAPURAM[0xf6], IAPURAM[0xf7]);   
  sdlfont_drawString(::render->screen, PORTTOOL_X + (8*5), PORTTOOL_Y+16, tmpbuf);
}

void Main_Window::draw_time_and_echo_status(int *x/*=0*/, int *y/*=0*/)
{
  static bool is_first_run=true;
  static int xx=0, yy=0;
  if (x == NULL && y == NULL)
  {
    if (xx == 0 && yy == 0) 
      return;
  }
  else
  {
    xx = *x; yy = *y;
  }

  int yyy = yy;
  sprintf(tmpbuf, "Time....: %0d:%02d / %0d:%02d", 
      (int(player->emu()->tell()/1000)/60),
      (int((player->emu()->tell()/1000))%60),
      song_time/60, song_time%60);
  sdlfont_drawString(::render->screen, xx, yyy, tmpbuf); yyy+=CHAR_HEIGHT;


  sprintf(tmpbuf, "Echo....: %s", player->spc_emu()->is_echoing() ? "On " : "Off"); 
  sdlfont_drawString(::render->screen, xx, yyy, tmpbuf); 
  if (is_first_run)
  {
    echo_on_x = xx + (strlen(tmpbuf)-3)*CHAR_WIDTH;
    echo_on_y = yyy;
    DEBUGLOG("echo_x = %d, echo_y = %d\n", echo_on_x , echo_on_y);
    is_first_run=false;
  }

  yyy+=CHAR_HEIGHT;

  if (y) *y = yyy;
}





// update window title with track info
void Main_Window::update_window_title()
{

  long seconds = player->track_info().length / 1000;
  const char* game = player->track_info().game;
  if ( !*game )
  {
    // extract filename
    game = strrchr( path, '\\' ); // DOS
    if ( !game )
      game = strrchr( path, '/' ); // UNIX
    if ( !game )
      game = path;
    else
      game++; // skip path separator
  }
  
  char title [512];
  sprintf( title, "%s: %d/%d %s (%ld:%02ld)",
      game, g_cur_entry+1, g_cfg.num_files, player->track_info().song,
      seconds / 60, seconds % 60 );
  SDL_SetWindowTitle(::render->sdlWindow, title);
}



void Main_Window::scroll_track_tags()
{
  static Uint32 recorded_sec_elapsed=0;
  Uint32 cur_sec_elapsed = time_cur / 195;
  bool can_scrollnow = (cur_sec_elapsed > recorded_sec_elapsed);
  recorded_sec_elapsed = cur_sec_elapsed;

  if (!can_scrollnow)
    return;

  scroll_tags.scroll_draw();
}
void Main_Window::draw_track_tag()
{
  static SDL_Rect r;

  
  if (!is_first_run)
    SDL_FillRect(::render->screen, &r, Colors::Interface::color[Colors::Interface::Type::bg]);

  //fprintf(stderr, "comment = %s\n", tag.comment);
  //fprintf(stderr, "path = %s\nsong = %s\ngame = %s\ndumper = %s\ncomment = %s")
  int y = INFO_Y + 8;
  int x = INFO_X;
  if (is_first_run)
  {
    r.x = x;
    r.y = y;
    r.w = 800-x;
  }
  
  DEBUGLOG("y == %d, ", y);
  draw_time_and_echo_status(&x, &y);
  DEBUGLOG("y == %d\n", y);
  
  y += CHAR_HEIGHT;
  
  //sprintf(tmpbuf, "Filename: %s", path);
  //sdlfont_drawString(::render->screen, INFO_X, y, tmpbuf); y+=CHAR_HEIGHT*2;
  
  Scroll_Tag::compute(tag.game, &scroll_tags.game, 1000);
  Scroll_Tag::compute(tag.song, &scroll_tags.song, 1200);
  Scroll_Tag::compute(tag.author, &scroll_tags.author, 1400);
  Scroll_Tag::compute(tag.dumper, &scroll_tags.dumper, 1600);
  Scroll_Tag::compute(tag.comment, &scroll_tags.comment, 1800);

  sprintf(tmpbuf, "Game....: %s", tag.game);
  sdlfont_drawString(::render->screen, INFO_X, y, tmpbuf); 
  if (is_first_run)
  {
    scroll_tags.game.rect.x = INFO_X + strlen("Game....: ")*CHAR_WIDTH;
    scroll_tags.game.rect.y = y;
  }
  y+=CHAR_HEIGHT;
  sprintf(tmpbuf, "Title...: %s", tag.song);
  sdlfont_drawString(::render->screen, INFO_X, y, tmpbuf); 
  if (is_first_run)
  {
    scroll_tags.song.rect.x = INFO_X + strlen("Game....: ")*CHAR_WIDTH;
    scroll_tags.song.rect.y = y;
  }
  y+=CHAR_HEIGHT;
  sprintf(tmpbuf, "Composer: %s", tag.author);
  sdlfont_drawString(::render->screen, INFO_X, y, tmpbuf); 
  if (is_first_run)
  {
    scroll_tags.author.rect.x = INFO_X + strlen("Game....: ")*CHAR_WIDTH;
    scroll_tags.author.rect.y = y;
  }
  y+=CHAR_HEIGHT*2;

  sprintf(tmpbuf, "Dumper..: %s", tag.dumper);
  sdlfont_drawString(::render->screen, INFO_X, y, tmpbuf); 
  if (is_first_run)
  {
    scroll_tags.dumper.rect.x = INFO_X + strlen("Game....: ")*CHAR_WIDTH;
    scroll_tags.dumper.rect.y = y;
  }
  y+=CHAR_HEIGHT;
  sprintf(tmpbuf, "Comment.: %s", tag.comment);
  sdlfont_drawString(::render->screen, INFO_X, y, tmpbuf); 
  if (is_first_run)
  {
    scroll_tags.comment.rect.x = INFO_X + strlen("Game....: ")*CHAR_WIDTH;
    scroll_tags.comment.rect.y = y;
  }
  y+=CHAR_HEIGHT*2;

  sprintf(tmpbuf, "Ignore tag time: %s", g_cfg.ignoretagtime ? "Yes" : "No");
  sdlfont_drawString2(::render->screen, INFO_X, y, tmpbuf);

  sprintf(tmpbuf, "Default time...: %d:%02d", g_cfg.defaultsongtime/60, g_cfg.defaultsongtime%60);
  sdlfont_drawString2(::render->screen, INFO_X, y, tmpbuf);
  
  if (is_first_run)
  {
    y+=CHAR_HEIGHT;
    r.h = y - r.y;
  }
}

void Main_Window::maybe_write_to_mem(bool force/*=false*/)
{
  Uint16 addr= mouseover_hexdump_area.addr_being_edited;
  if ( IS_SPECIAL_ADDRESSES(addr) )
  {
    if (!mouseover_hexdump_area.highnibble || force)
    {
      if (addr >= 0xf4 && addr <= 0xf7)
      {
        port_tool.write(addr-0xf4, mouseover_hexdump_area.tmp_ram);
        //player->spc_write_port()
      }
      else 
      {
        player->spc_write(addr, mouseover_hexdump_area.tmp_ram);
        fprintf(stderr, "WRite");
      }
      mouseover_hexdump_area.draw_tmp_ram = 0;
    }
    else mouseover_hexdump_area.draw_tmp_ram = 1;
  }
  else player->spc_write(addr, mouseover_hexdump_area.tmp_ram);
}




