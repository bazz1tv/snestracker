#include "Dsp_Window.h"

#define incprint(x) sdlfont_drawString(screen, x,i, tmpbuf, Colors::white); i+=9;

void Dsp_Window::run()
{
  uint i=10,remember_i1, remember_i2, remember_i3;
  int x = 10, remember_x = 10;
  sprintf(tmpbuf,"               General DSP    ");
  incprint(x);
  i+=9;
  // Read all DSP registers
  uint8_t v;
 
  // main vol
  v = player->spc_read_dsp(dsp_reg::mvol_l);
  sprintf(tmpbuf,"Mvol_L: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::mvol_r);
  sprintf(tmpbuf,"Mvol_R: $%02X",v);
  incprint(x)
  //
  i+=9; remember_i1 = i;
  // echo vol
  v = player->spc_read_dsp(dsp_reg::evol_l);
  sprintf(tmpbuf,"Evol_L: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::evol_r);
  sprintf(tmpbuf,"Evol_R: $%02X",v);
  incprint(x)
  //
  i+=9; remember_i2 = i;
  // random
  v = player->spc_read_dsp(dsp_reg::flg);
  sprintf(tmpbuf,"FLG...: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::kon);
  sprintf(tmpbuf,"KON...: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::koff);
  sprintf(tmpbuf,"KOFF..: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::endx);
  sprintf(tmpbuf,"ENDX..: $%02X",v);
  incprint(x)
  //
  remember_i3=i;
  i = remember_i2;
  x += 15*8;
  // echo
  v = player->spc_read_dsp(dsp_reg::eon);
  sprintf(tmpbuf,"EON...: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::esa);
  sprintf(tmpbuf,"ESA...: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::edl);
  sprintf(tmpbuf,"EDL...: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::efb);
  sprintf(tmpbuf,"EFB...: $%02X",v);
  incprint(x)
  //
  i = remember_i1-9;
  x+= 15*8;
  //
  v = player->spc_read_dsp(dsp_reg::c0);
  sprintf(tmpbuf,"C0: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::c1);
  sprintf(tmpbuf,"C1: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::c2);
  sprintf(tmpbuf,"C2: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::c3);
  sprintf(tmpbuf,"C3: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::c4);
  sprintf(tmpbuf,"C4: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::c5);
  sprintf(tmpbuf,"C5: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::c6);
  sprintf(tmpbuf,"C6: $%02X",v);
  incprint(x)
  v = player->spc_read_dsp(dsp_reg::c7);
  sprintf(tmpbuf,"C7: $%02X",v);
  incprint(x)
  //
  x = remember_x;
  remember_i3+= (9*4);
  for (int voice=0; voice < 8; voice++)
  {
    if (voice < 4)
      i = remember_i3;
    else
    {
      if (voice == 4)
        x = remember_x;
      i = remember_i3 + (14*9);
    }
    uint8_t n=0;
    sprintf(tmpbuf," Voice %d",voice);
    incprint(x)
    sprintf(tmpbuf,"---------");
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"vol_L: $%02X",v);
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"vol_R: $%02X",v);
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"p_lo.: $%02X",v);
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"p_hi.: $%02X",v);
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"srcn.: $%02X",v);
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"adsr1: $%02X",v);
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"adsr2: $%02X",v);
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"gain.: $%02X",v);
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"envx.: $%02X",v);
    incprint(x)
    v = player->spc_read_dsp(0x10*voice+n); n++;
    sprintf(tmpbuf,"outx.: $%02X",v);
    incprint(x)

    x += 14*8;
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
      switch (scancode)
      {
        case SDLK_ESCAPE:
          switch_mode(GrandMode::MAIN);
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
          player->spc_write_dsp(dsp_reg::kon,val);
          break;
        }
        case SDLK_m:
        {
          static int val=0;
          val = !val;
          player->spc_write_dsp(dsp_reg::koff,val);
          break;
        }
        case SDLK_d:
          BaseD::switch_mode(GrandMode::MAIN);
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
        /* menu bar */
        if (
          ((ev.button.y >screen->h-12) && (ev.button.y<screen->h)))
        {
          int x = ev.button.x / 8;
          if (x>=1 && x<=4) { printf ("penis5\n"); quitting=true; } // exit
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