#include "Instrument_Window.h"
#include "sdlfont.h"
#include "utility.h"
#include "report.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include "Notes.h"
#include "platform.h"

#define ENDLESS_LOOP_OPCODE 0xfe2f

Instrument_Window::Instrument_Window() : 
octave("octave"),
voice("voice")
{

}

void Instrument_Window::run()
{
  report::last_pc = (int)player->spc_emu()->pc(); 

  /* Check if it is time to change tune.
   */   
  if (player->emu()->tell()/1000 >= song_time) 
  {
    if (g_cfg.autowritemask) {
      write_mask(packed_mask);
      if (g_cfg.apply_block) {
        printf("Applying mask on file %s using $%02X as filler\n", g_cfg.playlist[g_cur_entry], g_cfg.filler);
        applyBlockMask(g_cfg.playlist[g_cur_entry]);
      }
    }
    g_cur_entry++;
    if (g_cur_entry>=g_cfg.num_files) { printf ("penis3\n"); quitting=true; return; }
    
    start_stop.is_started = false;
    reload();
  }

  if (is_first_run)
  {
    int x = start_stop.startc.rect.x, o_x=x, y = start_stop.startc.rect.y+(4*TILE_HEIGHT), o_y = y;




    voice.label.rect.x = x;
    voice.label.rect.y = y;

    x+=1*TILE_WIDTH;//center
    y+=TILE_HEIGHT;
    y+=TILE_HEIGHT;
    voice.left_arrow.rect.x = x;
    voice.left_arrow.rect.y = y;
    x += TILE_WIDTH;
    voice.n_x = x;
    voice.n_y = y;
    //y+=TILE_HEIGHT;
    x += TILE_WIDTH;
    voice.right_arrow.rect.x = x;
    voice.right_arrow.rect.y = y;
    
    y=o_y;
    x=o_x;
    x+=voice.label.rect.w + 2*TILE_WIDTH;

    octave.label.rect.x = x;
    octave.label.rect.y = y;
    x+=3*TILE_WIDTH; // center
    y+=TILE_HEIGHT;
    octave.up_arrow.rect.x = x;
    octave.up_arrow.rect.y = y;
    y+=TILE_HEIGHT;
    octave.n_x = x;
    octave.n_y = y;
    y+=TILE_HEIGHT;
    octave.down_arrow.rect.x = x;
    octave.down_arrow.rect.y = y;

    is_first_run = false;
  }
}

void Instrument_Window::draw()
{
  /*if (!start_stop.is_started)
  {
    start_stop.startc.draw(Colors::red);
  }
  else start_stop.stopc.draw(Colors::red);*/
  voice.label.draw(Colors::gray);
  voice.left_arrow.draw(Colors::white, true, false, true);
    sprintf(tmpbuf, "%d", voice.n);
    sdlfont_drawString(screen, voice.n_x, voice.n_y, tmpbuf, Colors::voice[voice.n]);
  voice.right_arrow.draw(Colors::white); // Vflip

  octave.label.draw(Colors::gray);
  octave.up_arrow.draw(Colors::white);
    sprintf(tmpbuf, "%d", octave.n);
    sdlfont_drawString(screen, octave.n_x, octave.n_y, tmpbuf, Colors::magenta);
  octave.down_arrow.draw(Colors::white, true, true); // flipV
  
  sdl_draw();
}

void Instrument_Window::receive_event(SDL_Event &ev)
{
  dblclick::check_event(&ev);
  switch (ev.type)
  {
    
    case SDL_QUIT:
      if (!g_cfg.nosound) {
        SDL_PauseAudio(1);
      }
      printf ("penis4\n");
      quitting = true;
      break;
    case SDL_MOUSEMOTION:
      {
        mouse::x = ev.motion.x; mouse::y = ev.motion.y;
      }
      break;
    case SDL_KEYUP:
      //scancode = 0;
      
      if (scancode == ev.key.keysym.sym) keyoff_current_voice();
      //SDL_Delay(1000);
      
      //fprintf(stderr, "DERP");
    break;
    case SDL_KEYDOWN:
    {
      //if (scancode == ev.key.keysym.sym)
        //break;
      if (ev.key.repeat != 0)
      break;
      scancode = ev.key.keysym.sym;
      switch (scancode)
      {
        case SDLK_SPACE: // toggle pause
          if (start_stop.is_started)
          {
            restore_spc();
            start_stop.is_started = false;
          }
          else player->toggle_pause();
        break;
        default:break;
      }
      if (ev.key.keysym.mod & (CMD_CTRL_KEY))
      {
        //is_shift_pressed=true;
        switch (scancode)
        {
          case SDLK_LEFT:
            start_stop.is_started = false;
            prev_track();

          break;
          case SDLK_RIGHT:
            start_stop.is_started = false;
            next_track();
            break;
          case SDLK_UP:
            inc_voice();
          break;
          case SDLK_DOWN:
            dec_voice();
          break;
          default:break;
        } 
        break;
      }
      
      switch (scancode)
      {
        case SDLK_ESCAPE:
          quitting=true;
        break;
        case SDLK_TAB:
          if (ev.key.keysym.mod & KMOD_SHIFT)
          {
            start_stop.is_started=false;
            prev_track25();
          }
          else 
          {
            start_stop.is_started=false;
            next_track25();
          }
          //goto reload;
          this->reload();
        break;

        case SDLK_LEFT:
          dec_voice();
          break;
        case SDLK_RIGHT:
          inc_voice();
          break;

        case SDLK_z:
          play_pitch(0);
        break;
        case SDLK_s:
          play_pitch(1);
        break;
        case SDLK_x:
          play_pitch(2);
        break;
        case SDLK_d:
          play_pitch(3);
        break;
        case SDLK_c:
          play_pitch(4);
        break;
        case SDLK_v:
          play_pitch(5);
        break;
        case SDLK_g:
          play_pitch(6);
        break;
        case SDLK_b:
          play_pitch(7);
        break;
        case SDLK_h:
          play_pitch(8);
        break;
        case SDLK_n:
          play_pitch(9);
        break;
        case SDLK_j:
          play_pitch(10);
        break;
        case SDLK_m:
          play_pitch(11);
        break;
        case SDLK_COMMA:
        case SDLK_q:
          play_pitch(12);
        break;
        case SDLK_l:
        case SDLK_2:
          play_pitch(13);
        break;
        case SDLK_PERIOD:
        case SDLK_w:
          play_pitch(14);
        break;
        case SDLK_SEMICOLON:
        case SDLK_3:
          play_pitch(15);
        break;
        case SDLK_SLASH:
        case SDLK_e:
          play_pitch(16);
        break;
        case SDLK_r:
          play_pitch(17);
        break;
        case SDLK_5:
          play_pitch(18);
        break;
        case SDLK_t:
          play_pitch(19);
        break;
        case SDLK_6:
          play_pitch(20);
        break;
        case SDLK_y:
          play_pitch(21);
        break;
        case SDLK_7:
          play_pitch(22);
        break;
        case SDLK_u:
          play_pitch(23);
        break;
        case SDLK_i:
          play_pitch(24);
        break;
        case SDLK_9:
          play_pitch(25);
        break;
        case SDLK_o:
          play_pitch(26);
        break;
        case SDLK_0:
          play_pitch(27);
        break;
        case SDLK_p:
          play_pitch(28);
        break;
        case SDLK_LEFTBRACKET:
          play_pitch(29);
        break;
        case SDLK_EQUALS:
          play_pitch(30);
        break;
        case SDLK_RIGHTBRACKET:
          play_pitch(31);
        break;

        //
        case SDLK_UP:
          inc_octave();
        break;
        case SDLK_DOWN:
          dec_octave();
        break;


        default:
          break;
      }
      

    }
      break;

    case SDL_USEREVENT:
    {
      if (ev.user.code == UserEvents::mouse_react)
      {
        SDL_Event *te = (SDL_Event *)ev.user.data1; // the mouse coordinates at time of double click

        
        
      }
      else if (ev.user.code == UserEvents::play_pitch)
      {
        uintptr_t tmp = (uintptr_t)ev.user.data1;
        unsigned char pitch = (unsigned char)tmp;
        play_pitch(pitch - Notes::midi_offset, true);
      }
      else if (ev.user.code == UserEvents::keyoff)
      {
        keyoff_current_voice();
      }
    } break;
    case SDL_MOUSEWHEEL:
    {
      
    } break;
    case SDL_MOUSEBUTTONDOWN:           
    {
      if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &start_stop.startc.rect))
      {
        if (!start_stop.is_started)
        {
          pause_spc();
          start_stop.is_started = true;
        }
        else
        {
          restore_spc();
          start_stop.is_started = false;
        }
      }


      if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &voice.right_arrow.rect))
      {
        inc_voice();
      }
      else if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &voice.left_arrow.rect))
      {
        dec_voice();
      }

      if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &octave.up_arrow.rect))
      {
        inc_octave();
      }
      else if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &octave.down_arrow.rect))
      {
        dec_octave();
      }

      if (
      ((ev.button.y >screen->h-12) && (ev.button.y<screen->h)))
      {
        int x = ev.button.x / CHAR_WIDTH;
        if (x>=1 && x<=4) { printf ("penis5\n"); quitting=true; } // exit
        if (x>=CHAR_WIDTH && x<=12) { 
          toggle_pause();
        } // pause

        if (x>=16 && x<=22) {  // restart
          restart_current_track();
        }

        if (x>=26 && x<=29) {  // prev
          SDL_PauseAudio(1);
          prev_track();
        }

        if (x>=33 && x<=36) { // next
          next_track();
        }

        if (x>=41 && x<=50) { // write mask
          //write_mask(packed_mask);
        }

        if (x>=53 && x<=54) { // Main
          //write_mask(packed_mask);
          //mode = MODE_DSP_MAP;
          restore_spc(false);
          switch_mode(GrandMode::MAIN);
        }
        if (x>=58 && x<=59) { // DSP MAP
          //write_mask(packed_mask);
          //mode = MODE_DSP_MAP;
          restore_spc(false);
          switch_mode(GrandMode::DSP_MAP);
        }
        if (x>=63 && x<=67) { // Instr
          //write_mask(packed_mask);
          //mode = MODE_DSP_MAP;
          //switch_mode(GrandMode::INSTRUMENT);
        }
      }
    }
    break;
    
    default:
    break;
  }
}

void Instrument_Window::set_voice(unsigned char v)
{
  voice.n = v % 8;
}

void Instrument_Window::inc_voice()
{
  if (voice.n == 7)
          voice.n = 0;
        else voice.n++;
}
void Instrument_Window::dec_voice()
{
  if (voice.n == 0)
          voice.n = 7;
        else voice.n--;
}

void Instrument_Window::inc_octave()
{
  if (octave.n == 6)
          octave.n = 0;
        else octave.n++;
}

void Instrument_Window::dec_octave()
{
  if (octave.n == 0)
          octave.n = 6;
        else octave.n--;
}

void Instrument_Window::play_pitch(int p, bool abs/*=false*/)
{
  if (!start_stop.is_started)
  {
    pause_spc();
  }
  player->spc_write_dsp(dsp_reg::koff, 0);
  uint8_t hi,lo;
  uint16_t index = (abs ? 0:12*octave.n) + 0x18 + 1 + p;
  if (index > Notes::pitch_table_size-1)
    return;
  hi = Notes::pitch_table[index] >> 8;
  lo = Notes::pitch_table[index] & 0xff;
  //fprintf(stderr, "p = 0x%02X%02X\n", hi, lo);
  player->spc_write_dsp(dsp_reg::plo + (voice.n*0x10), lo);
  player->spc_write_dsp(dsp_reg::phi + (voice.n*0x10), hi);
  player->spc_write_dsp(dsp_reg::kon, 1 << voice.n);
}

void Instrument_Window::pause_spc()
{
  player->pause(1);
  // backup_pc()
  pc_ptr = (uint16_t*)&IAPURAM[report::last_pc];
  pc_backup = *pc_ptr;

  song_time_backup = BaseD::song_time;
  BaseD::song_time = 10000;
  //player->ignore_silence();
  player->emu()->set_fade(1000*10000, 8000);


  // write with never-ending loop
  
  // overwrite_pc_with_endless_loop()
  *pc_ptr = ENDLESS_LOOP_OPCODE;

  player->pause(0);

  player->spc_write_dsp(dsp_reg::koff, 0xff);
  SDL_Delay(100);
  player->spc_write_dsp(dsp_reg::koff, 0x00);
  start_stop.is_started=true;
}

void Instrument_Window::restore_spc(bool resume/*=true*/)
{
  // restore_pc()
  //uint16_t *pc_ptr = (uint16_t*)&IAPURAM[report::last_pc];
  if (!pc_ptr) return;
  player->pause(1);
  *pc_ptr = pc_backup;
  if (resume) player->pause(0);
  pc_ptr = NULL;
  start_stop.is_started=false;
  //BaseD::song_time = song_time_backup;
  //track_info_backup = track_info_backup2;
}

void Instrument_Window::keyoff_current_voice()
{
  player->spc_write_dsp(dsp_reg::koff, 1 << voice.n);
}