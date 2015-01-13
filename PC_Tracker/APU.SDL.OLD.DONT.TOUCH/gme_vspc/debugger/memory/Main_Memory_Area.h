#pragma once
#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "memory.h" 
#include "gui/cursor.h"
#include "memory/Mouse_Hexdump_Area.h"
#include "gui/Context_Menu.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include "Instrument_Window.h"

// expects pre-calculated main_memory_area.brr
int write_plain_brr_to_file(void *data);  // data is main_memory_area ptr
int play_sample(void *data);
int solo_sample(void *data);

struct BRR
{
  BRR();
  enum BRR_TYPE {
    CLICKED_ON_LOOP_ONLY=0,
    LOOP_SAMPLE,
    PLAIN_SAMPLE,
    NOT_A_SAMPLE
  };
  uint8_t one_solo=0x0;
  uint8_t srcn_solo=0x0;
  uint16_t srcn[MAX_VOICES];  // current voices' SRCN addresses
  uint16_t brr_start,brr_end; // for downloading BRR samples
  uint16_t brr_loop_start,brr_loop_end;
  uint8_t lowest_srcn_index=0x0;  // indexes into report::src[]
  uint8_t lowest_loop_index=0x0;
  int check_brr(uint16_t *address);
  void play_sample(Instrument_Window *instr_window);
  void solo_sample();
};

struct Main_Memory_Area : Memory
{
public:
  //friend class Mouse_Hexdump_Area;
  Main_Memory_Area(Mouse_Hexdump_Area *mouse_hexdump_area, uint16_t *dir);

  void set_addr(int i);
  void set_addr_from_cursor(int x, int y);
  //void add_addr(int i);

  void lock(char l=1, int x=0, int y=0, uint8_t rx=0, uint8_t ry=0);
  void toggle_lock(int x=0, int y=0);
  
  void unlock();
  //void update_editing_address();
  
  //void processContextEvents(SDL_Event &ev);
  
  Mem_Cursor memcursor;
  Mouse_Hexdump_Area *mouse_hexdump_area;
  char locked=0;
  Uint8 address_remainder=0;
  uint16_t mouse_addr=0; 
  uint16_t *dir; // unused, should be deleted

  void log_the_fucking_address_for_the_fucking_context_window();
  
  BRR brr;

  struct Context
  {
    Context(Main_Memory_Area* base) : menu(menu_items)
    {
      menu_items[SOLOSAMPLE].clickable_text.data = base;
      menu_items[RIPBRR].clickable_text.data = base;
      menu_items[PLAYSAMPLE].clickable_text.data = base;
    }
    // for tcontext menu
    uint16_t addr_when_user_right_clicked=0;
    uint8_t voice_to_play=0;

    enum {
      SOLOSAMPLE=0,
      PLAYSAMPLE,
      RIPBRR,
      RIPBRRP,
      RIPI,
      SIZEOF_MENU
    };

    Context_Menu menu;
    Context_Menu_Item menu_items[SIZEOF_MENU+1] = 
    {
      {"Solo Sample",true, &solo_sample,NULL},
      {"Play Sample",true, &play_sample, NULL},
      {"RIP BRR",true, &write_plain_brr_to_file, NULL},
      {"Rip BRR+",true},
      {"Rip Instrument",true},
      {"",false, NULL,NULL}
    };

  } context;
};