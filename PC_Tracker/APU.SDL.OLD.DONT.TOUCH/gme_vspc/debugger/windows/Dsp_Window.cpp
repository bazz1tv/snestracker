#include "Dsp_Window.h"

#define print_then_inc_row(x) sdlfont_drawString(screen, x,i, tmpbuf, Colors::white); i+=CHAR_HEIGHT;
#define print_then_inc_row_voice(x,col) sdlfont_drawString(screen, x,i, tmpbuf, col); i+=CHAR_HEIGHT;
#define inc_row i+=CHAR_HEIGHT;

int mute_solo_voice(void *data)
{
  uintptr_t voicenum_plus_click = (uintptr_t)data;
  bool is_right_click = (voicenum_plus_click & 0x08);
  uintptr_t voicenum = voicenum_plus_click & 0x07;
  //fprintf(stderr, "Voice#: %d,", voicenum);
  if (is_right_click)
  {
    BaseD::voice_control.toggle_solo(voicenum+1);
    //fprintf(stderr, "solo\n");
  }
  else 
  {
    BaseD::voice_control.toggle_mute(voicenum+1);
    //fprintf(stderr, "mute\n");
  }




  // voice control
  return 0;
}


void print_binary(SDL_Surface *screen, int x, int y, uint8_t v, bool use_colors=true)
{
  int tmpx = x+(9*TILE_WIDTH);

  for (int z=7; z >= 0; z--)
  {
    Uint32 *col = use_colors ? &Colors::voice[z] : &Colors::white;
    if (z == 3)
    {
      sdlfont_drawString(screen, tmpx,y, " ", Colors::voice[z]);
      tmpx+=TILE_WIDTH;
    }
    if (v & (1 << z))
      sdlfont_drawString(screen, tmpx,y, "1", *col);
    else
      sdlfont_drawString(screen, tmpx,y, "0", Colors::white);
    tmpx+=TILE_WIDTH;
  }
}

void Dsp_Window::run()
{
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
    //goto reload;
    reload();
  }

  #define GENERAL_DSP_STR "General DSP"
  #define GEN_DSP_ENTRY_STR "Mvol_L: $%02X"

  // log the srcn# for each voice
  uint8_t srcn[MAX_VOICES];
  // non-obvious how the follow is used.. but i dynamically log certain coordinates
  //to help create the layout o_* means original
  uint i=10, o_i = i, remember_i1, remember_i2, remember_i3, remember_i4;
  int x = 10, /*o_x = x,*/ remember_x = 10, remember_x2;
  // start drawing from 10,10

  /* Because we are dynamically generating the layout, we don't write the
  title first because it needs to be centered, after dynamic generation,
  we use the max length of the section and properly center the title header string */

  // pretend we wrote title here
  //sprintf(tmpbuf,GENERAL_DSP_STR);
  //print_then_inc_row(x);
  // inc is a preprocessor definition above to go down a row
  inc_row
  inc_row
  inc_row
  inc_row
  remember_i4=i;
  // Read all DSP registers
  uint8_t v;
 
  // main vol
  v = player->spc_read_dsp(dsp_reg::mvol_l);
  sprintf(tmpbuf,"Mvol_L: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::mvol_r);
  sprintf(tmpbuf,"Mvol_R: $%02X",v);
  print_then_inc_row(x)
  //
  i+=CHAR_HEIGHT; remember_i1 = i;
  // echo vol
  v = player->spc_read_dsp(dsp_reg::evol_l);
  sprintf(tmpbuf,"Evol_L: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::evol_r);
  sprintf(tmpbuf,"Evol_R: $%02X",v);
  print_then_inc_row(x)
  //
  i+=CHAR_HEIGHT; remember_i2 = i;
  // random
  v = player->spc_read_dsp(dsp_reg::esa);
  sprintf(tmpbuf,"ESA...: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::edl);
  sprintf(tmpbuf,"EDL...: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::efb);
  sprintf(tmpbuf,"EFB...: $%02X",v);
  print_then_inc_row(x)
  
  //
  remember_i3=i+TILE_HEIGHT;
  //i = remember_i2-(3*TILE_HEIGHT);
  i = remember_i4;
  x += 15*CHAR_WIDTH;
  v = player->spc_read_dsp(dsp_reg::flg);
  sprintf(tmpbuf,"FLG...: %%");
  sdlfont_drawString(screen, x,i, tmpbuf, Colors::white);
  //print_binary(screen, x,i,v, false);
  int tmpx = x+(9*TILE_WIDTH);
  for (int z=7; z >= 0; z--)
  {
    switch (z)
    {
      case 7:
        sdlfont_drawString(screen, tmpx,i-TILE_HEIGHT, "R", Colors::white);
        break;
      case 6:
        sdlfont_drawString(screen, tmpx,i-TILE_HEIGHT, "M", Colors::white);
        break;
      case 5:
        sdlfont_drawString(screen, tmpx,i-TILE_HEIGHT, "E", Colors::white);
        break;
      case 2:
        sdlfont_drawString(screen, tmpx,i-TILE_HEIGHT, "N", Colors::white);
        break;
      case 4:
      {
        sdlfont_drawString(screen, tmpx,i, " ", Colors::white);
        tmpx+=TILE_WIDTH;
      } break;

      default:break;
    }
    if (v & (1 << z))
      sdlfont_drawString(screen, tmpx,i, "1", Colors::white);
    else
      sdlfont_drawString(screen, tmpx,i, "0", Colors::white);
    tmpx+=TILE_WIDTH;
  }
  inc_row
  i += TILE_HEIGHT;
  v = player->spc_read_dsp(dsp_reg::kon);
  sprintf(tmpbuf,"KON...: %%");
  sdlfont_drawString(screen, x,i, tmpbuf, Colors::white);
  print_binary(screen, x,i,v);
  inc_row
  v = player->spc_read_dsp(dsp_reg::koff);
  sprintf(tmpbuf,"KOFF..: %%");
  sdlfont_drawString(screen, x,i, tmpbuf, Colors::white);
  print_binary(screen, x,i,v);
  inc_row
  //pmon,non
  v = player->spc_read_dsp(dsp_reg::non);
  sprintf(tmpbuf,"NON...: %%");
  sdlfont_drawString(screen, x,i, tmpbuf, Colors::white);
  print_binary(screen, x,i,v);
  inc_row
  v = player->spc_read_dsp(dsp_reg::pmon);
  sprintf(tmpbuf,"PMON..: %%");
  sdlfont_drawString(screen, x,i, tmpbuf, Colors::white);
  print_binary(screen, x,i,v);
  inc_row
  //
  i+=TILE_HEIGHT;
  // echo
  v = player->spc_read_dsp(dsp_reg::eon);
  sprintf(tmpbuf,"EON...: %%");
  sdlfont_drawString(screen, x,i, tmpbuf, Colors::white);
  print_binary(screen, x,i,v);
  inc_row

  v = player->spc_read_dsp(dsp_reg::endx);
  sprintf(tmpbuf,"ENDX..: %%");
  sdlfont_drawString(screen, x,i, tmpbuf, Colors::white);
  print_binary(screen, x,i,v);
  inc_row
  
  //
  i = remember_i1-(CHAR_HEIGHT*3);
  x+= (15+8)*CHAR_WIDTH;
  //
  v = player->spc_read_dsp(dsp_reg::c0);
  sprintf(tmpbuf,"C0: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::c1);
  sprintf(tmpbuf,"C1: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::c2);
  sprintf(tmpbuf,"C2: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::c3);
  sprintf(tmpbuf,"C3: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::c4);
  sprintf(tmpbuf,"C4: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::c5);
  sprintf(tmpbuf,"C5: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::c6);
  sprintf(tmpbuf,"C6: $%02X",v);
  print_then_inc_row(x)
  v = player->spc_read_dsp(dsp_reg::c7);
  sprintf(tmpbuf,"C7: $%02X",v);
  print_then_inc_row(x)
  //
  //remember_x2 = x;

  // now draw title 
  sprintf(tmpbuf, GEN_DSP_ENTRY_STR, i); // i is any variable for this
  int strlen_dsp_entry = strlen(tmpbuf);
  int max_x = x + (strlen_dsp_entry*TILE_WIDTH);
  int whole_length = max_x - remember_x;
  sprintf(tmpbuf,GENERAL_DSP_STR);
  int strlen_dsp_header = strlen(tmpbuf);
  int header_x = (whole_length/2 - (strlen_dsp_header*CHAR_WIDTH)/2) + remember_x;
  sdlfont_drawString(screen, header_x,o_i, tmpbuf, Colors::white);

  //i += (CHAR_HEIGHT*4); use for GEN DSP
  i = remember_i4;
  
  //x = remember_x + ( - (strlen("Voices")*TILE_WIDTH/2) ;
  //sprintf(tmpbuf, "Voices");
  //print_then_inc_row(x)
  inc_row

  x = remember_x;
  remember_i3 = remember_i4 + (15*CHAR_HEIGHT);//+= (CHAR_HEIGHT*5);
  int voice_header_i = remember_i3 - (TILE_HEIGHT*2);
  for (int voice=MAX_VOICES-1; voice >= 0; voice--)
  {
    if (voice >= 4)
      i = remember_i3;
    else
    {
      if (voice == 3)
      {
        remember_x2 = x;
        x = remember_x;
      }
      i = remember_i3 + (14*CHAR_HEIGHT);
    }
    uint8_t n=0;
    sprintf(tmpbuf,"%d",voice);
    // we're going to draw it, but first let's log those coordinates/width/height~
    int voice_label_x = x+(4*TILE_WIDTH);
    if (is_first_run)
    {
      voice_title[voice].str = tmpbuf; // can i have std::string = char * ??
      voice_title[voice].set_rect(voice_label_x, i, strlen(tmpbuf)*TILE_WIDTH, TILE_HEIGHT);
      //fprintf(stderr, "voice label %d: (%d,%d,%d,%d)\n", voice, voice_title[voice].rect.x, voice_title[voice].rect.y,
        //voice_title[voice].rect.w,voice_title[voice].rect.h);
      voice_title[voice].data = (void*)voice;
      voice_title[voice].action = &mute_solo_voice;
    }
    Uint32 *color; 
    //Uint32 darker_color = Colors::subtract(&Colors::voice[voice], 0xc0);
    // this needs to be expanded to cover mute
    if (voice_control.is_muted(voice))
    {
      color = &Colors::nearblack ;
    }
    else 
      color = &Colors::voice[voice];
    print_then_inc_row_voice(voice_label_x, *color) // poor man's center on voices
    inc_row
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"vol_L: $%02X",v);
    print_then_inc_row(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"vol_R: $%02X",v);
    print_then_inc_row(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"p_lo.: $%02X",v);
    print_then_inc_row(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"p_hi.: $%02X",v);
    print_then_inc_row(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    srcn[voice] = v;
    sprintf(tmpbuf,"srcn.: $%02X",v);
    print_then_inc_row(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"adsr1: $%02X",v);
    print_then_inc_row(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"adsr2: $%02X",v);
    print_then_inc_row(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"gain.: $%02X",v);
    print_then_inc_row(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"envx.: $%02X",v);
    print_then_inc_row(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"outx.: $%02X",v);
    print_then_inc_row(x)


    if (voice == 0)
      is_first_run=false;

    x += (strlen(tmpbuf)+3)*CHAR_WIDTH;
  } x -= (strlen(tmpbuf)+3)*CHAR_WIDTH;

  // have max_X at this point
  max_x = strlen_dsp_entry*TILE_WIDTH + x;
  x = remember_x + (max_x/2 - (strlen("Voices")*TILE_WIDTH/2));
  x -= TILE_WIDTH; // cause i didnt like the orig result
  sdlfont_drawString(screen, x,voice_header_i, "Voices", Colors::white);
  
  
  // DIR
  x = remember_x2+(TILE_WIDTH*6);
  v = player->spc_read_dsp(dsp_reg::dir);
  uint16_t dir_ram_addr = v*0x100;
  uint16_t *dir = (uint16_t*)&IAPURAM[dir_ram_addr];
  uint16_t dir_index=0+(dir_offset*2);
  int row_complete=0;
  
  
  i=TILE_HEIGHT*7 + 10;

  #define TEMPLATE_DIR_ENTRY_STR "$%02X: $%04X,$%04X"
  #define TEMPLATE_DIR_STR "DIRECTORY ($%04X)"

  int PIXEL_START_X = (x-TILE_WIDTH)-((MAX_VOICES-1)*5);
  sprintf(tmpbuf, TEMPLATE_DIR_ENTRY_STR, i, i, i);
  int template_dir_entry_strlen = strlen(tmpbuf);
  int next_column_x = (CHAR_WIDTH) * (template_dir_entry_strlen+7);

  sprintf(tmpbuf, TEMPLATE_DIR_STR, i);
  int template_dir_strlen = strlen(tmpbuf);
  //fprintf(stderr, "template_dir_strlen = %d\n", template_dir_strlen);
  // CENTER TEXT
  // GET ENTIRE LENGTH / 2 - strlen(str)/2
  // GET ENTIRE LENGTH
  int entire_length = (x+next_column_x+(template_dir_entry_strlen*TILE_WIDTH)) - PIXEL_START_X;
  //fprintf(stderr, "Entire length = %d", entire_length);
  int center_x  = ((entire_length/2) - ((template_dir_strlen*TILE_WIDTH)/2)) + PIXEL_START_X;

  sprintf(tmpbuf, TEMPLATE_DIR_STR, dir_ram_addr);
  print_then_inc_row(center_x)
  inc_row
  inc_row
  
  int tmp=i;

  assert (((screen->h/CHAR_HEIGHT)-4) > 8);
  uint8_t fakerow=0;
  for (int row=0; fakerow < 8*4*2; row++)
  {
    if (row != 0 && !(fakerow % 8))
    {
      row_complete++;
      if (row_complete==4)
      {
        row_complete=0;
        x+=next_column_x;
        i = tmp;
      }
      else { i+=CHAR_HEIGHT; row++; }
    }

    int voice_iter;
    bool is_voice_dir_entry=false;
    // check if this belongs to a voice right now
    if (fakerow+dir_offset == 0x100)
      dir_index = 0;

    for (voice_iter=0; voice_iter < MAX_VOICES; voice_iter++)
    {
      if ( srcn[voice_iter] == (dir_index/2) )
      {
        put4pixel(screen, (x-TILE_WIDTH)-(voice_iter*5), i+(TILE_HEIGHT/2)-1, Colors::voice[voice_iter]);
        //fprintf(stderr, "voice %d SRCN: %02X, dir_index*2 = %04X")
        is_voice_dir_entry = true;
        //break;
      }
      else  /* ((x-TILE_WIDTH)-((MAX_VOICES-1)*5))+(voice_iter*5) */
        put4pixel(screen, (x-TILE_WIDTH)-(voice_iter*5), i+(TILE_HEIGHT/2)-1, Colors::nearblack);
    }

    //p = dir;
    //dir++;
    
    sprintf(tmpbuf, TEMPLATE_DIR_ENTRY_STR, (uint8_t)(fakerow+dir_offset), dir[dir_index], dir[dir_index+1]);
    if (is_voice_dir_entry)
    {
      // print specific voice color
      sdlfont_drawString(screen, x,i, tmpbuf, Colors::white); //Colors::voice[voice_iter]);
      inc_row
    }
    else 
    {
      sdlfont_drawString(screen, x,i, tmpbuf, Colors::nearblack);
      inc_row
    }
    dir_index+=2;
    fakerow++;
  }

}

void Dsp_Window::draw()
{ 
/*
               General DSP    
mvol_L: $FF                   
mvol_R: $FF 
                              C0: $FF
evol_L: $FF                   C1: $FF
evol_R: $FF                   C2: $FF
                              C3: $FF
flg...: $FF    eon...: $FF    C4: $FF
kon...: $FF    esa...: $FF    C5: $FF
koff..: $FF    edl...: $FF    C6: $FF
endx..: $FF    efb...: $FF    C7: $FF

                              
 Voice 0       Voice 1       Voice 2       Voice 3       
---------     ---------     ---------     ---------       
vol_L: $FF    vol_l: $FF    vol_l: $FF    vol_l: $FF       
vol_R: $FF    vol_r: $FF    vol_r: $FF    vol_r: $FF       
p_lo.: $FF    p_lo.: $FF    p_lo.: $FF    p_lo.: $FF       
p_hi.: $FF    p_hi.: $FF    p_hi.: $FF    p_hi.: $FF       
srcn.: $FF    srcn.: $FF    srcn.: $FF    srcn.: $FF       
adsr1: $FF    adsr1: $FF    adsr1: $FF    adsr1: $FF       
adsr2: $FF    adsr2: $FF    adsr2: $FF    adsr2: $FF       
gain.: $FF    gain.: $FF    gain.: $FF    gain.: $FF       
envx.: $FF    envx.: $FF    envx.: $FF    envx.: $FF       
outx.: $FF    outx.: $FF    outx.: $FF    outx.: $FF       


 Voice 4       Voice 5       Voice 6       Voice 7       
---------     ---------     ---------     ---------       
vol_l: $FF    vol_l: $FF    vol_l: $FF    vol_l: $FF       
vol_r: $FF    vol_r: $FF    vol_r: $FF    vol_r: $FF       
p_lo.: $FF    p_lo.: $FF    p_lo.: $FF    p_lo.: $FF       
p_hi.: $FF    p_hi.: $FF    p_hi.: $FF    p_hi.: $FF       
srcn.: $FF    srcn.: $FF    srcn.: $FF    srcn.: $FF       
adsr1: $FF    adsr1: $FF    adsr1: $FF    adsr1: $FF       
adsr2: $FF    adsr2: $FF    adsr2: $FF    adsr2: $FF       
gain.: $FF    gain.: $FF    gain.: $FF    gain.: $FF       
envx.: $FF    envx.: $FF    envx.: $FF    envx.: $FF       
outx.: $FF    outx.: $FF    outx.: $FF    outx.: $FF        

*/

  // Display all DSP registers
  //sprintf(tmpbuf, "%s")
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Dsp_Window::receive_event(SDL_Event &ev)
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
    case SDL_KEYDOWN:
    {
      int scancode = ev.key.keysym.sym;
      if (ev.key.keysym.mod & (KMOD_SHIFT))
      {
        switch (scancode)
        {
          case SDLK_LEFT:
          dir_offset-=0x20;
          break;
        case SDLK_RIGHT:
          dir_offset+= 0x20;
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

        case SDLK_SPACE: // toggle pause
          paused = !paused;
          player->pause( paused );
          break;

        case SDLK_r:
          restart_current_track();
          break;

        case SDLK_LEFT:
          prev_track();
          break;
        case SDLK_RIGHT:
          next_track();
          break;
        case SDLK_e:
          player->spc_emu()->toggle_echo();
          break;
        case SDLK_n:
        {
          static int val=0;
          val = !val;
          if (val) val = 0xff;
          player->spc_write_dsp(dsp_reg::kon,val);
          break;
        }
        case SDLK_m:
        {
          static int val=0;
          val = !val;
          if (val) val = 0xff;
          player->spc_write_dsp(dsp_reg::koff,val);
          break;
        }
        case SDLK_d:
        case SDLK_SLASH:
          BaseD::switch_mode(GrandMode::MAIN);
          break;

        
        
          break;
        default:
          break;
      }
      

    }
      break;

    case SDL_USEREVENT:
    {
      
    } break;
    case SDL_MOUSEWHEEL:
    {
      
    } break;
    case SDL_MOUSEBUTTONDOWN:           
      {

        for (int i=0; i < MAX_VOICES; i++)
        {
          uintptr_t newdata = (uintptr_t)voice_title[i].data;
          if (ev.button.button == SDL_BUTTON_RIGHT)
            newdata = (uintptr_t)voice_title[i].data | 0x08;
          voice_title[i].check_mouse_and_execute(ev.button.x, ev.button.y, (void*)newdata);
        }

        /* menu bar */
        if (
          ((ev.button.y >screen->h-12) && (ev.button.y<screen->h)))
        {
          int x = ev.button.x / CHAR_WIDTH;
          if (x>=1 && x<=4) { printf ("penis5\n"); quitting=true; } // exit
          if (x>=CHAR_WIDTH && x<=12) { 
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
            //write_mask(packed_mask);
          }

          if (x>=53 && x<=59) { // DSP MAP
            //write_mask(packed_mask);
            //mode = MODE_DSP_MAP;
            switch_mode(GrandMode::MAIN);
          }
        }
      }
      break;
      default:
      break;
  }
}