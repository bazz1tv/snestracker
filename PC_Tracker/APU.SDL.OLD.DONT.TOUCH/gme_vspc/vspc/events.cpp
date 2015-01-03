
#include "globals.h"

#include "report.h"
#include "track.h"
#include "voices.h"
#include <math.h>
#include "sdl_dblclick.h"
#include "gui/porttool.h"
#include "mode.h"
#include "mouse_hexdump.h"
#include "colors.h"
#include "platform.h"

#define L_FLAG 0
#define R_FLAG 1

int tmp=0, i=0;
SDL_Rect tmprect;
unsigned char packed_mask[32];
uint16_t mouse_addr=0; 

void draw_program_counter();
void draw_voices_pitchs();
void draw_voices_volumes();
void draw_global_volumes();
void draw_main_volume();
void draw_echo_volume();
void draw_mouseover_hexdump();
void draw_porttool();
void draw_time_and_echo_status();



void toggle_pause();
void restart_track();
void prev_track();
void next_track();
void exit_edit_mode();
void inc_ram(int addr, int i);
void dec_ram(int addr, int i);
void reload();

void toggle_pause()
{
  player->toggle_pause();
}

void restart_track()
{
  SDL_PauseAudio(1);
  g_cur_entry=0;
  player->pause(0);
  reload();
}

void prev_track()
{
  SDL_PauseAudio(true);
  g_cur_entry--;
  if (g_cur_entry<0) { g_cur_entry = g_cfg_num_files-1; }
  reload();
}

void next_track()
{
  SDL_PauseAudio(true);
  g_cur_entry++;
  if (g_cur_entry>=g_cfg_num_files) { g_cur_entry = 0; }
  reload();
}

void exit_edit_mode()
{
  mode = MODE_NAV;
  submode = 0;
  mouse_hexdump::draw_tmp_ram = 0;

  mouse_hexdump::unlock();
}

void draw_block_usage_bar()
{
  // draw the 256 bytes block usage bar
  tmprect.x = MEMORY_VIEW_X-1;
  tmprect.w = 1; tmprect.h = 2;
  int tmp=0;
  for (int i=0; i<256; i++)
  {
    tmprect.y = MEMORY_VIEW_Y + i * 2;
    if (used2[i])
    {
      SDL_FillRect(screen, &tmprect, colors::white); 
      tmp++;
    }
  }
  
  sprintf(tmpbuf, "Blocks used: %3d/256 (%.1f%%)  ", tmp, (float)tmp*100.0/256.0);
  sdlfont_drawString(screen, MEMORY_VIEW_X, MEMORY_VIEW_Y + memsurface.sdl_surface->h + 2, tmpbuf, colors::white);

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

    sdlfont_drawString(screen, MEMORY_VIEW_X, MEMORY_VIEW_Y + memsurface.sdl_surface->h + 2 + 9, tmpbuf, colors::white);
  }
}

void draw_mouse_address()
{
  // write the address under mouse cursor
  if (mouse_hexdump::address >=0)
  {
    sprintf(tmpbuf, "Addr mouse: $%04X", mouse_addr);
    sdlfont_drawString(screen, MEMORY_VIEW_X+8*(23), MEMORY_VIEW_Y-10, tmpbuf, colors::white);
  }
}

void reload()
{
#ifdef WIN32
  g_real_filename = strrchr(g_cfg_playlist[g_cur_entry], '\\');
#else
  g_real_filename = strrchr(g_cfg_playlist[g_cur_entry], '/');
#endif
  if (!g_real_filename) {
    g_real_filename = g_cfg_playlist[g_cur_entry];
  }
  else {
    // skip path sep
    g_real_filename++;
  } 
    
  // Load file
  path = g_cfg_playlist[g_cur_entry];
  handle_error( player->load_file( g_cfg_playlist[g_cur_entry] ) );
  
  IAPURAM = player->spc_emu()->ram();
  
  // memsurface.init
  memsurface.clear();

  memset(used, 0, sizeof(used));
  memset(used2, 0, sizeof(used2));
  //if (!mouse_hexdump::address)mouse_hexdump::address =0;
  last_pc = -1;
  
  start_track( 1, path );
  voices::was_keyed_on = 0;
  player->mute_voices(voices::muted);
  player->ignore_silence();
  

  // draw one-time stuff
  if (!g_cfg_novideo)
  {
    SDL_FillRect(screen, NULL, 0);
    memsurface.init_video();
    
    tmprect.x = MEMORY_VIEW_X-1;
    tmprect.y = MEMORY_VIEW_Y-1;
    tmprect.w = 512+2;
    tmprect.h = 512+2;
    SDL_FillRect(screen, &tmprect, colors::white); 
    
    sdlfont_drawString(screen, MEMORY_VIEW_X, MEMORY_VIEW_Y-10, "spc memory:", colors::white);

    sprintf(tmpbuf, " QUIT - PAUSE - RESTART - PREV - NEXT - WRITE MASK - DSP MAP");
    sdlfont_drawString(screen, 0, screen->h-9, tmpbuf, colors::yellow);

    //sprintf(tmpbuf, "Interp. : %s", spc_config.is_interpolation ? "On" : "Off");  
    //sdlfont_drawString(screen, INFO_X, INFO_Y+64, tmpbuf, colors::white);
  
    //sprintf(tmpbuf, "Autowrite mask.: %s", g_cfg_autowritemask ? "Yes" : "No");
    //sdlfont_drawString(screen, INFO_X, INFO_Y+72, tmpbuf, colors::white);

    track::update_tag();

    sprintf(tmpbuf, "Ignore tag time: %s", g_cfg_ignoretagtime ? "Yes" : "No");
    sdlfont_drawString(screen, INFO_X, INFO_Y+80, tmpbuf, colors::white);

    sprintf(tmpbuf, "Default time...: %d:%02d", g_cfg_defaultsongtime/60, g_cfg_defaultsongtime%60);
    sdlfont_drawString(screen, INFO_X, INFO_Y+88, tmpbuf, colors::white);

    
    sdlfont_drawString(screen, PORTTOOL_X, PORTTOOL_Y, "     - Port tool -", colors::white);
  }
}

void pack_mask(unsigned char packed_mask[32])
{
  int i;
  
  memset(packed_mask, 0, 32);
  for (i=0; i<256; i++)
  {
    if (used2[i])
    packed_mask[i/8] |= 128 >> (i%8);
  }
}

void fade_arrays()
{
  memsurface.fade_arrays();
}

static int audio_samples_written=0;

void applyBlockMask(char *filename)
{
  FILE *fptr;
  unsigned char nul_arr[256];
  int i;

  memset(nul_arr, g_cfg_filler, 256);
  
  fptr = fopen(filename, "r+");
  if (!fptr) { perror("fopen"); }

  printf("[");
  for (i=0; i<256; i++)
  {
    fseek(fptr, 0x100+(i*256), SEEK_SET);
    
    if (!used2[i]) {
      printf(".");
      fwrite(nul_arr, 256, 1, fptr);
    } else {
      printf("o");
    }
    fflush(stdout);
  }
  printf("]\n");
  
  fclose(fptr);
}

void write_mask(unsigned char packed_mask[32])
{
  FILE *msk_file;
  char *sep;
  char filename[1024];
  unsigned char tmp;
  int i;
  strncpy(filename, g_cfg_playlist[g_cur_entry], 1024);
#ifdef WIN32
  sep = strrchr(filename, '\\');
#else
  sep = strrchr(filename, '/');
#endif
  // keep only the path
  if (sep) { sep++; *sep = 0; } 
  else { 
    filename[0] = 0; 
  }

  // add the filename
  strncat(filename, g_real_filename, 1024);

  // but remove the extension if any
  sep = strrchr(filename, '.');
  if (sep) { *sep = 0; }

  // and use the .msk extension
  strncat(filename, ".msk", 1024);

  msk_file = fopen(filename, "wb");
  if (!msk_file) {
    perror("fopen");
  }
  else
  {
    fwrite(packed_mask, 32, 1, msk_file);
  }

  printf("Writing mask to '%s'\n", filename);

  // the first 32 bytes are for the 256BytesBlock mask
  printf("256 Bytes-wide block mask:\n");
  for (i=0; i<32; i++) {
    printf("%02X",packed_mask[i]);
  }
  printf("\n");

  printf("Byte level mask..."); fflush(stdout);
  memset(packed_mask, 0, 32);
  for (i=0; i<65536; i+=8)
  {
    tmp = 0;
    if (used[i]) tmp |= 128;
    if (used[i+1]) tmp |= 64;
    if (used[i+2]) tmp |= 32;
    if (used[i+3]) tmp |= 16;
    if (used[i+4]) tmp |= 8;
    if (used[i+5]) tmp |= 4;
    if (used[i+6]) tmp |= 2;
    if (used[i+7]) tmp |= 1;
    fwrite(&tmp, 1, 1, msk_file);
  }
  printf("Done.\n");
  fclose(msk_file);
}


static char *marquees[3] = { (char*)CREDITS, track::now_playing, NULL };
static char *cur_marquee = NULL;
static int cur_marquee_id = 0;

void do_scroller(int elaps_milli)
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

  keep += elaps_milli;  
  
  steps = keep*60/900;
  if (!steps) { return; }

  elaps_milli = keep;
  keep=0;

  if (cur_marquee == NULL) { 
    cur_marquee = marquees[cur_marquee_id]; 
    p = screen->w;
  }
  
  cur_len = strlen(cur_marquee);
  cur_min = -cur_len*8;

  angle = start_angle;
        
  cs = player->emu()->tell() / 1010;
  cs %= 12;

  // clear area 
  tmprect.x = 0;
  tmprect.y = 0;
  tmprect.w = screen->w;
  tmprect.h = 28;
  SDL_FillRect(screen, &tmprect, colors::black);
  
  
  for (i=0; i<cur_len; i++)
  {
    off = sin(angle)*8.0;
    c[0] = cur_marquee[i];
    if (  (tmprect.x + i*8 + p > 0) && (tmprect.x + i*8 + p < screen->w) )
    {
      sdlfont_drawString(screen, tmprect.x + i*8 + p, 12 + off, c, colors::colorscale[cs]);
    }
    angle-=0.1;
  }
  start_angle += steps * 0.02;
  p-=steps;

  if (p<cur_min) {
    if (marquees[cur_marquee_id+1]!=NULL) {
      cur_marquee = marquees[++cur_marquee_id];
    }
    else {
      p = screen->w;
    }
  }
}

void base_mode_game_loop()
{
  
  // this is the address that shows up after "Addr Mouse"

reload:
  reload();
  
  for (;;) 
  {
    
    SDL_Event ev;

    pack_mask(packed_mask);
    
    if (g_cfg_statusline) {
      printf("%s  %d / %d (%d %%)        \r", 
          g_cfg_playlist[g_cur_entry],
          audio_samples_written/44100,
          track::song_time,
          (audio_samples_written/44100)/(track::song_time));
      fflush(stdout);
    }
    
    /* Check if it is time to change tune.
     */   
    if (player->emu()->tell()/1000 >= track::song_time) 
    {
      if (g_cfg_autowritemask) {
        write_mask(packed_mask);
        if (g_cfg_apply_block) {
          printf("Applying mask on file %s using $%02X as filler\n", g_cfg_playlist[g_cur_entry], g_cfg_filler);
          applyBlockMask(g_cfg_playlist[g_cur_entry]);
        }
      }
      g_cur_entry++;
      if (g_cur_entry>=g_cfg_num_files) { printf ("penis3\n"); goto clean; }
      goto reload;
    }
    
    if (!g_cfg_novideo)
    {
      while (SDL_PollEvent(&ev))
      {
        dblclick::check_event(&ev);
        switch (ev.type)
        {
          case SDL_QUIT:
            if (!g_cfg_nosound) {
              SDL_PauseAudio(1);
            }
            printf ("penis4\n");
            goto clean;
            break;
          case SDL_MOUSEMOTION:
            {
              mouse::x = ev.motion.x; mouse::y = ev.motion.y;

              if (ev.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
              {
                voices::checkmouse_mute(ev.motion.x, ev.motion.y);
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
                  //set_addr(y*256+x);
                  
                  mouse_addr = y*256+x;
                  if (!mouse_hexdump::locked) {
                    mouse_hexdump::set_addr(mouse_addr); //_from_cursor(x,y);
                  }
                }
              }
            
            }
            break;
          case SDL_KEYDOWN:
          {
            int scancode = ev.key.keysym.sym;
            if (scancode == SDLK_m)
            {
              memcursor::toggle_disable();
            }
            if (scancode == SDLK_k)
            {
              player->spc_write_dsp(dsp_reg::kon,0x0);
            }
            if (ev.key.keysym.sym == SDLK_u)
            {
              //player->spc_write_dsp(dsp_reg::voice0_pitch_lo, 0xff);
              //player->spc_write_dsp(dsp_reg::voice0_pitch_hi, 0x63);
              /*player->spc_write_dsp(dsp_reg::eon, 0x1);
              player->spc_write_dsp(dsp_reg::flg, 0x00);
              player->spc_write_dsp(dsp_reg::esa, 0x50);
              player->spc_write_dsp(dsp_reg::edl, 0x0f);
              player->spc_write_dsp(dsp_reg::efb, 0x40);
              player->spc_write_dsp(dsp_reg::evol_l, 127);
              player->spc_write_dsp(dsp_reg::evol_r, 127);
              player->spc_write_dsp(dsp_reg::c0, 0x7f);
              player->spc_write_dsp(dsp_reg::kon,0x1);*/
              player->gain -= 0.01;
              fprintf(stderr, "gain = %f", player->gain, INT16_MIN, INT16_MAX);
              //player->spc_write(0xf2, 0x4c);
              //player->spc_write(0xf3, 0);
              //player->spc_write(0xf3, 1);
            }
            else if (ev.key.keysym.sym == SDLK_i)
            {
              player->gain += 0.01;
              fprintf(stderr, "gain = %f", player->gain);
            }
            else if (scancode == SDLK_o)
            {
              ::filter_is_active = !::filter_is_active;
              fprintf(stderr, "filter_is_active = %d", ::filter_is_active);
            }
            else if (scancode == SDLK_SPACE)
            {
              player->toggle_pause();
            }
            else if (scancode == SDLK_r)
            {
              player->start_track(0); // based on only having 1 track
              player->pause(0);
              // in the program .. this would have to change otherwise
            }
            if (mode == MODE_NAV)
            {
              // VOICES!!! 
              if ((scancode >= '0') && (scancode <= '9'))
              {
                uint8_t i = scancode - '0';
                if (!i)
                  voices::toggle_mute_all();
                if (i > 0 && i < 9)
                  voices::toggle_mute(i); // channel num
                //else
                  //voices::mute_all();
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
                mouse_hexdump::highnibble = 1;
                mouse_hexdump::res_x = 0;
                mouse_hexdump::res_y = 0;

                mode = MODE_EDIT_MOUSE_HEXDUMP;
                submode = mouse_hexdump::EASY_EDIT;
                // order matters .. call here: 
                mouse_hexdump::lock();
                //mouse_hexdump::addr_being_edited = mouse_hexdump::address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x
                //mouse_hexdump::res_x = 1; //mouse_hexdump::address_remainder;
                
                cursor::start_timer();
              }
              if (ev.key.keysym.sym == SDLK_ESCAPE)
              {
                if (mouse_hexdump::locked)
                {
                  mouse_hexdump::unlock(); 
                }
                else
                {
                  if (!g_cfg_nosound) {
                    SDL_PauseAudio(1);
                  }
                  goto clean;
                }
              }
            }
            else if (mode == MODE_EDIT_MOUSE_HEXDUMP)
            {
              int scancode = ev.key.keysym.sym;
              if (ev.key.keysym.mod & (CMD_CTRL_KEY))  // GUI in SDL2
              {
                fprintf(stderr, "EOO");
                if (scancode == SDLK_LEFT)
                  prev_track();
                else if (scancode == SDLK_RIGHT)
                  next_track();
              }
              else if (scancode == 'h' || scancode == 'H')
              {
                mouse_hexdump::horizontal = !mouse_hexdump::horizontal;
              }
              else if ( ((scancode >= '0') && (scancode <= '9')) || ((scancode >= 'A') && (scancode <= 'F')) || 
                ((scancode >= 'a') && (scancode <= 'f')) )
              {
                uint i=0;
                //int addr;
                Uint16 addr = mouse_hexdump::addr_being_edited;
                //fprintf(stderr, "Addr = %04x\n", addr);
                
                if ((scancode >= '0') && (scancode <= '9'))
                  i = scancode - '0';
                else if ((scancode >= 'A') && (scancode <= 'F'))
                  i = (scancode - 'A') + 0x0a;
                else if ((scancode >= 'a') && (scancode <= 'f'))
                  i = (scancode - 'a') + 0x0a;  

                // test reg is unimplemented but i'll leave this here..
                if ( (addr == 0xf3 && (IAPURAM[0xf2] == 0x4c || IAPURAM[0xf2] == 0x5c) ) || 
                  addr==0xf1 || addr == 0xf0 || (addr >= 0xf4 && addr <= 0xf7)  )
                {
                  // only update the buffer the first time.. if we haven't started writing in a new value
                  if (!mouse_hexdump::draw_tmp_ram)
                  {
                    if (addr == 0xf1 || (addr >= 0xf4 && addr <= 0xf7)  )
                      mouse_hexdump::tmp_ram = IAPURAM[addr];
                    else mouse_hexdump::tmp_ram = player->spc_read(addr);

                    fprintf(stderr, "tmpram = 0x%02x\n", mouse_hexdump::tmp_ram);
                  }
                }
                else 
                {
                  if (mouse_hexdump::draw_tmp_ram) mouse_hexdump::draw_tmp_ram = 0;
                  mouse_hexdump::tmp_ram = player->spc_read(addr);
                }
                if (mouse_hexdump::highnibble)
                {
                  i <<= 4;
                  i &= 0xf0;
                  //IAPURAM[mouse_hexdump::address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x] &= 0x0f;
                  mouse_hexdump::tmp_ram &= 0x0f;
                }
                else
                {
                  i &= 0x0f;
                  //IAPURAM[mouse_hexdump::address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x] &= 0xf0;
                  mouse_hexdump::tmp_ram &= 0xf0;
                }

                //IAPURAM[mouse_hexdump::address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x] |= i;
                mouse_hexdump::tmp_ram |= i;

                if ( (addr == 0xf3 && (IAPURAM[0xf2] == 0x4c || IAPURAM[0xf2] == 0x5c) ) || 
                  addr==0xf1 || addr == 0xf0 || (addr >= 0xf4 && addr <= 0xf7)  )
                {
                  if (!mouse_hexdump::highnibble)
                  {
                    if (addr >= 0xf4 && addr <= 0xf7)
                    {
                      porttool::write(addr-0xf4, mouse_hexdump::tmp_ram);
                      //player->spc_write_port()
                    }
                    else 
                    {
                      player->spc_write(addr, mouse_hexdump::tmp_ram);
                      fprintf(stderr, "WRite");
                    }
                    mouse_hexdump::draw_tmp_ram = 0;
                  }
                  else mouse_hexdump::draw_tmp_ram = 1;
                }
                else player->spc_write(addr, mouse_hexdump::tmp_ram);
                
                if (mouse_hexdump::horizontal) mouse_hexdump::inc_cursor_pos();
                
              }
              /*else if (scancode == SDLK_SPACE)
              {
                mouse_hexdump::inc_cursor_pos();
              }*/
              else if (scancode == SDLK_TAB)
              {
                mouse_hexdump::inc_cursor_pos();
                mouse_hexdump::inc_cursor_pos();
              }
              else if (scancode == SDLK_BACKSPACE)
              {
                // eh
                int i = mouse_hexdump::address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x;
                while (i < (0x10000) )
                {
                  player->spc_write(i-1, player->spc_read(i));
                  //IAPURAM[i-1] = IAPURAM[i];
                  i++;
                }
                mouse_hexdump::dec_cursor_pos();
                mouse_hexdump::dec_cursor_pos();
                mouse_hexdump::highnibble=1;
              }
              else if (scancode == SDLK_DELETE)
              {
                // eh
                int i = mouse_hexdump::address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x;
                while (i < (0x10000) )
                {
                  player->spc_write(i, player->spc_read(i+1));
                  //IAPURAM[i] = IAPURAM[i+1];
                  i++;
                }
                mouse_hexdump::highnibble=1;
              }
              else if (scancode == SDLK_LEFT)
              {
                mouse_hexdump::dec_cursor_pos();
              }
              else if (scancode == SDLK_RIGHT)
              {
                mouse_hexdump::inc_cursor_pos();
              }
              else if (scancode == SDLK_UP)
              {
                mouse_hexdump::dec_cursor_row();
              }
              else if (scancode == SDLK_DOWN)
              {
                mouse_hexdump::inc_cursor_row();
              }

              if (ev.key.keysym.sym == SDLK_ESCAPE || ev.key.keysym.sym == SDLK_RETURN)
              {
                exit_edit_mode();
              }
            }   
            else if (mode == MODE_EDIT_APU_PORT)
            {

              int scancode = ev.key.keysym.sym;

              if (scancode == 'h' || scancode == 'H')
              {
                porttool::horizontal = !porttool::horizontal;
              }

              if ( ((scancode >= '0') && (scancode <= '9')) || ((scancode >= 'A') && (scancode <= 'F')) || 
                ((scancode >= 'a') && (scancode <= 'f')))
              {
                Uint8 i;

                i = cursor::scancode_to_hex(scancode); 

                if (porttool::highnibble)
                {
                  i <<= 4;
                  i &= 0xf0;
                  //IAPURAM[mouse_hexdump::address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x] &= i;
                  porttool::tmp[porttool::portnum] &= 0x0f;
                }
                else
                {
                  i &= 0x0f;
                  porttool::tmp[porttool::portnum] &= 0xf0;
                }

                porttool::tmp[porttool::portnum] |= i;
                
                if (porttool::horizontal) porttool::inc_cursor_pos();
              }
              else if (scancode == SDLK_SPACE)
              {
                porttool::inc_cursor_pos();
              }
              else if (scancode == SDLK_TAB)
              {
                porttool::inc_cursor_pos();
                porttool::inc_cursor_pos();
              }
              else if (scancode == SDLK_BACKSPACE)
              {
                if (porttool::highnibble == 0)
                {
                  porttool::tmp[porttool::portnum] &= 0x0f;
                  porttool::dec_cursor_pos();
                }
              }
              else if (scancode == SDLK_DELETE)
              {
                if (porttool::highnibble)
                {
                  porttool::tmp[porttool::portnum] &= 0x0f;
                }
                else porttool::tmp[porttool::portnum] &= 0xf0;
              }
              else if (scancode == SDLK_LEFT)
              {
                porttool::dec_cursor_pos();
              }
              else if (scancode == SDLK_RIGHT)
              {
                porttool::inc_cursor_pos();
              }
              else if (scancode == SDLK_UP)
              {
                if (porttool::highnibble)
                {
                  porttool::tmp[porttool::portnum] += 0x10;
                }
                else
                {
                  Uint8 tmp = porttool::tmp[porttool::portnum] + 1;
                  tmp &= 0x0f;
                  porttool::tmp[porttool::portnum] &= 0xf0;
                  porttool::tmp[porttool::portnum] |= tmp;
                }
              }
              else if (scancode == SDLK_DOWN)
              {
                if (porttool::highnibble)
                {
                  porttool::tmp[porttool::portnum] -= 0x10;
                }
                else
                {
                  Uint8 tmp = porttool::tmp[porttool::portnum] - 1;
                  tmp &= 0x0f;
                  porttool::tmp[porttool::portnum] &= 0xf0;
                  porttool::tmp[porttool::portnum] |= tmp;
                }
              }

              if (ev.key.keysym.sym == SDLK_ESCAPE)
              {
                mode = MODE_NAV;
                cursor::stop_timer();
                porttool::reset_port();
              }
              else if (ev.key.keysym.sym == SDLK_RETURN)
              {
                porttool::write();
              }
            }       
          }
            break;
        
          case SDL_USEREVENT:
          {
            if (ev.user.code == UserEvents::mouse_react)
            {
              SDL_Event *te = (SDL_Event *)ev.user.data1;
              if (te->motion.x >= (MOUSE_HEXDUMP_START_X - 2) && te->motion.x <= (MOUSE_HEXDUMP_END_X + 2) &&
                te->motion.y >= MOUSE_HEXDUMP_START_Y && te->motion.y <= MOUSE_HEXDUMP_END_Y)
              {
                // editor stuffz
                Uint8 oldmode = mode;
                mode = MODE_EDIT_MOUSE_HEXDUMP;
                

                int res_x, res_y, highnibble;

                const int entry_width = MOUSE_HEXDUMP_ENTRY_X_INCREMENT;
                const int entry_height = MOUSE_HEXDUMP_ENTRY_Y_INCREMENT;

                mouse_hexdump::rel_x = te->motion.x - MOUSE_HEXDUMP_START_X;
                mouse_hexdump::rel_x+=2;
                mouse_hexdump::rel_y = te->motion.y - MOUSE_HEXDUMP_START_Y;

                res_x = mouse_hexdump::rel_x / entry_width;
                res_y = mouse_hexdump::rel_y / entry_height;

                int res_half = mouse_hexdump::rel_x % entry_width;
                int tmp = entry_width / 2;

                if (res_half < tmp) highnibble = 1;
                else highnibble = 0;

                if (oldmode == MODE_EDIT_MOUSE_HEXDUMP)
                {
                  if (res_x == mouse_hexdump::res_x && res_y == mouse_hexdump::res_y && highnibble == mouse_hexdump::highnibble)
                  {
                    mode = MODE_NAV;
                    cursor::stop_timer();
                    mouse_hexdump::unlock();
                    break;
                  }
                }
                
               

                // order matters .. call here: 
                mouse_hexdump::lock(1,0,0,res_x,res_y);
                mouse_hexdump::highnibble = highnibble;
                //mouse_hexdump::res_x = res_x;
                //mouse_hexdump::res_y = res_y;

                
                if (mouse_hexdump::res_y == 16) mouse_hexdump::res_y = 15;
              }

              /* porttool */
              else if ( (te->button.x >= PORTTOOL_X + (8*5)) &&
                  te->button.y >= PORTTOOL_Y && te->button.y < (PORTTOOL_Y + 16))
              {
                int x, y;
                x = te->button.x - (PORTTOOL_X + (8*5));
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
                    porttool::x = ((PORTTOOL_X + (8*5)) + (x*8));
                }
                
                y = te->button.y - PORTTOOL_Y;
                y /= 8;

                if (te->button.button == SDL_BUTTON_LEFT)
                {
                  switch (x)
                  {
                    // i think single click takes care of this
                    //case 1: IAPURAM[0xf4]++; break;
                    case 2:
                    {
                      porttool::set_port(0);

                      porttool::highnibble = 1;
                      mode = MODE_EDIT_APU_PORT;
                      cursor::start_timer();
                    } break;
                    case 3:
                    {
                      porttool::set_port(0);
                      porttool::highnibble = 0;
                      mode = MODE_EDIT_APU_PORT;
                      cursor::start_timer();
                    } break;
                    //case 4: IAPURAM[0xf4]--; break;
                    //case 6: IAPURAM[0xf5]++; break;
                    case 7:
                    {
                      porttool::set_port(1);
                      porttool::highnibble = 1;
                      mode = MODE_EDIT_APU_PORT;
                      cursor::start_timer();
                    } break;
                    case 8:
                    {
                      porttool::set_port(1);
                      porttool::highnibble = 0;
                      mode = MODE_EDIT_APU_PORT;
                      cursor::start_timer();
                    } break;
                    //case 9: IAPURAM[0xf5]--; break;
                    //case 11: IAPURAM[0xf6]++; break;
                    case 12:
                    {
                      porttool::set_port(2);
                      porttool::highnibble = 1;
                      mode = MODE_EDIT_APU_PORT;
                      cursor::start_timer();
                    } break;
                    case 13:
                    {
                      porttool::set_port(2);
                      porttool::highnibble = 0;
                      mode = MODE_EDIT_APU_PORT;
                      cursor::start_timer();
                    } break;
                    //case 14: IAPURAM[0xf6]--; break;
                    //case 16: IAPURAM[0xf7]++; break;
                    case 17:
                    {
                      porttool::set_port(3);
                      porttool::highnibble = 1;
                      mode = MODE_EDIT_APU_PORT;
                      cursor::start_timer();
                    } break;
                    case 18:
                    {
                      porttool::set_port(3);
                      porttool::highnibble = 0;
                      mode = MODE_EDIT_APU_PORT;
                      cursor::start_timer();
                    } break;
                    //case 19: IAPURAM[0xf7]--; break;
                  }
                }
              }
            }
          } break;
          case SDL_MOUSEBUTTONDOWN:           
            {
              voices::checkmouse(ev.motion.x, ev.motion.y, ev.button.button); 

              bool is_in_memory_window= (ev.motion.x >= MEMORY_VIEW_X && 
                    ev.motion.x < MEMORY_VIEW_X + 512 &&
                    ev.motion.y >= MEMORY_VIEW_Y &&
                    ev.motion.y < MEMORY_VIEW_Y + 512) ? true:false;

              

              if (  ev.motion.x >= screen_pos::locked.x && 
                    ev.motion.x < screen_pos::locked.x + screen_pos::locked.w &&
                    ev.motion.y >= screen_pos::locked.y &&
                    ev.motion.y < screen_pos::locked.y + 9 )
              {
                if(mouse_hexdump::locked)
                  mouse_hexdump::toggle_lock();
              }
              else if (  ev.motion.x >= screen_pos::echoE.x && 
                    ev.motion.x < screen_pos::echoE.x + screen_pos::echoE.w &&
                    ev.motion.y >= screen_pos::echoE.y &&
                    ev.motion.y < (screen_pos::echoE.y + screen_pos::echoE.h) )
              {
                // toggle_echo()
                player->spc_emu()->toggle_echo();
              }

              else if (mode == MODE_NAV)
              {
                
                if (  ev.motion.x >= INFO_X+(10*8) && 
                    ev.motion.x < INFO_X+(13*8) &&
                    ev.motion.y >= INFO_Y+56 &&
                    ev.motion.y < INFO_Y+56+9 )
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
                    mouse_hexdump::toggle_lock(ev.motion.x, ev.motion.y);
                  }
                }
              }

              else if (mode == MODE_EDIT_MOUSE_HEXDUMP)
              {
                if (is_in_memory_window && ev.button.button == SDL_BUTTON_LEFT)
                {
                  exit_edit_mode();
                  mouse_hexdump::set_addr_from_cursor(ev.motion.x, ev.motion.y);
                }
              }
              
              if (ev.motion.x >= (MOUSE_HEXDUMP_START_X - 2) && ev.motion.x <= (MOUSE_HEXDUMP_END_X + 2) &&
                ev.motion.y >= MOUSE_HEXDUMP_START_Y && ev.motion.y <= MOUSE_HEXDUMP_END_Y)
              {
                if (ev.button.button == SDL_BUTTON_RIGHT)
                {
                  mouse_hexdump::toggle_lock();
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
                    case 1: porttool::dec_port (0); break;
                    case 4: porttool::inc_port (0); break;
                    case 6: porttool::dec_port (1); break;
                    case 9: porttool::inc_port (1); break;
                    case 11: porttool::dec_port(2); break;
                    case 14: porttool::inc_port(2); break;
                    case 16: porttool::dec_port(3); break;
                    case 19: porttool::inc_port(3); break;
                  }
                }
                if (ev.button.button == SDL_BUTTON_WHEELUP ||
                  ev.button.button == SDL_BUTTON_WHEELDOWN)
                {
                  Uint8 i;
                  if (ev.button.button == SDL_BUTTON_WHEELUP) { i=1; } else { i = -1; }
                  if (x>1 && x<4) { inc_ram(0xf4, i); }
                  if (x>6 && x<9) { inc_ram(0xf5, i); }
                  if (x>11 && x<14) { inc_ram(0xf6, i); }
                  if (x>16 && x<19) { inc_ram(0xf7, i); }
                }
              } 


              if (ev.button.button == SDL_BUTTON_WHEELUP)
              {
                mouse_hexdump::add_addr(-0x08);
                break;          
              }
              else if (ev.button.button == SDL_BUTTON_WHEELDOWN)
              {
                mouse_hexdump::add_addr(0x08);
                break;
              }

              /* menu bar */
              else if (
                ((ev.button.y >screen->h-12) && (ev.button.y<screen->h)))
              {
                int x = ev.button.x / 8;
                if (x>=1 && x<=4) { printf ("penis5\n");goto clean; } // exit
                if (x>=8 && x<=12) { 
                  toggle_pause();
                } // pause

                if (x>=16 && x<=22) {  // restart
                  restart_track();
                }

                if (x>=26 && x<=29) {  // prev
                  SDL_PauseAudio(1);
                  prev_track();
                }

                if (x>=33 && x<=36) { // next
                  next_track();
                }

                if (x>=41 && x<=50) { // write mask
                  write_mask(packed_mask);
                }

                if (x>=53 && x<=59) { // DSP MAP
                  //write_mask(packed_mask);
                  mode = MODE_DSP_MAP;
                }
              }
            }
            break;
            default:
            break;
        }
      } // while (pollevent)
      
    } // !g_cfg_novideo

    if (!g_cfg_novideo)
    {
      time_cur = SDL_GetTicks();
      do_scroller(time_cur - time_last);
      
      fade_arrays();      
      
      // draw the memory read/write display area
      memsurface.draw(screen);

      draw_block_usage_bar();

      // The following are correlated from i and tmp. DO NOT MESS WITH THAT
      // base height
      i = 32;      
      draw_mouse_address();
      draw_program_counter();
      draw_voices_pitchs(); // tmp is fucked with inside this function. DONT MESS
      draw_voices_volumes();
      draw_main_volume();
      draw_echo_volume();
      draw_mouseover_hexdump();
      draw_porttool();
      draw_time_and_echo_status();

      


      
      
      if (mode == MODE_EDIT_MOUSE_HEXDUMP)
      {
        mouse_hexdump::draw_cursor(screen, colors::green);
      }
      else if (mode == MODE_EDIT_APU_PORT)
      {
        porttool::draw_cursor(screen, colors::green);
      }

      // toggle should be 0 ALWAYS when deactivated
      if (memcursor::is_active())
      {
        if (memcursor::is_toggled())
        {
          report_cursor(mouse_hexdump::addr_being_edited);
        }
        else report::restore_color(mouse_hexdump::addr_being_edited);
      }
      

      if (mouse::show)
      {
        if (mouse::x < (screen->w-40) && mouse::y < (screen->h - 8))
        { 
          sprintf(tmpbuf, "(%d,%d)", mouse::x, mouse::y);
          sdlfont_drawString(screen, mouse::x, mouse::y, tmpbuf, colors::white);
        }
      }
      
      SDL_UpdateRect(screen, 0, 0, 0, 0);
      time_last = time_cur;
      if (g_cfg_nice) {  SDL_Delay(100); }
      //SDL_Delay( 1000 / 100 );
    } // if !g_cfg_novideo
    is_first_run = false;
  }

  clean:
  ;
}

void draw_program_counter()
{
  // write the program counter
  last_pc = (int)player->spc_emu()->pc(); 
  sprintf(tmpbuf, "PC: $%04x  ", last_pc);
  sdlfont_drawString(screen, MEMORY_VIEW_X+8*12, MEMORY_VIEW_Y-10, tmpbuf, colors::white);
}

void draw_voices_pitchs()
{
  tmp = i+10; // y 
  sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp, "Voices pitches:", colors::white);
  tmp += 9;
  
  tmprect.x=MEMORY_VIEW_X+520;
  tmprect.y=tmp;
  tmprect.w=screen->w-tmprect.x;
  tmprect.h=8*8;
  SDL_FillRect(screen, &tmprect, colors::black);
  tmprect.w=5;
  tmprect.h = 5;
  for (i=0; i<8; i++)
  {
    
    unsigned short pitch = (player->spc_read_dsp(2+(i*0x10)) | (player->spc_read_dsp(3+(i*0x10))<<8)) & 0x3fff; 
    // I believe pitch is max 0x3fff but kirby is using higher values for some unknown reason...
    //Uint32 gray = colors::gray;
    Uint32 *cur_color= &colors::gray;

    uint8_t voice_base_addr = (i*0x10);
    uint8_t outx = player->spc_read_dsp(voice_base_addr+0x09);
    uint8_t envx = player->spc_read_dsp(voice_base_addr+0x08);

    if (player->spc_read_dsp(0x4c)&(1<<i) && !(voices::was_keyed_on & i) )
    {
      cur_color = &colors::white;
      voices::was_keyed_on |= 1<<i;
    } else if (player->spc_read_dsp(0x5c)&(1<<i)) {
      //cur_color = &colors::gray;
      voices::was_keyed_on &= ~(1<<i);
      //if (i==1)
        //fprintf(stderr, "KEYOFF\n");
    }

    if (outx || envx) 
    {
      cur_color = &colors::white;
    }
    else // check if the sample is looping
    {
      
      // I added this section because when outx reaches 0
      // the voice will go black for split second. this actually
      // happens hundreds or thousands of times a second but the visual
      // only catches it once in awhile.. but it's annoying and 
      // I don't like it.. so I coded this to take up your CPU
      if (voices::was_keyed_on & (1<<i))
      {
        uint16_t addr = player->spc_read_dsp(0x5d) * 0x100;
        //fprintf(stderr,"0x%04x,", addr);
        uint8_t samp_index;
        samp_index = player->spc_read_dsp(voice_base_addr+0x04);
        //fprintf(stderr,"0x%02x,", samp_index);
        uint16_t *brr_header_addr = (uint16_t*)&IAPURAM[addr+(samp_index*4)];
        if (IAPURAM[*brr_header_addr] & 2)
          cur_color = &colors::white;
        /*else if (outx) 
        {
          cur_color = &colors::white;
        }*/
      }
      else
      {
        //if (i==1)
          //fprintf(stderr, "WHAT");
        // the note is truly dead
      }
        /*else if (outx) 
        {
          cur_color = &colors::white;
        }*/

        //fprintf(stderr,"0x%04x\n", *brr_header_addr);

      //}
    }


    if (voices::is_muted(i))
    {
      /* I would use sub color here to
      get a view of the activity but the mute happens at the DSP level
      it invokes KOFF on the muted channel making it impossible to get its
      real status without hacking it at deep level */
      /* i will leave out the pointers in the color logic still ... altho
      the pointers are faster and can be used if the pointed-to-value is
      not changed */
      cur_color = &colors::nearblack;
      //colors::subtract(&cur_color, 0x60);
    }

    int x =MEMORY_VIEW_X+520;
    int y = tmp+ (i*8);
    sprintf(tmpbuf,"%d:",i);
    sdlfont_drawString(screen, x, y, tmpbuf, *cur_color);
    if (::is_first_run && i == 0)
    {
      screen_pos::voice0pitch.x = x;
      screen_pos::voice0pitch.y = y;
    }
    
    tmprect.y= tmp+(i*8)+2;
    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.x += pitch*(screen->w-tmprect.x-20)/((0x10000)>>2);
    if (voices::is_muted(i))
      SDL_FillRect(screen, &tmprect, colors::nearblack);
    else
      SDL_FillRect(screen, &tmprect, colors::white);
    
  }
}

void draw_voices_volumes()
{
  tmp += 9*8;

  sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp, "Voices volumes:", colors::white);
  sdlfont_drawString(screen, MEMORY_VIEW_X+520+(16*8), tmp, "Left", colors::yellow);      

  sdlfont_drawString(screen, MEMORY_VIEW_X+520+(20*8)+4, tmp, "Right", colors::cyan);
  sdlfont_drawString(screen, MEMORY_VIEW_X+520+(26*8), tmp, "Gain", colors::magenta);
  tmp += 9;

  tmprect.x=MEMORY_VIEW_X+520;
  tmprect.y=tmp;
  tmprect.w=screen->w-tmprect.x;
  tmprect.h=10*11;
  SDL_FillRect(screen, &tmprect, colors::black);   
  tmprect.w=2;
  tmprect.h=2;

  for (i=0; i<8; i++)
  {
    Uint32 c1 = colors::white, c2 = colors::gray, c3 = colors::magenta;
    Uint32 *Color1=&c1, *Color2 = &c1;
    
    unsigned char is_inverted=0;
    unsigned char left_vol = player->spc_read_dsp(0+(i*0x10));
    unsigned char right_vol = player->spc_read_dsp(1+(i*0x10));
    unsigned char adsr1 = player->spc_read_dsp(5+(i*0x10));
    unsigned char gain;
    if (adsr1 & 0x80) gain=0;
    else gain = player->spc_read_dsp(7+(i*0x10));

    
    
    //Value -128 can be safely used
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

    if (voices::is_muted(i))
      color = &colors::nearblack;
    else 
      color = &colors::white;

    sdlfont_drawString(screen, x, y, tmpbuf, *color);
    if (::is_first_run && i == 0)
    {
      screen_pos::voice0vol.x = x;
      screen_pos::voice0vol.y = y;  
    }
    
    sprintf(tmpbuf,"\x1");
    if (voices::is_muted(i))
    {
      colors::subtractp(Color1,0x60);
      colors::subtractp(Color2,0x60);
    }
    sdlfont_drawString2c(screen, MEMORY_VIEW_X+520 + 8, tmp + (i*10), tmpbuf, *Color1, *Color2);
    

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10);
    tmprect.w = left_vol*(screen->w-tmprect.x-20)/255;

    
    // L volume
    if (voices::is_muted(i))
      color = &colors::dark_yellow;
    else color = &colors::yellow;
    SDL_FillRect(screen, &tmprect, *color);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+3;
    
    if (voices::is_muted(i))
      color = &colors::dark_cyan;
    else color = &colors::cyan;
    SDL_FillRect(screen, &tmprect, *color);

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
    tmprect.w = gain * (screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+6;
    if (voices::is_muted(i))
    {
      Uint8 r1,g1,b1;
      SDL_GetRGB(c3, screen->format, &r1, &b1, &g1);
      c3 = SDL_MapRGB(screen->format,r1-0x60,g1-0x60,b1-0x60);
    }

    SDL_FillRect(screen, &tmprect, c3);
  }
}

void draw_global_volumes()
{
  draw_main_volume();
  draw_echo_volume();
}

void draw_main_volume()
{
  i=9;
  // 
  {
    Uint32 *Color1=&colors::white, *Color2 = &colors::white;
    Uint32 *thecolor = &colors::gray;
    unsigned char is_inverted=0;
    unsigned char left_vol = player->spc_read_dsp(dsp_reg::mvol_l);
    unsigned char right_vol = player->spc_read_dsp(dsp_reg::mvol_r);
    //unsigned char gain = player->spc_read_dsp(7+(i*0x10));

    //Value -128 canNOT be safely used
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
        Color1 = &colors::red; // this is bad value according to FullSNES
      else Color1 = thecolor;
    }
    if (is_inverted & (1 << R_FLAG) )
    {
      if (right_vol == 0x80)
        Color2 = &colors::red; // this is bad value according to FullSNES
      Color2 = thecolor;
    }

    sprintf(tmpbuf,"M");
    sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp + (i*10), tmpbuf, colors::white);
    sprintf(tmpbuf,"\x1");
    sdlfont_drawString2c(screen, MEMORY_VIEW_X+520+8, tmp + (i*10), tmpbuf, *Color1, *Color2);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10)+1;
    tmprect.w = left_vol*(screen->w-tmprect.x-20)/255;

    SDL_FillRect(screen, &tmprect, colors::yellow);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+4;
    
    SDL_FillRect(screen, &tmprect, colors::cyan);
  }
}

void draw_echo_volume()
{
  i++;
  {
    Uint32 Color1=colors::white;
    Uint32 Color2=colors::white;// = &colors::white;
    Uint32 *leftbarvol_col=&colors::yellow;
    Uint32 *rightbarvol_col=&colors::cyan;
    //Uint32 thecolor = &
    unsigned char is_inverted=0;
    unsigned char left_vol = player->spc_read_dsp(dsp_reg::evol_l);
    unsigned char right_vol = player->spc_read_dsp(dsp_reg::evol_r);
    
    //Value -128 can be safely used
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
      Color1 = colors::gray;
    }
    if (is_inverted & (1 << R_FLAG) )
    {
      Color2 = colors::gray;
    }

    //
    Uint32 *c;
    sprintf(tmpbuf,"E");
    if (::is_first_run)
    {
      screen_pos::echoE.y = tmp+(i*10);
    }

    if (player->spc_emu()->is_echoing())
    {
      c = &colors::white;
    }
    else
    {
      c = &colors::nearblack;
      colors::subtractp(&Color1, 0x60);
      colors::subtractp(&Color2, 0x60);
      leftbarvol_col=&colors::dark_yellow;
      rightbarvol_col=&colors::dark_cyan;
    }
    sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp + (i*10), tmpbuf, *c);
    sprintf(tmpbuf,"\x1");
    sdlfont_drawString2c(screen, MEMORY_VIEW_X+520+8, tmp + (i*10), tmpbuf, Color1, Color2);
    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10)+1;
    tmprect.w = left_vol*(screen->w-tmprect.x-20)/255;

    SDL_FillRect(screen, &tmprect, *leftbarvol_col);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+4;
    
    SDL_FillRect(screen, &tmprect, *rightbarvol_col);
  }
}




#undef L_FLAG
#undef R_FLAG

void draw_mouseover_hexdump()
{
  i++;

  tmp += i*10 + 8;
  screen_pos::locked.y = tmp;
  sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp, "  - Mouseover Hexdump -", colors::white);
  if (mouse_hexdump::locked) {
    
    sdlfont_drawString(screen, MEMORY_VIEW_X+520+24*8, tmp, LOCKED_STR, colors::red);
  } else {
    sdlfont_drawString(screen, MEMORY_VIEW_X+520+24*8, tmp, "      ", colors::red);
  }
  
  tmp+=9;
  MOUSE_HEXDUMP_START_Y = tmp;
  if (mouse_hexdump::address>=0)
  {
    
    for (i=0; i<128; i+=8)
    {
      unsigned char *st;
      Uint16 cut_addr = (mouse_hexdump::address + i) & 0xffff;
      
      st = &IAPURAM[cut_addr];
      
      int p = MEMORY_VIEW_X+520;
      
      sprintf(tmpbuf, "%04X: ", cut_addr);
      sdlfont_drawString(screen, p, tmp, tmpbuf, colors::white);
      p += 6*8;

      for (int j=0; j<8; j++) {

        int idx = ((cut_addr+j)&0xff00)<<4; 
        Uint32 color;

        idx += ((cut_addr+j) % 256)<<3;
        color = SDL_MapRGB(screen->format, 
            0x7f + (memsurface.data[idx]>>1), 
            0x7f + (memsurface.data[idx+1]>>1), 
            0x7f + (memsurface.data[idx+2]>>1)
            );
            
        Uint16 cur_addr = cut_addr+j;
        if (cur_addr != mouse_hexdump::addr_being_edited)
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
        else 
        {
          if (cur_addr == 0xf3 )
          {
            if (mouse_hexdump::draw_tmp_ram)
              sprintf(tmpbuf, "%02X ", mouse_hexdump::tmp_ram);
            else sprintf(tmpbuf, "%02X ", player->spc_read_dsp(IAPURAM[0xf2]));
          }
          else if (cur_addr == 0xf0 )
          {
            if (mouse_hexdump::draw_tmp_ram)
              sprintf(tmpbuf, "%02X ", mouse_hexdump::tmp_ram);
            else sprintf(tmpbuf, "%02X ", player->spc_read(cur_addr));
          }
          else if (cur_addr == 0xf1 || (cur_addr >= 0xf4 && cur_addr <= 0xf7))
          {
            if (mouse_hexdump::draw_tmp_ram)
              sprintf(tmpbuf, "%02X ", mouse_hexdump::tmp_ram);
            else sprintf(tmpbuf, "%02X ", *st);
          }
          else sprintf(tmpbuf, "%02X ", *st);
        }


        st++;

        sdlfont_drawString(screen, p, tmp, tmpbuf, color);
        p+= 2*8 + 4;
      }
      
      tmp += 9;
    }

    
  }
}

void draw_porttool()
{
  sdlfont_drawString(screen, PORTTOOL_X, PORTTOOL_Y+8, " APU:", colors::white);
  sdlfont_drawString(screen, PORTTOOL_X, PORTTOOL_Y+16, "SNES:", colors::white);

  if (mode == MODE_EDIT_APU_PORT)
    sprintf(tmpbuf, " -%02X+ -%02X+ -%02X+ -%02X+", porttool::tmp[0], porttool::tmp[1], porttool::tmp[2], porttool::tmp[3]);    
  else 
    sprintf(tmpbuf, " -%02X+ -%02X+ -%02X+ -%02X+", porttool::portdata[0], porttool::portdata[1], porttool::portdata[2], porttool::portdata[3]);  
  
  sdlfont_drawString(screen, PORTTOOL_X + (8*5), PORTTOOL_Y+8, tmpbuf, colors::white);
  
  sprintf(tmpbuf, "  %02X   %02X   %02X   %02X", IAPURAM[0xf4], IAPURAM[0xf5], IAPURAM[0xf6], IAPURAM[0xf7]);   
  sdlfont_drawString(screen, PORTTOOL_X + (8*5), PORTTOOL_Y+16, tmpbuf, colors::white);
}

void draw_time_and_echo_status()
{
  sprintf(tmpbuf, "Time....: %0d:%02d / %0d:%02d", 
      int(player->emu()->tell()/1000)/60,
      int((player->emu()->tell()/1000))%60,
      track::song_time/60, track::song_time%60);
  sdlfont_drawString(screen, INFO_X, INFO_Y+48, tmpbuf, colors::white);


  sprintf(tmpbuf, "Echo....: %s", player->spc_emu()->is_echoing() ? "On " : "Off"); 
  sdlfont_drawString(screen, INFO_X, INFO_Y+56, tmpbuf, colors::white);
}


