#include "Dsp_Window.h"
#include "utility.h"
#include "platform.h"
#include "Menu_Bar.h"
#include "shared/Voice_Control.h"

#define DIR_ENTRIES_PER_COLUMN 0x20

// These were made static only so that the DIR entries right-click
// callback could access it while using its datafield to keep track of the
// index
uint16_t *Dsp_Window::dir;
uint16_t Dsp_Window::dir_index;
uint8_t Dsp_Window::dir_offset = 0;

#define print_then_inc_row(x) sdlfont_drawString(screen, x,i, tmpbuf, Colors::white); i+=CHAR_HEIGHT;
#define print_then_inc_row_voice(x,col) sdlfont_drawString(screen, x,i, tmpbuf, col); i+=CHAR_HEIGHT;
#define inc_row i+=CHAR_HEIGHT;

static int get_center_x(int remember_x, int x, const char *newstr)
{
  int max_x = x; //strlen(newstr)*TILE_WIDTH + x;
  x = remember_x + (max_x/2 - (strlen(newstr)*TILE_WIDTH/2));
  x -= TILE_WIDTH; // cause i didnt like the orig result
  return x;
}
  

static Uint8* get_brr_end_block(Uint8 *brr_sample)
{
  while (!(*brr_sample & 1))
    brr_sample+=9;
  return brr_sample;
}

int Dsp_Window::Loop_Clickable::toggle_loop(void *index)
{
  Uint8 *iindex = (Uint8*)index;
  Uint8 *brr_sample = &IAPURAM[dir[dir_index + (*iindex * 2)]];
  brr_sample = get_brr_end_block(brr_sample);
  //while ()
  DEBUGLOG("brr_addr = %04X\n", dir[dir_index + (*iindex * 2)]);
  // find the end brr block
  
  if (*brr_sample & 2)
  {
    *brr_sample &= ~2;
  }
  else
  {
    *brr_sample |= 2;
  }
  return 0;
}

Dsp_Window::Dsp_Window() : 
screw_clickable("Screw Around", toggle_screw, this, Colors::nearblack)
{
  clear_used_srcn();
}

int Dsp_Window::toggle_screw(void *dsp_win)
{
  Dsp_Window *dsp_window = (Dsp_Window*)dsp_win;
  //*is_screwing = !*is_screwing;
  dsp_window->is_screwing = !dsp_window->is_screwing;
  if (dsp_window->is_screwing)
  {
    BaseD::Hack_Spc::pause_spc();
  }
  else
  {
    BaseD::Hack_Spc::restore_spc();
  }
  return 0;
}

void Dsp_Window::reset_screw()
{
  is_screwing = false;
  BaseD::Hack_Spc::restore_spc();
}

void print_binary(SDL_Surface *screen, int x, int y, uint8_t v, bool use_colors=true)
{
  int tmpx = x+(9*TILE_WIDTH);

  for (int z=7; z >= 0; z--)
  {
    Uint32 *col;
    if (use_colors)
    {
      if (::voice_control.is_muted(z))
        col = &Colors::nearblack;
      else 
        col = &Colors::voice[z];
    }
    else col = &Colors::white;
    if (z == 3)
    {
      sdlfont_drawString(screen, tmpx,y, " ", Colors::voice[z]);
      tmpx+=TILE_WIDTH;
    }
    if (v & (1 << z))
      sdlfont_drawString(screen, tmpx,y, "1", *col);
    else
      sdlfont_drawString(screen, tmpx,y, "0", *col);
    tmpx+=TILE_WIDTH;
  }
}

void Dsp_Window::clear_used_srcn()
{
  for (int i=0; i < MAX_SRCN_ENTRIES/8; i++)
  {
    used_srcn[i]=0;
    
  }
  for (int i=0; i < MAX_SRCN_ENTRIES; i++)
  {
    used_srcn_voice[i]=0;
  }
}


// helper function for the run() first run init
void Dsp_Window::init_voice_clickable(char *str, int &x, int &i)
{
  static int loindex=0, hiindex=MAX_VOICES-1;
  assert (loindex < SIZEOF_VOICE_ENUM && hiindex < MAX_VOICES);
  int tmp = (strlen(tmpbuf)-2)*CHAR_WIDTH;
  // here's the first comment example: 
  // calculate how many characters until the variable part?
  // init the x,y,
  clickable_voice[hiindex][loindex].rect.x = x + tmp;
  clickable_voice[hiindex][loindex].rect.y = i;
  //w,h
  clickable_voice[hiindex][loindex].rect.w = CHAR_WIDTH*2;
  clickable_voice[hiindex][loindex].rect.h = CHAR_HEIGHT;
  loindex++;
  if (loindex == SIZEOF_VOICE_ENUM)
  {
    hiindex--;
    loindex=0;
  }
}

void Dsp_Window::enter_edit_mode()
{
  mode = MODE_EDIT_ADDR;
  cursor.start_timer();
}

void Dsp_Window::exit_edit_mode()
{
  mode = MODE_NAV;
  submode = NONE;
  cursor.stop_timer();
  tmp_ram = 0;
}

// The clickable text are dynamically calculated from the string lengths
  // it's kind of unconnected although the strings are right above for referencing...
  // the first one mainvol_l I will comment as an example: 
void Dsp_Window::init_gen_dsp_clickable(char *str, int &x, int &i)
{
  static int index=0;
  assert (index < SIZEOF_GEN_DSP_ENUM);
  int tmp = (strlen(tmpbuf)-2)*CHAR_WIDTH;
  // here's the first comment example: 
  // calculate how many characters until the variable part?
  // init the x,y,
  clickable_gen_dsp[index].rect.x = x + tmp;
  clickable_gen_dsp[index].rect.y = i;
  //w,h
  clickable_gen_dsp[index].rect.w = CHAR_WIDTH*2;
  clickable_gen_dsp[index].rect.h = CHAR_HEIGHT;
  index++;
}

void Dsp_Window::run()
{
  SDL_Surface *screen = ::render->screen;
  BaseD::check_time();

  #define GENERAL_DSP_STR "General DSP"
  #define GEN_DSP_ENTRY_STR "Mvol_L: $%02X"

  int gen_dsp_index=0;
  uint8_t *valp;
  // log the srcn# for each voice
  uint8_t srcn[MAX_VOICES];
  // non-obvious how the follow is used.. but i dynamically log certain coordinates
  //to help create the layout o_* means original
  int i = BaseD::menu_bar->tabs.rect.y + BaseD::menu_bar->tabs.rect.h + TILE_HEIGHT*5, o_i = i, remember_i1, remember_i2, remember_i3, remember_i4;
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

  for (; gen_dsp_index <= efb; gen_dsp_index++)
  {
    // main vol
    v = player->spc_read_dsp(gen_dsp_map[gen_dsp_index].addr); 
    if (mode == MODE_EDIT_ADDR && gen_dsp_map[gen_dsp_index].addr == current_edit_addr)
    {
      valp = &tmp_ram;
      tmp_ram = v;
    }
    else valp = &v; 
    sprintf(tmpbuf,gen_dsp_map[gen_dsp_index].format_str,*valp);
    if (is_first_run)
      init_gen_dsp_clickable(tmpbuf,x,i);
    // be careful to seperate the bit editing mode from normal byte editing mode
    print_then_inc_row(x)

    if (gen_dsp_index == 1)
    {
      i+=CHAR_HEIGHT; remember_i1 = i;
    }
    else if (gen_dsp_index == 3)
    {
      i+=CHAR_HEIGHT; remember_i2 = i;
    }
  }
  remember_i3=i+TILE_HEIGHT;

  i = remember_i4;
  x += 15*CHAR_WIDTH;
  v = player->spc_read_dsp(dsp_reg::flg);
  sprintf(tmpbuf,"FLG...: %%");
  int tmpx =  x + strlen(tmpbuf)*CHAR_WIDTH;

  if (is_first_run)
  {
    // init_flg()
    for (int b=7; b >=0; b--)
    {
      if (b == 4)
        tmpx += CHAR_WIDTH;
      byte[flg].bits[b].x = tmpx;
      byte[flg].bits[b].y = i;
      byte[flg].bits[b].w = CHAR_WIDTH;
      byte[flg].bits[b].h = CHAR_HEIGHT;
      tmpx+=CHAR_WIDTH;
    }
  }

  sdlfont_drawString(screen, x,i, tmpbuf, Colors::white);
  //print_binary(screen, x,i,v, false);
  tmpx = x+(9*TILE_WIDTH);
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
  

  for (int byte_index = kon; byte_index < SIZEOF_8BIT_GEN_DSP_ENUM; byte_index++)
  {
    if (byte_index == eon)
    {
      inc_row
    }
    v = player->spc_read_dsp(gen_8bit_dsp_map[byte_index].addr);
    sprintf(tmpbuf,gen_8bit_dsp_map[byte_index].format_str);

    if (is_first_run)
    {
      // init_flg()
      tmpx =  x + strlen(tmpbuf)*CHAR_WIDTH;
      for (int b=7; b >=0; b--)
      {
        if (b == 3)
        {
          tmpx += CHAR_WIDTH;
        }
        byte[byte_index].bits[b].x = tmpx;
        byte[byte_index].bits[b].y = i;
        byte[byte_index].bits[b].w = CHAR_WIDTH;
        byte[byte_index].bits[b].h = CHAR_HEIGHT;
        tmpx+=CHAR_WIDTH;
      }
    }
    sdlfont_drawString(screen, x,i, tmpbuf, Colors::white);
    print_binary(screen, x,i,v);
    inc_row
  }
  

  
  
  
  
  
  //
  i = remember_i1-(CHAR_HEIGHT*3);
  x+= (15+8)*CHAR_WIDTH;
  //
  for (; gen_dsp_index <= c7; gen_dsp_index++)
  {
    // main vol
    v = player->spc_read_dsp(gen_dsp_map[gen_dsp_index].addr); 
    if (mode == MODE_EDIT_ADDR && gen_dsp_map[gen_dsp_index].addr == current_edit_addr)
    {
      valp = &tmp_ram;
      tmp_ram = v;
    }
    else valp = &v; 
    sprintf(tmpbuf,gen_dsp_map[gen_dsp_index].format_str,*valp);
    if (is_first_run)
      init_gen_dsp_clickable(tmpbuf,x,i);
    // be careful to seperate the bit editing mode from normal byte editing mode
    print_then_inc_row(x)
  }
  
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

  if (is_first_run)
  {
    screw_clickable.setup(header_x+(strlen(GENERAL_DSP_STR)+4)*CHAR_WIDTH, o_i);
  }
  if (is_screwing)
    screw_clickable.color = Colors::Interface::color[Colors::Interface::text_fg];
  else screw_clickable.color = Colors::nearblack;
  screw_clickable.draw();

  //i += (CHAR_HEIGHT*4); use for GEN DSP
  //i = remember_i4;
  
  //x = remember_x + ( - (strlen("Voices")*TILE_WIDTH/2) ;
  //sprintf(tmpbuf, "Voices");
  //print_then_inc_row(x)
  inc_row
  inc_row
  inc_row
  inc_row


  //
  
  //int timer_header_i = i; //- CHAR_HEIGHT*2;
  
  //int remember_timer_i = i;
  x = remember_x;
  for (int timer=0; timer < NUM_TIMERS; timer++)
  {
    bool timer_is_active=false;
    //i = remember_timer_i;
    sprintf(tmpbuf,"%d",timer);
    //int label_x = x+4*CHAR_WIDTH;

    Uint32 color;

    Uint8 spc_control = IAPURAM[0xf1]; //player->spc_read(0xf1);
    //DEBUGLOG("spc_control = %02X\n", spc_control);
    if (spc_control & (1<<timer))
      timer_is_active=true;

    if (timer_is_active)
    {
      color = Colors::white;
      timers.label[timer].color = Colors::white;
    }
    else 
    {
      color = Colors::nearblack;
      timers.label[timer].color = Colors::nearblack;
    }

    //print_then_inc_row_voice(label_x, color) // poor man's center on voices
    //inc_row

    if (is_first_run)
    {
      int center_x = get_center_x(remember_x, max_x, "Timer X Ticks: $00");
      timers.label[timer].rect.x = center_x;
      timers.label[timer].rect.y = i;
    }
    timers.label[timer].draw();
    // print timer ticks
    Uint8 ticks = player->spc_read(0xfa+timer);
    //DEBUGLOG("ticks = %02X\n", ticks);
    //if (ticks == 0) ticks = 256;
    sprintf(tmpbuf, "Ticks: $%02X", ticks);
    int tick_x = timers.label[timer].rect.x + timers.label[timer].rect.w + CHAR_WIDTH;
    
    if (is_first_run)
    {
      timers.value[timer].rect.x = tick_x + ((strlen(tmpbuf)-2)*CHAR_WIDTH);
      timers.value[timer].rect.y = i;
    }
    print_then_inc_row_voice(tick_x, color)
    //timers.value[timer].draw();
    // print timer count
    //Uint8 count = player->spc_read(0xfd+timer); //player->spc_read(0xfa+timer);
    //if (ticks == 0) ticks = 256;
    //sprintf(tmpbuf, "Count: %d", count);
    //print_then_inc_row_voice(x, color)

    //x += (strlen(tmpbuf)+5+3+2)*CHAR_WIDTH;
  } //x -= (strlen(tmpbuf)+5+3+2)*CHAR_WIDTH;
  //sdlfont_drawString(screen, center_x(remember_x, x, "Timers", GENERAL_DSP_STR),timer_header_i, "Timers", Colors::white);
  inc_row
  inc_row
  inc_row
  inc_row


  x = remember_x;
  remember_i3 = i;//+= (CHAR_HEIGHT*5);
  int voice_header_i = remember_i3 - CHAR_HEIGHT*2; // - (TILE_HEIGHT*2);
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
      voice_title[voice].action = &Voice_Control::mute_solo_voice;
    }
    Uint32 *color = voice_control.is_muted(voice) ? &Colors::nearblack : &Colors::voice[voice];
    //Uint32 darker_color = Colors::subtract(&Colors::voice[voice], 0xc0);
    // this needs to be expanded to cover mute
    
    print_then_inc_row_voice(voice_label_x, *color) // poor man's center on voices
    inc_row

    for (uint8_t n=0; n < SIZEOF_VOICE_ENUM; n++)
    {
      v = player->spc_read_dsp(0x10*voice+n);
      if (n == dsp_reg::srcn)
      {
        srcn[voice] = v;
        uint8_t byte = v/8, bit = v % 8;
        used_srcn[byte] |= (1 << bit);
      }
      sprintf(tmpbuf,voice_map[n],v);
      if (is_first_run)
        init_voice_clickable(tmpbuf,x,i);
      print_then_inc_row(x)
    }
      

    x += (strlen(tmpbuf)+3)*CHAR_WIDTH;
  } x -= (3)*CHAR_WIDTH;



  // have max_X at this point
  sdlfont_drawString(screen, get_center_x(remember_x, x, "Voices"),voice_header_i, "Voices", Colors::white);
  
  
  // DIR
  x = remember_x2+(TILE_WIDTH*6);
  v = player->spc_read_dsp(dsp_reg::dir);
  dir_ram_addr = v*0x100;
  dir = (uint16_t*)&IAPURAM[dir_ram_addr];
  dir_index=0+(dir_offset*2);
  int row_complete=0;
  
  
  i=TILE_HEIGHT*7 + o_i;

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
  if (is_first_run)
  {
    dir_rect.x = center_x + (template_dir_strlen-5)*CHAR_WIDTH;
    dir_rect.y = i;
    dir_rect.w = 4*TILE_WIDTH;
    dir_rect.h = TILE_HEIGHT;
  }

  sprintf(tmpbuf, TEMPLATE_DIR_STR, dir_ram_addr);
  print_then_inc_row(center_x)
  inc_row
  inc_row
  
  int tmp=i;

  assert (((screen->h/CHAR_HEIGHT)-4) > 8);
  uint8_t fakerow=0;
  for (int row=0; fakerow < NUM_DIR_ENTRIES_DISPLAYED; row++)
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

    /* The only time the below is 0x100 is when the right-side rows
     * reference 0x00. But the calculations below continue to offset from
     * fakerow, so we need to negatively account for that to get to 0x00
     * again.*/
    if (fakerow+dir_offset == 0x100)
      dir_index = 0 - (fakerow*2);

    uint16_t cur_dir_index = dir_index + (fakerow*2);

    for (voice_iter=0; voice_iter < MAX_VOICES; voice_iter++)
    {
      if ( srcn[voice_iter] == (cur_dir_index/2) )
      {
        used_srcn_voice[cur_dir_index/2] |= 1 << voice_iter;
        put4pixel(screen, (x-TILE_WIDTH)-(voice_iter*5), i+(TILE_HEIGHT/2)-1, Colors::voice[voice_iter]);
        //fprintf(stderr, "voice %d SRCN: %02X, cur_dir_index*2 = %04X")
        is_voice_dir_entry = true;
        //break;
      }
      else if (used_srcn_voice[cur_dir_index/2] & (1 << voice_iter))
      {
        put4pixel(screen, (x-TILE_WIDTH)-(voice_iter*5), i+(TILE_HEIGHT/2)-1, 
          Colors::gray);
      }
      else  /* ((x-TILE_WIDTH)-((MAX_VOICES-1)*5))+(voice_iter*5) */
        put4pixel(screen, (x-TILE_WIDTH)-(voice_iter*5), i+(TILE_HEIGHT/2)-1, Colors::nearblack);
    }
    
    sprintf(tmpbuf, TEMPLATE_DIR_ENTRY_STR, (uint8_t)(fakerow+dir_offset), dir[cur_dir_index], dir[cur_dir_index+1]);
    if (is_first_run)
    {
      int xx = x + strlen(tmpbuf)*CHAR_WIDTH;

			// init clickable dir-rects for context menu download
			Clickable_Rect *cr = &dir_rects[fakerow];
			SDL_Rect *r = &cr->rect;
			r->x = x;
			r->y = i;
			r->w = (sizeof(TEMPLATE_DIR_ENTRY_STR) - 1) * CHAR_WIDTH;
			r->h = CHAR_HEIGHT;
			cr->action = dir_rect_clicked;
			cr->data = (void*)fakerow;

      loop_clickable[fakerow].index = fakerow;
      loop_clickable[fakerow].clickable_text.rect.x = xx;
      loop_clickable[fakerow].clickable_text.rect.y = i;
      //sprintf(tmpbuf, "L");
      //sdlfont_drawString(screen, xx,i, tmpbuf, Colors::white);
    }
    if (is_voice_dir_entry)
    {
      // print specific voice color
      sdlfont_drawString(screen, x,i, tmpbuf, Colors::white); //Colors::voice[voice_iter]);
      do_loop_point_color(fakerow, &IAPURAM[dir[cur_dir_index]], Colors::white, Colors::nearblack);
      inc_row

    }
    else 
    {
      if (is_srcn_used(fakerow + dir_offset))
      {
        sdlfont_drawString(screen, x,i, tmpbuf, Colors::gray);
        do_loop_point_color(fakerow, &IAPURAM[dir[cur_dir_index]], Colors::gray, Colors::nearblack); 
      }
      else
      {
        sdlfont_drawString(screen, x,i, tmpbuf, Colors::nearblack);
        loop_clickable[fakerow].clickable_text.color = Colors::nearblack;
        //do_loop_point_color(fakerow, &IAPURAM[dir[cur_dir_index]], Colors::nearblack, Colors::nearblack); 
      }

      inc_row
    }
    
    loop_clickable[fakerow].clickable_text.draw();

    //dir_index_offset+=2;
    fakerow++;
  }
  is_first_run=false;
}

void Dsp_Window::do_loop_point_color(int index, Uint8* brr_sample, Uint32 active_color, Uint32 inactive_color)
{
  //Uint8 *brr_sample = &IAPURAM[dir[cur_dir_index]];
  brr_sample = get_brr_end_block(brr_sample);
  if (*brr_sample & 2)
  {
    loop_clickable[index].clickable_text.color = active_color;      
  }
  else
  {
    loop_clickable[index].clickable_text.color = inactive_color;      
  }
}

bool Dsp_Window::is_srcn_used(Uint8 dirnum)
{
  //uint8_t dirnum = fakerow;
  uint8_t byte = dirnum / 8, bit = dirnum % 8;
  if (used_srcn[byte] & (1<<bit))
    return true;
  return false;
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
  BaseD::draw_menu_bar();

  if (mode == MODE_EDIT_ADDR)
  {
    cursor.draw(::render->screen, Colors::green);
  }

	if (::brrcontext.menu.is_active)
		::brrcontext.menu.draw(::render->screen);

  // Display all DSP registers
  //sprintf(tmpbuf, "%s")
  SDL_UpdateTexture(::render->sdlTexture, NULL, ::render->screen->pixels,
                                      ::render->screen->pitch);
  SDL_RenderClear(::render->sdlRenderer);
  SDL_RenderCopy(::render->sdlRenderer, ::render->sdlTexture, NULL, NULL);
}

int Dsp_Window::receive_event(SDL_Event &ev)
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

  dblclick::check_event(&ev);
  switch (ev.type)
  {
    case SDL_QUIT:
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
      //bool is_shift_pressed=false;
      switch (scancode)
      {
        case SDLK_TAB:
          if (ev.key.keysym.mod & KMOD_SHIFT)
          {
            clear_used_srcn();
            reset_screw();
            prev_track25();
          }
          else 
          {
            clear_used_srcn();
            reset_screw();
            next_track25();
          }
          //goto reload;
          this->reload();
        break;
        case SDLK_SPACE: // toggle pause
          player->toggle_pause();
        break;
        case SDLK_SLASH:
          if (mode == MODE_EDIT_ADDR)
            exit_edit_mode();
          BaseD::switch_mode(GrandMode::MAIN);
          break;
        default:break;
      }
      if (ev.key.keysym.mod & (KMOD_SHIFT) && mode != MODE_EDIT_ADDR)
      {
        //is_shift_pressed=true;
        switch (scancode)
        {
          case SDLK_LEFT:
          dir_offset -= DIR_ENTRIES_PER_COLUMN;
          break;
        case SDLK_RIGHT:
          dir_offset += DIR_ENTRIES_PER_COLUMN;
          break;
          default:break;
        } 
        break;
      }
      if (mode == MODE_EDIT_ADDR)
      {
        // COMMON THINGS
        switch (scancode)
        {
          // prolly aint gonna put anything here hehhe
        }
        if (submode != EDIT_GEN_DSP_8BIT_ADDR)
        {
          if ( ((scancode >= '0') && (scancode <= '9')) || ((scancode >= 'A') && (scancode <= 'F')) || 
            ((scancode >= 'a') && (scancode <= 'f')) )
          {
            uint32_t i=0;
            Uint16 addr = current_edit_addr;
            
            i = Utility::scancode_to_hex(scancode);
            fprintf(stderr, "i = %d", i);
            if (highnibble)
            {
              i <<= 4;
              i &= 0xf0;
              tmp_ram &= 0x0f;
            }
            else
            {
              i &= 0x0f;
              tmp_ram &= 0xf0;
            }
            tmp_ram |= i;

            if (submode == EDIT_TIMER)
              player->spc_write(addr, tmp_ram);
            else player->spc_write_dsp(addr, tmp_ram);
            
            //if (mouseover_hexdump_area.horizontal) mouseover_hexdump_area.inc_cursor_pos();
            
          }
          switch (scancode)
          {
            case SDLK_LEFT:
              if (highnibble) //= !highnibble;
              {
                cursor.rect.x += CHAR_WIDTH;
              }
              else cursor.rect.x -= CHAR_WIDTH;
              highnibble = !highnibble;

            break;
            case SDLK_RIGHT:
              if (highnibble) //= !highnibble;
              {
                cursor.rect.x += CHAR_WIDTH;
              }
              else cursor.rect.x -= CHAR_WIDTH;
              highnibble = !highnibble;
            break;
            case SDLK_UP:
              if (submode == EDIT_TIMER)
              {
                if (selected_index == 0)
                  selected_index = 2;
                else selected_index--;

                timers.num = selected_index;
                current_edit_addr = 0xfa + selected_index;
                tmp_ram = player->spc_read(current_edit_addr);
                cursor.rect.x = timers.value[selected_index].rect.x + (highnibble ? 0:CHAR_WIDTH);
                cursor.rect.y = timers.value[selected_index].rect.y;
              }
              else if (submode == EDIT_GEN_DSP_ADDR)
              {
                if (selected_index == 0)
                  selected_index = (SIZEOF_GEN_DSP_ENUM-1);
                else selected_index--;
                current_edit_addr = gen_dsp_map[selected_index].addr;
                tmp_ram = player->spc_read_dsp(current_edit_addr);
                cursor.rect.x = clickable_gen_dsp[selected_index].rect.x + (highnibble ? 0:CHAR_WIDTH);
                cursor.rect.y = clickable_gen_dsp[selected_index].rect.y;
              }
              else if (submode == EDIT_VOICE_ADDR)
              {
                if (selected_sub_index == 0)
                  selected_sub_index = (SIZEOF_VOICE_ENUM-1);
                else selected_sub_index--;
                cursor.rect.x = clickable_voice[selected_index][selected_sub_index].rect.x + (highnibble ? 0:CHAR_WIDTH);
                cursor.rect.y = clickable_voice[selected_index][selected_sub_index].rect.y;
                current_edit_addr = (selected_index * 0x10) + selected_sub_index;
                tmp_ram = player->spc_read_dsp(current_edit_addr);
              }
            break;
            case SDLK_DOWN:
              if (submode == EDIT_TIMER)
              {
                if (selected_index == 2)
                  selected_index = 0;
                else selected_index++;

                timers.num = selected_index;
                current_edit_addr = 0xfa + selected_index;
                tmp_ram = player->spc_read(current_edit_addr);
                cursor.rect.x = timers.value[selected_index].rect.x + (highnibble ? 0:CHAR_WIDTH);
                cursor.rect.y = timers.value[selected_index].rect.y;
              }
              else if (submode == EDIT_GEN_DSP_ADDR)
              {
                if (selected_index == (SIZEOF_GEN_DSP_ENUM-1))
                  selected_index = 0;
                else selected_index++;
                current_edit_addr = gen_dsp_map[selected_index].addr;
                tmp_ram = player->spc_read_dsp(current_edit_addr);
                cursor.rect.x = clickable_gen_dsp[selected_index].rect.x + (highnibble ? 0:CHAR_WIDTH);
                cursor.rect.y = clickable_gen_dsp[selected_index].rect.y;
              }
              else if (submode == EDIT_VOICE_ADDR)
              {
                if (selected_sub_index == (SIZEOF_VOICE_ENUM-1))
                  selected_sub_index = 0;
                else selected_sub_index++;
                cursor.rect.x = clickable_voice[selected_index][selected_sub_index].rect.x + (highnibble ? 0:CHAR_WIDTH);
                cursor.rect.y = clickable_voice[selected_index][selected_sub_index].rect.y;
                current_edit_addr = (selected_index * 0x10) + selected_sub_index;
                tmp_ram = player->spc_read_dsp(current_edit_addr);
              }
            break;

            case SDLK_RETURN:
            case SDLK_ESCAPE:
              exit_edit_mode();
            default:break;
          }
        }
        else
        {
          // shit we want regardless of the CMD/CTRL key: 
          switch (scancode)
          {
            case SDLK_LEFT:
              fprintf(stderr, "left");
              if (selected_bit == 7) //= !highnibble;
              {
                selected_bit = 0;
              }
              else
              {
                selected_bit++;
              }
              cursor.rect.x = byte[selected_index].bits[selected_bit].x;
            break;
            case SDLK_RIGHT:
              fprintf(stderr, "right");
              if (selected_bit == 0) //= !highnibble;
              {
                selected_bit = 7;
              }
              else
              {
                selected_bit--;
              }
              cursor.rect.x = byte[selected_index].bits[selected_bit].x;
            break;
            default:break;
          }
          if (ev.key.keysym.mod & (CMD_CTRL_KEY))
          {
            fprintf(stderr, "WOOT");
            switch (scancode)
            {
              case SDLK_DOWN:
              case SDLK_UP:
                //fprintf(stderr, "tmpram before = 0x%02x, sb = %d\n", tmp_ram, selected_bit);
                tmp_ram ^= (1 << selected_bit);
                //fprintf(stderr, "tmpram after = 0x%02x, sb = %d", tmp_ram, selected_bit);
                player->spc_write_dsp(gen_8bit_dsp_map[selected_index].addr,tmp_ram);
              break;
              
              default:break;
            }
            break;
          }
          switch (scancode)
          {
            case '0':
              //uint i=0;
              tmp_ram &= ~(1 << selected_bit);
              // we'll just write it immediately for now, can change to have user press enter later
              player->spc_write_dsp(gen_8bit_dsp_map[selected_index].addr, tmp_ram);
            break;
            case '1':
              tmp_ram |= 1 << selected_bit;
              player->spc_write_dsp(gen_8bit_dsp_map[selected_index].addr,tmp_ram);
            break;

            case SDLK_UP:
              if (selected_index == 0)
                selected_index = (SIZEOF_8BIT_GEN_DSP_ENUM-1);
              else selected_index--;
              cursor.rect.y = byte[selected_index].bits[selected_bit].y;
              cursor.rect.x = byte[selected_index].bits[selected_bit].x;
              tmp_ram = player->spc_read_dsp(gen_8bit_dsp_map[selected_index].addr);
              current_edit_addr = gen_8bit_dsp_map[selected_index].addr;
            break;
            case SDLK_DOWN:
              if (selected_index == (SIZEOF_8BIT_GEN_DSP_ENUM-1))
                selected_index = 0;
              else selected_index++;
              cursor.rect.x = byte[selected_index].bits[selected_bit].x;
              cursor.rect.y = byte[selected_index].bits[selected_bit].y;
              tmp_ram = player->spc_read_dsp(gen_8bit_dsp_map[selected_index].addr);
              current_edit_addr = gen_8bit_dsp_map[selected_index].addr;
            break;

            case SDLK_RETURN:
            case SDLK_ESCAPE:
              exit_edit_mode();
            default:break;
          }
        }
        break;
      } 
      switch (scancode)
      {
        case SDLK_ESCAPE:
          quitting=true;
          break;
        case SDLK_r:
          reset_screw();
          restart_current_track();
          break;

        case SDLK_LEFT:
          reset_screw();
          clear_used_srcn();
          prev_track();
          break;
        case SDLK_RIGHT:
          reset_screw();
          clear_used_srcn();
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
          if (mode == MODE_NAV)
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
      if (ev.user.code == UserEvents::mouse_react)
      {
        SDL_Event *te = (SDL_Event *)ev.user.data1; // the mouse coordinates at time of double click

        if (mode == MODE_EDIT_ADDR && Utility::coord_is_in_rect(te->motion.x, te->motion.y, &cursor.rect))
        {
          exit_edit_mode();
          break;
        }

        if (Utility::coord_is_in_rect(te->motion.x, te->motion.y, &dir_rect))
        {
          current_edit_addr = dsp_reg::dir;
          tmp_ram = player->spc_read_dsp(current_edit_addr);
          cursor.rect.x = dir_rect.x;
          cursor.rect.y = dir_rect.y;
          if (te->motion.x >= (dir_rect.x+CHAR_WIDTH))
          {
            fprintf(stderr, "clicked lonibble\n");
            cursor.rect.x += CHAR_WIDTH;
            highnibble = false;
            //
          }
          else highnibble = true;

          enter_edit_mode();
          submode = EDIT_DIR;
        }

        for (int b=0; b < SIZEOF_8BIT_GEN_DSP_ENUM; b++)
        {
          for (int i=0; i < 8; i++)
          {
            if (Utility::coord_is_in_rect(te->motion.x, te->motion.y, &byte[b].bits[i]))
            {
              fprintf(stderr, "you clicked [%d] bit %d\n",b, i);
              
              cursor.rect.x = byte[b].bits[i].x;
              cursor.rect.y = byte[b].bits[i].y;
              enter_edit_mode();
              submode = EDIT_GEN_DSP_8BIT_ADDR;
              //gen_8bit_dsp_map[b].addr;
              current_edit_addr = gen_8bit_dsp_map[b].addr;
              tmp_ram = player->spc_read_dsp(gen_8bit_dsp_map[b].addr);
              selected_index = b;
              selected_bit = i;
              break;
            }
          }
        }
        
        for (int i=0; i < SIZEOF_GEN_DSP_ENUM; i++)
        {
          //int x = ;
          // coord_is_in_rect(int x, int y, SDL_Rect *r);
          if (Utility::coord_is_in_rect(te->motion.x, te->motion.y, &clickable_gen_dsp[i].rect) )
          {
            fprintf(stderr, "OMG you clicked [%i]\n", i);

            selected_index = i;
            current_edit_addr = gen_dsp_map[i].addr;
            tmp_ram = player->spc_read_dsp(current_edit_addr);
            submode = EDIT_GEN_DSP_ADDR; //vs EDIT_VOICE_ADDR
            fprintf(stderr, "tmpram = 0x%02x\n", tmp_ram);

            cursor.rect.y = clickable_gen_dsp[i].rect.y;
            // try to derive LO/HI byte clicked
            

            //int rect_x = ;
            // assumption that all clicks will be over a "2char entry"
            if (te->motion.x >= (clickable_gen_dsp[i].rect.x+CHAR_WIDTH))
            {
              fprintf(stderr, "clicked lonibble\n");
              cursor.rect.x = clickable_gen_dsp[i].rect.x+CHAR_WIDTH;
              highnibble = false;
              //
            }
            else
            {
              highnibble=true;
              cursor.rect.x = clickable_gen_dsp[i].rect.x;
            }

            enter_edit_mode();
            break;
          }
        }

        for (int i=0; i < NUM_TIMERS; i++)
        {
          //int x = ;
          // coord_is_in_rect(int x, int y, SDL_Rect *r);
          if (Utility::coord_is_in_rect(te->motion.x, te->motion.y, &timers.value[i].rect) )
          {
            fprintf(stderr, "OMG you clicked [%i]\n", i);

            selected_index = i;
            current_edit_addr = 0xfa + i; //gen_dsp_map[i].addr;
            tmp_ram = player->spc_read(current_edit_addr);
            submode = EDIT_TIMER; //EDIT_GEN_DSP_ADDR; //vs EDIT_VOICE_ADDR
            fprintf(stderr, "tmpram = 0x%02x\n", tmp_ram);

            cursor.rect.y = timers.value[i].rect.y;
            // try to derive LO/HI byte clicked
            

            //int rect_x = ;
            // assumption that all clicks will be over a "2char entry"
            if (te->motion.x >= (timers.value[i].rect.x+CHAR_WIDTH))
            {
              fprintf(stderr, "clicked lonibble\n");
              cursor.rect.x = timers.value[i].rect.x+CHAR_WIDTH;
              highnibble = false;
              //
            }
            else
            {
              highnibble=true;
              cursor.rect.x = timers.value[i].rect.x;
            }

            enter_edit_mode();
            break;
          }
        }

        for (int voice=0; voice < MAX_VOICES; voice++)
        {
          for (int dsp_reg=0; dsp_reg < SIZEOF_VOICE_ENUM; dsp_reg++)
          {
            if (Utility::coord_is_in_rect(te->motion.x, te->motion.y, &clickable_voice[voice][dsp_reg].rect) )
            {
              fprintf(stderr, "OMG you clicked voice[%d][%d]\n",voice,dsp_reg);
              selected_index = voice;
              selected_sub_index = dsp_reg;
              submode = EDIT_VOICE_ADDR;
              current_edit_addr = voice*0x10 + dsp_reg;
              tmp_ram = player->spc_read_dsp(current_edit_addr);
              fprintf(stderr, "tmpram = 0x%02x\n", tmp_ram);

              cursor.rect.y = clickable_voice[voice][dsp_reg].rect.y;
              // try to derive LO/HI byte clicked
              

              //int rect_x = ;
              // assumption that all clicks will be over a "2char entry"
              if (te->motion.x >= (clickable_voice[voice][dsp_reg].rect.x+CHAR_WIDTH))
              {
                highnibble=false;
                fprintf(stderr, "clicked lobyte\n");
                cursor.rect.x = clickable_voice[voice][dsp_reg].rect.x+CHAR_WIDTH;
                //
              }
              else
              {
                highnibble= true;
                cursor.rect.x = clickable_voice[voice][dsp_reg].rect.x;
              }

              enter_edit_mode();
              break;
            }
          }
        }
      }
    } break;
    case SDL_MOUSEWHEEL:
    {
      
    } break;
    case SDL_MOUSEBUTTONDOWN:           
      {
        if (screw_clickable.check_mouse_and_execute(ev.button.x, ev.button.y))
          return 0;
        for (int i=0; i < NUM_TIMERS; i++)
        {
          timers.num = i;
          if (timers.label[i].check_mouse_and_execute(ev.button.x, ev.button.y))
            return 0;
        }
				/* NOTE: Add code here to check for clicks over DIR entries */
        for (int i=0; i < NUM_DIR_ENTRIES_DISPLAYED; i++)
        {
					// hooray for super unreadable code :(
					if ( ( ev.button.button == SDL_BUTTON_RIGHT &&
					      dir_rects[i].check_mouse_and_execute(ev.button.x, ev.button.y) ) ||
					     ( ev.button.button == SDL_BUTTON_LEFT &&
					      loop_clickable[i].clickable_text.check_mouse_and_execute(ev.button.x, ev.button.y)) )
            return 0;
        }
        for (int i=0; i < MAX_VOICES; i++)
        {
          uintptr_t newdata = (uintptr_t)voice_title[i].data; // originally the voice number itself
          if (ev.button.button == SDL_BUTTON_RIGHT)
            newdata = (uintptr_t)voice_title[i].data | 0x08;
          voice_title[i].check_mouse_and_execute(ev.button.x, ev.button.y, (void*)newdata);
        }
      }
      break;
      default:
      break;
  }
  return 0;
}

int Dsp_Window::dir_rect_clicked(void *idx)
{
	uintptr_t index = (uintptr_t) idx;

	// spawn a brr context here
	// supply the context action data
	// get the DIR address

	DEBUGLOG("index: 0x%x ; ", index);

	dir_index = 0 + (dir_offset * 2);
	if (index + dir_offset >= 0x100)
			dir_index = 0 - (DIR_ENTRIES_PER_COLUMN * 2);

	uint16_t cur_dir_index = dir_index + (index * 2);
	DEBUGLOG("BRR addr: %04x, %04x\n", dir[cur_dir_index], dir[cur_dir_index + 1]);
  ::brrcontext.update(dir[cur_dir_index], dir[cur_dir_index + 1]);

	::brrcontext.menu.preload(mouse::x, mouse::y);
	::brrcontext.menu.activate();
	return 0;
}
