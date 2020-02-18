#pragma once
#include "DEBUGLOG.h"
#include "SDL.h"
/* This is a collection of cursors 

Set the cursor with SDL_SetCursor:
SDL_SetCursor(cursors.cursor[Cursors::CURSOR_SIZEALL]);
*/

#define SET_CURSOR_ARROW SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_ARROW])
#define SET_CURSOR_IBEAM SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_IBEAM])
//
#define SET_CURSOR_WAIT SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_WAIT])
#define SET_CURSOR_CROSSHAIR SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_CROSSHAIR])
#define SET_CURSOR_WAITARROW SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_WAITARROW])
//
#define SET_CURSOR_SIZENWSE SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_SIZENWSE])
#define SET_CURSOR_SIZENESW SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_SIZENESW])
#define SET_CURSOR_SIZEWE SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_SIZEWE])
#define SET_CURSOR_SIZENS SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_SIZENS])
#define SET_CURSOR_SIZEALL SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_SIZEALL])
//
#define SET_CURSOR_NO SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_NO])
#define SET_CURSOR_HAND SDL_SetCursor(BaseD::cursors.cursor[Cursors::CURSOR_HAND])

struct Cursors
{
	Cursors();
	~Cursors();

  void set_yoshi();

	SDL_Cursor **syscursors;
	int index=0;

  struct BmpCursor
  {
    ~BmpCursor();
    SDL_Point hotspot;
    const char *filename;
    SDL_Surface *surface;
    SDL_Cursor *cursor;
    bool loaded;
  };
  BmpCursor *bci;

  struct BmpCursorFrame
  {
    ~BmpCursorFrame();
    const char *filename;
    int delay; // you can specify a per-frame delay :)
    SDL_Surface *surface;
    SDL_Cursor *cursor;
  };

  struct BmpCursorAni
  {
    ~BmpCursorAni();
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
    /* Sends a custom SDL User event to update the cursor from the main
     * thread */
    static Uint32 push_cursor_ani_update_event(Uint32 interval=0, void *param=NULL);
    static void stop(); /* stop animating. Used when changing the cursor from external
    API (set_cursor), and also from destructor code, precluding object deletion */
    /* called from the custom UserEvent "mouse_ani", this takes an index
     * into actively animating BmpCursorAni, not to be confused as an index into the
     * all-encompassing cursor enum found in Cursors.cpp. */
    static void set_frame(int i);
    static int ani_idx;
    BmpCursorFrame *frames;
  };
  BmpCursorAni *bca;

	void next();
	void prev();
  
  void set_cursor(int i);
private:
  void load_bmp();
  void load_ani();
};
