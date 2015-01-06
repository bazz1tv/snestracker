#include "debugger/gui/Main_Memory_Context_Menu.h"
#include "report.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"

Main_Memory_Context_Menu::Main_Memory_Context_Menu()
{

}
void Main_Memory_Context_Menu::preload(int &x, int &y, uint16_t &address)
{
  Context_Menu::preload(x,y);

  // determine if right-click happened on a BRR Sample
  // find the closest entry in array that is above the user's clicked address..
  uint16_t lowest_offset=0xffff;
  bool can_show_brr_context=false;
  uint8_t lowest_brr_index;
  for (uint8_t i=0; i < BRR_HEADER_MAX; i++)
  {
    if (report::BRR_Headers[i] == 0xffff)
      break;
    
    if (report::BRR_Headers[i] > address)
    {
      uint16_t offset = report::BRR_Headers[i] - address; //@init = 0xffff-0xffff
      if (offset < lowest_offset)
      {
        lowest_offset = offset;
        lowest_brr_index = i;
      }
    }
    

  }

  if (lowest_offset != 0xffff)
  {
    // POTENTIAL
    uint16_t dir = (uint16_t)player->spc_read_dsp(dsp_reg::dir)*0x100;
    // checking the voices' SRCN (*4)
    for (int i=0; i < 8; i++)
    {
      uint16_t srcn = player->spc_read_dsp(i*0x10+04);
      uint16_t dir_entry = srcn*4;

      if (IAPURAM[dir+dir_entry] < address)
      {
        // should have an array of ALL SRCN used during song
        // MUST reinitialize  BRR_Headers array at BaseD::reload()
      }

    }
  }
  // even if user clicks before a BRR sample.. this logic will just download the
  // next sample in memory

}


void Main_Memory_Context_Menu::push(Clickable_Text &ref)
{

}