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

	SDL_Cursor **cursor;
	int index=0;
  struct BmpCursor
  {
    //~BmpCursor();
    SDL_Point hotspot;
    const char *filename;
    SDL_Surface *surface;
    bool loaded;
  };
  BmpCursor *bci;

  struct BmpCursorFrame
  {
    //~BmpCursorFrame();
    // could add this if dynamic hotspots are needed
    //SDL_Point hotspot;
    const char *filename;
    int delay; // only supporting constant framerate for the time being
    SDL_Surface *surface;
    SDL_Cursor *cursor;
  };

  struct BmpCursorAni
  {
    //~BmpCursorAni();
    SDL_Point hotspot;
    int num_frames; // only used if ani is true
    bool loaded;
    static BmpCursorAni *animating;
    static Uint32 timerid;
    static Uint32 push_cursor_ani_update_event(Uint32 interval=0, void *param=NULL);
    static void stop();
    static void set(int i);
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
