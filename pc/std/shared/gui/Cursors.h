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
    SDL_Point hotspot;
    const char *filename;
    SDL_Surface *surface;
  };
  BmpCursor *bci;

	void next();
	void prev();
};
