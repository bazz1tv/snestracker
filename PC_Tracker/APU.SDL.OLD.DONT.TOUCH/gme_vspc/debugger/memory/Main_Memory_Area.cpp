#include "Main_Memory_Area.h"
#include "gme/player/Music_Player.h"
#include "report.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include <nfd.h>
#include "SDL_mouse.h"

//extern uint8_t* IAPURAM;
//extern Music_Player *player;

int demo(void *data)
{
  
  //SDL_SetWindowGrab(BaseD::sdlWindow, SDL_TRUE);
  Main_Memory_Area *mma = (Main_Memory_Area*)data;

  nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_SaveDialog( NULL, NULL, &outPath );

    if ( result == NFD_OKAY ) {
        puts("Success!");
        puts(outPath);
        //SDL_RWops* SDL_RWFromFile(const char* file,
          //                const char* mode)
        SDL_RWops *file = SDL_RWFromFile(outPath, "wb");
        if (file == NULL)
        {
          printf( "Warning: Unable to open file! SDL Error: %s\n", SDL_GetError() );
          return -1;
        }
        SDL_RWwrite(file, &BaseD::IAPURAM[mma->brr.brr_start], mma->brr.brr_end - mma->brr.brr_start + 1, 1);
        SDL_RWclose(file);
        free(outPath);
    }
    else if ( result == NFD_CANCEL ) {
        puts("User pressed cancel.");
    }
    else {
        printf("Error: %s\n", NFD_GetError() );
    }
  // need the following to keep focus on window after saving the file
  SDL_RaiseWindow(BaseD::sdlWindow);

  return 0;
}

int play_sample(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data;
  mma->brr.play_sample(BaseD::instr_window);
  return 0;
}

int solo_sample(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data;
  mma->brr.solo_sample();
  return 0;
}

void BRR::solo_sample()
{
  BaseD::voice_control.solo_bits(srcn_solo);
}

void BRR::play_sample(Instrument_Window *instr_window)
{
  //Instrument_Window *instr_window = 
  instr_window->switch_mode(BaseD::GrandMode::INSTRUMENT);
  instr_window->voice.n = one_solo;
  instr_window->pause_spc();
}

BRR::BRR()
{
  for (int i=0; i < MAX_VOICES; i++)
  {
    srcn[i]=0xffff;
  }
}

int BRR::check_brr(uint16_t *address)
{
  bool does_sample_loop = false;
  bool no_lower_loop_found=false;
  //uint16_t *address = &context.addr_when_user_right_clicked;
  //fprintf(stderr, "address = %04X\n", context.addr_when_user_right_clicked);
  assert (address != 0);
  lowest_srcn_index=0x0;
  lowest_loop_index=0x0;
  srcn_solo=0x0;
  one_solo=0x0;
  

  // get closest SRCN address before clicked address
  uint16_t lowest_closest_srcn_address=0xffff, lowest_offset=0xffff;
  uint16_t lowest_closest_brrloopstart_address_from_click=0xffff;
  uint16_t lowest_offset2=0xffff, lowest_offset3=0xffff;
  for (int i=0; i < MAX_SRCN_ENTRIES; i++)
  {
    if (report::src[i].brr_start == 0xffff) continue;

    if (report::src[i].brr_start < *address)
    {
      uint16_t offset = *address - report::src[i].brr_start;
      if (offset < lowest_offset)
      {
        lowest_closest_srcn_address=report::src[i].brr_start;

        lowest_srcn_index=i;
        //fprintf(stderr, "lowest = %04X, %04X\n", lowest_closest_srcn_address, report::SRCN_used[i]);
        lowest_offset = offset;
      }
    }
    if (report::src[i].brr_loop_start < *address)
    {
      uint16_t offset = *address - report::src[i].brr_loop_start;
      if (offset < lowest_offset2)
      {
        lowest_loop_index = i;
        lowest_closest_brrloopstart_address_from_click = report::src[i].brr_loop_start;
        lowest_offset2 = offset;
      }
    }

  }

  // quit out of we clicked before any SAMPLE.. i mean that's an idiot click right there...
  if (lowest_closest_srcn_address == 0xffff)
  {
    fprintf(stderr, "no appropriate SRCN was found :(\n");
    return NOT_A_SAMPLE;
  }



  // solo the voices that are using this sample RIGHT NOW
  // if no voice is using this sample, just keep them all going..
  uint8_t first_bit = 0;
    for (int x=0; x < MAX_VOICES; x++)
    {
      if (srcn[x] == lowest_closest_srcn_address)
      {
        srcn_solo |= 1<<x;
        one_solo = x;
      }
    }


  // get closest LOOP Address before clicked address
  if (lowest_closest_brrloopstart_address_from_click == 0xffff)
  {
    no_lower_loop_found = true;
    fprintf(stderr, "no appropriate LOOP point was found :(\n");
    //return -1;
  }

  // Find closest BRR end block after closest SRCN
  uint16_t lowest_closest_brrend_address_from_srcn = 0xffff;

  /*if (report::src[lowest_srcn_index].brr_end != 0xffff)
  {
    fprintf(stderr, "derp");
    lowest_closest_brrend_address_from_srcn = report::src[lowest_srcn_index].brr_end;
  }
  else
  {*/
    uint16_t p = lowest_closest_srcn_address;
    while(1)
    {
      if (BaseD::IAPURAM[p] & 1)
      {
        if (BaseD::IAPURAM[p] & 2)
          does_sample_loop=true;
        p+=8;
        break;
      }
      p+=9;
    }
    lowest_closest_brrend_address_from_srcn = p;
    // = p;
  //}

  if (lowest_closest_brrend_address_from_srcn == 0xffff)
  {
    fprintf(stderr, "no appropriate BRR_END after SRCN was found :(\n");
    //return -1;
  }

  // get loop end
  uint16_t lowest_closest_brrend_address_from_loop = 0xffff;

  /*if (report::src[lowest_loop_index].brr_loop_end != 0xffff)
    ;lowest_closest_brrend_address_from_loop= report::src[lowest_loop_index].brr_loop_end;
  else
  {*/
    p = report::src[lowest_loop_index].brr_loop_start;
    while(1)
    {
      if (BaseD::IAPURAM[p] & 1)
      {
        p+=8;
        break;
      }
      p+=9;
    }
    lowest_closest_brrend_address_from_loop = p;
    report::src[lowest_loop_index].brr_loop_end = p;
  //}

  if (lowest_closest_brrend_address_from_loop == 0xffff)
  {
    fprintf(stderr, "no appropriate Loop BRR_END was found :(\n");
    //return -1;
  }  

  if (!no_lower_loop_found && lowest_closest_brrloopstart_address_from_click > lowest_closest_brrend_address_from_srcn)
  {
    fprintf(stderr,"You clicked on an external Loop sample\n");
    brr_start = report::src[lowest_loop_index].brr_start;
    brr_end = report::src[lowest_loop_index].brr_end;
    brr_loop_start = report::src[lowest_loop_index].brr_loop_start;
    brr_loop_end = report::src[lowest_loop_index].brr_loop_end;
    return CLICKED_ON_LOOP_ONLY;
  }
  else 
  {
    report::src[lowest_srcn_index].brr_end = lowest_closest_brrend_address_from_srcn;
    fprintf(stderr, "BRR @ 0x%04X-0x%04X\n", lowest_closest_srcn_address, lowest_closest_brrend_address_from_srcn);
    brr_start = lowest_closest_srcn_address;
    brr_end = lowest_closest_brrend_address_from_srcn; //inclusive
    if (does_sample_loop == true)
    {
      brr_loop_start = report::src[lowest_srcn_index].brr_loop_start;
      brr_loop_end = report::src[lowest_srcn_index].brr_loop_end;
      return LOOP_SAMPLE;
    }
    else
    {
      brr_loop_start=0xffff;
      brr_loop_end = 0xffff;
      return PLAIN_SAMPLE;
    }
  }

  fprintf(stderr, "lowest_closest_srcn_address = %04X\n", lowest_closest_srcn_address);
  fprintf(stderr, "lowest_closest_brrend_address_from_srcn = %04X\n", lowest_closest_brrend_address_from_srcn);
  //fprintf(stderr, "lowest closest")
  fprintf(stderr, "lowest_closest_brrloopstart_address_from_click = %04X\n", lowest_closest_brrloopstart_address_from_click);
  //fprintf(stderr, "lowest_closest_brrend_address_from_click = %04X\n", lowest_closest_brrend_address_from_click);
  
}

void Main_Memory_Area::log_the_fucking_address_for_the_fucking_context_window()
{
  context.addr_when_user_right_clicked = mouse_addr;
}

Main_Memory_Area::Main_Memory_Area(Mouse_Hexdump_Area *mouse_hexdump_area, uint16_t *dir) :
mouse_hexdump_area(mouse_hexdump_area), dir(dir),
context(this)
{
  
}



void Main_Memory_Area::set_addr(int i)
{
  if (memcursor.is_active())
  {
    report::restore_color(addr_being_edited);
    memcursor.start_timer();
  }
  Memory::set_addr(i);
  // shared variables, but not all of them
  mouse_hexdump_area->update_editing_address();
}
void Main_Memory_Area::set_addr_from_cursor(int x, int y)
{
  x-= MEMORY_VIEW_X;
  y -= MEMORY_VIEW_Y;
  x /= 2;
  y /= 2;
  set_addr(y*256+x);
}


//void dec_cursor_row();
//void inc_cursor_row();

void Main_Memory_Area::lock(char l/*=1*/, int x/*=0*/, int y/*=0*/, uint8_t rx/*=0*/, uint8_t ry/*=0*/)
{
  
  locked = l;
  if (locked)
  {
     res_y = 0;
    if (mouse_hexdump_area->submode != Mouse_Hexdump_Area::EASY_EDIT)
    {
      if (rx || ry)
      {
        res_x = rx; res_y = ry;
      }
      else
      {
        address_remainder = address % 8;
        add_addr(-address_remainder);
        res_x = 0; // address_remainder;
      }
    }

    //mode = MODE_EDIT_MOUSE_HEXDUMP;
    cursor.start_timer();

    memcursor.start_timer();
    report::restore_color(addr_being_edited);
    addr_being_edited = address+(res_y*8)+res_x;
  }
  else
  {
    if (x && y)
    {
      if (  x >= MEMORY_VIEW_X && 
                  x < MEMORY_VIEW_X + 512 &&
                  y >= MEMORY_VIEW_Y &&
                  y < MEMORY_VIEW_Y + 512 )
      {
        set_addr_from_cursor(x, y);
      }
    }
    
    report::restore_color(addr_being_edited);
    //mode = MODE_NAV;
    cursor.stop_timer();
    memcursor.stop_timer();
  }
  
}
void Main_Memory_Area::toggle_lock(int x/*=0*/, int y/*=0*/)
{
  lock(!locked, x, y);
}

void Main_Memory_Area::unlock()
{
  lock(0);
}