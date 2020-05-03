#pragma once

/* I've got an idea how the scrollbar can be created to modify arbitrary
 * widgets. We know can discover the math to determine the scroll bar
 * height. The scroll bar's position controls an offset value into the
 * rows that will be displayed by the widget. This offset value should be
 * directly owned by the widget. When the widget creates the scrollbar
 * object it owns, it should pass a pointer to the scroll bar so that it
 * can be manipulated by the scrollbar as it's controlled by the user.*/

/* This is not a "pixel" scroller. It scrolls by rows. For
 * example:
 *
 * +----+----+ ^
 * | 00 | 00 | -
 * | 01 | 01 | |
 * | 02 | 02 | -
 * | 03 | 02 |
 * +----+----+ v
 *
 * In this pattern table example, only 4 entries are visible at a time.
 * When > amount_visible is exceeded, the scroll bar becomes relevant.
 * 
 * 1. There will be math to specify the size of the vertical height of the
 * scroll bar.
 *
 * . Create the graphics for the scroll bars. The up arrow button, the
 * down arrow button (just a vflip), and the scroll bar block which is
 * dynamically created based on its height.
 */ 

class Scroll_Bar
{
public:
  Scroll_Bar(const Uint32 *total_rows, const Uint32 *num_vis,
              const Uint32 *display_offset) :
    up_arrow(FONT_CODE_V_TRI_STR),
    down_arrow(FONT_CODE_V_TRI_STR),
    x(x), y(y), total_entries(total), num_visible(num_vis),
    cur_offset(cur_offset)
  {
    scrollbar.rect.w = CHAR_WIDTH; // for now, have it the same width as regular characters
    scrollbar.rect.x = x;
    scrollbar.rect.y = y + CHAR_HEIGHT; // scrollbar starts underneath the vert arrow char
  }

  void run(); // 
  void draw(SDL_Screen *screen);

  Clickable_Text up_arrow,down_arrow;
  // This is the actual scrollbar whose height is dynamic.
  Clickable_Rect scrollbar;

  SDL_Rect rect; // (x,y) location of top-left of scroll-bar
  // also its  width, height

  // These are read-only pointers to the original data, for easy keeping
  // up to date if the graphical specifications change in real time (eg. a
  // pattern changing in length, window being resized could up/downgrade
  // the maximum possible displayed constraints
  const int *vis_invis_rows; // number of rows pertaining to this scroll bar
  const int *vis_rows; // number of rows that can be visible in the scrollable area
  int *disp_offset; // the starting offset we are displaying into the total_entries
  // So, we display entries from offset to (offset + num_visible)
};
