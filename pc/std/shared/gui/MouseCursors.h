#pragma once
#include "DEBUGLOG.h"
#include "SDL.h"

/////////////////////////////////////////////////////////////////////////
struct BmpCursor
{
  ~BmpCursor();
  SDL_Point hotspot;
  const char *filename;
  SDL_Surface *surface;
  SDL_Cursor *cursor;
  bool loaded;
};

struct BmpCursorAniFrame
{
  ~BmpCursorAniFrame();
  const char *filename;
  int delay; // you can specify a per-frame delay :)
  SDL_Surface *surface;
  SDL_Cursor *cursor;
};

struct BmpCursorAni
{
  ~BmpCursorAni();
  static void set_cursor(BmpCursorAni *b);
  /* Sends a custom SDL User event to update the cursor from the main
   * thread */
  static Uint32 push_cursor_ani_update_event(Uint32 interval=0, void *param=NULL);

  /* called from the custom UserEvent "mouse_ani", this takes an index
   * into actively animating BmpCursorAni, not to be confused as an index into the
   * all-encompassing cursor enum found in Cursors.cpp. */
  static void set_frame(int i);
  
  static void stop(); /* stop animating. Used when changing the cursor from external
  API (set_cursor), and also from destructor code, precluding object deletion */

  /* Use one hotspot for the whole animation. It would be possible to
   * have custom hotspot per animation frame, but I imagine that kind of
   * moving hotspot would be terrible to use! So we use one. */
  SDL_Point hotspot;
  int num_frames;
  bool loaded;
  static BmpCursorAni *animating; /* The currently animating cursor,
                                     if any. NULL when there is none */

  static Uint32 timerid; /* the SDL timer id for the active animated
  cursor, 0 if no timer is active */

  static int ani_idx;
  BmpCursorAniFrame *frames;
};
/////////////////////////////////////////////////////////////////////////

struct MouseCursors
{
	MouseCursors();
	~MouseCursors();

	void set_cursor(int i);
	void next();
	void prev();

  int handle_event(const SDL_Event &ev);

  SDL_Cursor **syscursors;
  int index=0;

  BmpCursor *bci;
  BmpCursorAni *bca;

private:
  void load_bmp();
  void load_ani();
};
