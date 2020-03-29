#pragma once
#include "memory.h" 
#include "gui/Mem_Cursor.h"
#include "memory/Mouse_Hexdump_Area.h"

int write_brri_to_file(void *data);
int write_brrp_to_file(void *data);
int write_plain_brr_to_file(void *data);  // data is main_memory_area ptr

struct Main_Memory_Area : Memory
{
public:
  Main_Memory_Area(Mouse_Hexdump_Area *mouse_hexdump_area, uint16_t *dir);

  void set_addr(int i);
  void set_addr_from_cursor(int x, int y);

  void lock(char l=1, int x=0, int y=0, uint8_t rx=0, uint8_t ry=0);
  void toggle_lock(int x=0, int y=0);
  
  void unlock();

/////////////// BRR COLLECTION /////////////////
	enum BRRTYPE {
		CLICKED_ON_LOOP_ONLY=0,
		LOOP_SAMPLE,
		PLAIN_SAMPLE,
		NOT_A_SAMPLE
	};

	void spawnbrrcontextmenu(int x, int y);

	int check_brr(Uint16 *address);
	bool is_looped_sample=false;
	bool is_loop_external=false;
	Uint16 brr_start,brr_end; // for downloading BRR samples
	Uint16 brr_loop_start,brr_loop_end;
	Uint8 corresponding_voice=0x0;
	Uint8 one_solo=0x0;
	Uint8 srcn_solo=0x0;
	static int solo_sample(void *data);
	static int play_sample(void *data);
///////////// END BRR COLLECTION ///////////////

  Mem_Cursor memcursor;
  Mouse_Hexdump_Area *mouse_hexdump_area;
  char locked=0;
  Uint8 address_remainder=0;
  uint16_t mouse_addr=0; 
  uint16_t *dir; // unused, should be deleted

	uint16_t addr_when_user_right_clicked=0;
  void log_the_fucking_address_for_the_fucking_context_window();
};
