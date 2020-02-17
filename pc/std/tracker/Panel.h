#pragma once
#include "shared/gui/Text_Edit_Rect.h"
#include "shared/utility.h"
// Helper functions shared between any panels. Putting here for now. Will
// be shared between Sample and Instrument panels.
void panel_clear_all_rows(Text_Edit_Rect *ters, int num_rows, SDL_Surface *screen);
void panel_clear_row(Text_Edit_Rect *ters, int row, SDL_Surface *screen);

inline void conv_idx2ascii(int i, char *c)
{
  // convert index to ascii
  *(c++) = Utility::nibble_to_ascii(i >> 4);
  *(c++) = Utility::nibble_to_ascii(i);
  *(c++) = '|';
  *(c++) = 0;
}

int mousewheel_rows_event_handler(const SDL_Event &ev, int *rows_scrolled,
    int visible_rows, int total_rows, SDL_Rect *rect);

/*struct Panel
{

};*/
