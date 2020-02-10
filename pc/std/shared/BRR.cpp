#include "BRR.h"

const char BRR::BRRP_MAGIC_STR[] = "ST-BRRP";  // the S has the low 2 bits set, 
                                                //an unlikely situation for any BRR Sample
                                                // 8-byte aligned
const char BRR::BRR_FILE_EXTENSION[] = "brr";
const char BRR::BRRP_FILE_EXTENSION[] = "brrp";

const char BRR::BRRI_MAGIC_STR[] = "ST-BRRI";  // 8 byte aligned
const char BRR::BRRI_FILE_EXTENSION[] = "brri";


int BRR::write_plain_brr_to_file(BRR *brr)
{
  SDL_RWops *file;
  nfdchar_t *outPath=NULL;
  if (Utility::get_file_write_handle(&outPath, &file, BRR_FILE_EXTENSION) == NFD_OKAY)
  {
    if (outPath !=NULL)
      fprintf(stderr, "%s\n", outPath);
    SDL_RWwrite(file, &BaseD::IAPURAM[brr->brr_start], brr->brr_end - brr->brr_start + 1, 1);
    SDL_RWclose(file);
    free(outPath);
  }

  return 0;
}

int BRR::write_loop_info_to_file(BRR *brr, SDL_RWops *file)
{
  Uint16 offset;
  SDL_RWwrite(file, &brr->is_looped_sample, 1, 1);
  SDL_RWwrite(file, &brr->is_loop_external, 1, 1);
  if (brr->is_looped_sample && ! brr->is_loop_external)
  {
    offset = brr->brr_loop_start - brr->brr_start;
    SDL_WriteLE16(file, offset);
  }
  else 
  {
    offset = 0x0000;
    SDL_WriteLE16(file, offset);
  }
  return 0;
}

int BRR::write_brrp_to_file(BRR *brr)
{
  SDL_RWops *file;
  nfdchar_t *outPath = NULL;
  
  //Uint16 zero_word = 0x0000;

  if (Utility::get_file_write_handle(&outPath, &file, BRRP_FILE_EXTENSION) == NFD_OKAY)
  {
    if (outPath !=NULL)
      fprintf(stderr, "%s\n", outPath);



    //MAGIC "penis breath"
    SDL_RWwrite(file, BRRP_MAGIC_STR, strlen(BRRP_MAGIC_STR), 1);
    // 
    write_loop_info_to_file(brr, file);
    //BRR Sample
    SDL_RWwrite(file, &BaseD::IAPURAM[brr->brr_start], brr->brr_end - brr->brr_start + 1, 1);
    /* 1 Byte boolean "is_loop_external" : 0x00 or 0xff - no or yes
      if no.: next and final byte is 16bit offset into previous sample where the loop point is.
      if yes: Next BRR Sample is provided */
    //

    if (brr->is_looped_sample && brr->is_loop_external) // write the external loop sample
      SDL_RWwrite(file, &BaseD::IAPURAM[brr->brr_loop_start], brr->brr_loop_end - brr->brr_loop_start + 1, 1);


    SDL_RWclose(file);
    free(outPath);
  }
  return 0;
}

int BRR::write_brri_to_file(BRR *brr)
{
  SDL_RWops *file;
  nfdchar_t *outPath = NULL;

  if (Utility::get_file_write_handle(&outPath, &file, BRRI_FILE_EXTENSION) == NFD_OKAY)
  {
    if (outPath !=NULL)
      fprintf(stderr, "%s\n", outPath);

    //MAGIC "penis breath"
    SDL_RWwrite(file, BRRI_MAGIC_STR, strlen(BRRI_MAGIC_STR), 1);
    // 
    write_loop_info_to_file(brr, file);
    // ADSR 1
    Uint8 tmpb = ::player->spc_read_dsp(brr->corresponding_voice*0x10 + dsp_reg::adsr1);
    SDL_RWwrite(file, &tmpb, 1, 1); 
    // ADSR 2
    tmpb = ::player->spc_read_dsp(brr->corresponding_voice*0x10 + dsp_reg::adsr2);
    SDL_RWwrite(file, &tmpb, 1, 1); 
    // GAIN
    tmpb = ::player->spc_read_dsp(brr->corresponding_voice*0x10 + dsp_reg::gain);
    SDL_RWwrite(file, &tmpb, 1, 1); 
    //BRR Sample
    SDL_RWwrite(file, &BaseD::IAPURAM[brr->brr_start], brr->brr_end - brr->brr_start + 1, 1);
    //
    if (brr->is_looped_sample && brr->is_loop_external) // write the external loop sample
      SDL_RWwrite(file, &BaseD::IAPURAM[brr->brr_loop_start], brr->brr_loop_end - brr->brr_loop_start + 1, 1);

    SDL_RWclose(file);
    free(outPath);
  }
  return 0;
}

void BRR::solo_sample()
{
  BaseD::voice_control.solo_bits(srcn_solo);
}

void BRR::play_sample(Instrument_Window *instr_window)
{
  instr_window->switch_mode(BaseD::GrandMode::INSTRUMENT);
  instr_window->voice.n = one_solo;
  instr_window->pause_spc();
  BaseD::voice_control.unmute_all();
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
  is_looped_sample = false;
  is_loop_external = false;
  bool no_lower_loop_found=false;

  assert (address != 0);
  Uint8 lowest_srcn_index=0x0;
  Uint8 lowest_loop_index=0x0;
  srcn_solo=0x0;
  fprintf(stderr, "one_solo = %d", 0);
  one_solo=0x0;

  // get closest SRCN address before clicked address
  uint16_t lowest_closest_srcn_address=0xffff, lowest_offset=0xffff;
  uint16_t lowest_closest_brrloopstart_address_from_click=0xffff;
  uint16_t lowest_offset2=0xffff;
  for (int i=0; i < MAX_SRCN_ENTRIES; i++)
  {
    Spc_Report::Src *src = &Spc_Report::src[i];
		if (src->brr_start == 0xffff) continue;

    if (src->brr_start < *address)
    {
      uint16_t offset = *address - src->brr_start;
      if (offset < lowest_offset)
      {
        lowest_closest_srcn_address=src->brr_start;

        lowest_srcn_index=i;
        lowest_offset = offset;
      }
    }
    if (src->brr_loop_start < *address)
    {
      uint16_t offset = *address - src->brr_loop_start;
      if (offset < lowest_offset2)
      {
        lowest_loop_index = i;
        lowest_closest_brrloopstart_address_from_click = src->brr_loop_start;
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
  for (int x=0; x < MAX_VOICES; x++)
  {
    if (srcn[x] == lowest_closest_srcn_address)
    {
      srcn_solo |= 1<<x;
      one_solo = x;
      corresponding_voice = x;
    }
  }


  // get closest LOOP Address before clicked address
  if (lowest_closest_brrloopstart_address_from_click == 0xffff)
  {
    no_lower_loop_found = true;
    fprintf(stderr, "no appropriate LOOP point was found :(\n");
  }

  // Find closest BRR end block after closest SRCN
  uint16_t lowest_closest_brrend_address_from_srcn = 0xffff;

  uint16_t p = lowest_closest_srcn_address;
  while(1)
  {
    if (BaseD::IAPURAM[p] & 1)
    {
      if (BaseD::IAPURAM[p] & 2)
        is_looped_sample=true;
      p+=8;
      break;
    }
    p+=9;
  }
  lowest_closest_brrend_address_from_srcn = p;

  if (lowest_closest_brrend_address_from_srcn == 0xffff)
  {
    fprintf(stderr, "no appropriate BRR_END after SRCN was found :(\n");
  }

  // get loop end
  uint16_t lowest_closest_brrend_address_from_loop = 0xffff;

  p = Spc_Report::src[lowest_loop_index].brr_loop_start;
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
  Spc_Report::src[lowest_loop_index].brr_loop_end = p;

  if (lowest_closest_brrend_address_from_loop == 0xffff)
  {
    fprintf(stderr, "no appropriate Loop BRR_END was found :(\n");
  }  

  if (!no_lower_loop_found && lowest_closest_brrloopstart_address_from_click > lowest_closest_brrend_address_from_srcn)
  {
    fprintf(stderr,"You clicked on an external Loop sample\n");
    brr_start = Spc_Report::src[lowest_loop_index].brr_start;
    brr_end = Spc_Report::src[lowest_loop_index].brr_end;
    brr_loop_start = Spc_Report::src[lowest_loop_index].brr_loop_start;
    brr_loop_end = Spc_Report::src[lowest_loop_index].brr_loop_end;
    return CLICKED_ON_LOOP_ONLY;
  }
  else 
  {
    Spc_Report::src[lowest_srcn_index].brr_end = lowest_closest_brrend_address_from_srcn;
    fprintf(stderr, "BRR @ 0x%04X-0x%04X\n", lowest_closest_srcn_address, lowest_closest_brrend_address_from_srcn);
    brr_start = lowest_closest_srcn_address;
    brr_end = lowest_closest_brrend_address_from_srcn; //inclusive
    if (is_looped_sample == true)
    {
      brr_loop_start = Spc_Report::src[lowest_srcn_index].brr_loop_start;
      brr_loop_end = Spc_Report::src[lowest_srcn_index].brr_loop_end;

      if (brr_loop_start > brr_end)
        is_loop_external = true;
      else is_loop_external = false;

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
  fprintf(stderr, "lowest_closest_brrloopstart_address_from_click = %04X\n", lowest_closest_brrloopstart_address_from_click);
}
