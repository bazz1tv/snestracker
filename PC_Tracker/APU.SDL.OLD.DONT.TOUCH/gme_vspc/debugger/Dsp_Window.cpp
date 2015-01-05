#include "Dsp_Window.h"

void Dsp_Window::run()
{
  // Read all DSP registers
  for (int i=0; i < dsp_reg::how_many; i++)
  {
    vals[i] = player->spc_read_dsp(dsp_reg::lut[i].addr);
  }
}

void Dsp_Window::draw()
{
  sprintf(tmpbuf,"               General DSP    ");
  sdlfont_drawString(screen, 10,10, tmpbuf, Colors::white);

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
      if (scancode == SDLK_ESCAPE)
      {
        change_grand_mode(GrandMode::MAIN);
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
        
      }
      break;
      default:
      break;
  }
}