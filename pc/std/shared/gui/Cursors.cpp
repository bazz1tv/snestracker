#include "gui/Cursors.h"
#include "shared/File_System.h"
#include "shared/Render.h"

enum {
  CURSOR_ARROW=0,
  CURSOR_IBEAM,
  CURSOR_WAIT,
  CURSOR_CROSSHAIR,
  CURSOR_WAITARROW,
  CURSOR_SIZENWSE,
  CURSOR_SIZENESW,
  CURSOR_SIZEWE,
  CURSOR_SIZENS,
  CURSOR_SIZEALL,
  CURSOR_NO,
  CURSOR_HAND,
  // BMP CURSORS
  CURSOR_YOSHI_NORMAL,
  CURSOR_YOSHI_REC,
  CURSOR_KIRBYSTAR,
  CURSOR_MKART_1UP,
  CURSOR_THING,
  CURSOR_THING2,
  CURSOR_MPAINT_MARIO,
  CURSOR_MPAINT_FROGGY,
  CURSOR_MPAINT_GOLDEN_HAND,
  CURSOR_MPAINT_WHITE_HAND,
  NUM_CURSORS
};

#define NUM_BMP_CURSORS (NUM_CURSORS - CURSOR_YOSHI_NORMAL)

Cursors::Cursors()
{
  cursor = new SDL_Cursor*[NUM_CURSORS];
	cursor[CURSOR_ARROW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	cursor[CURSOR_IBEAM] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	cursor[CURSOR_WAIT] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
	cursor[CURSOR_CROSSHAIR] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
	cursor[CURSOR_WAITARROW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
	cursor[CURSOR_SIZENWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	cursor[CURSOR_SIZENESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	cursor[CURSOR_SIZEWE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
	cursor[CURSOR_SIZENS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	cursor[CURSOR_SIZEALL] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	cursor[CURSOR_NO] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
	cursor[CURSOR_HAND] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

  bci = new BmpCursor[NUM_BMP_CURSORS]{
    { {8, 8}, "cursor" },
    { {8, 8}, "cursor-rec" },
    { {7, 7}, "cursor-kirbystar" },
    { {5, 5}, "cursor-mariokart-1up" },
    { {8, 8}, "thing" },
    { {8, 8}, "thing2" },
    { {6, 7}, "mpaint-mario" },
    { {7, 8}, "mpaint-froggy" },
    { {1, 1}, "mpaint-golden-hand" },
    { {1, 1}, "mpaint-white-hand" },
  };

}

Cursors::~Cursors() {
	DEBUGLOG("~Cursors\n");
	for (int i=0; i < NUM_CURSORS; i++)
		SDL_FreeCursor(cursor[i]);

  for (int i=0; i < NUM_BMP_CURSORS; i++)
    SDL_free(bci[i].surface);

  delete[] bci;
  delete[] cursor;
}

void Cursors::set_yoshi()
{
  char tb[200];
  int len;
  assert(::file_system);
  // quoted data path does not play nice here
  strcpy(tb, ::file_system->data_path);
  len = strlen(tb);
  for (int i=0; i < NUM_BMP_CURSORS; i++)
  {
    tb[len] = 0;
    strcat(tb, bci[i].filename);
    strcat(tb, ".bmp");
    //strcat(tb, "\"");
    bci[i].surface = SDL_LoadBMP(tb);
    SDL_SetColorKey(bci[i].surface, SDL_TRUE, SDL_MapRGB(bci[i].surface->format, 0, 0xff, 0));
    DEBUGLOG("path = %s\n", tb);
    if (!bci[i].surface)
    {
      DEBUGLOG("SURFACE: %s\n", SDL_GetError());
      goto _exit;
    }
    cursor[CURSOR_YOSHI_NORMAL + i] = SDL_CreateColorCursor(bci[i].surface,
            bci[i].hotspot.x, bci[i].hotspot.y);
    if (!cursor[CURSOR_YOSHI_NORMAL + i])
      goto _exit;
  }

  SDL_SetCursor(cursor[CURSOR_MPAINT_WHITE_HAND]);
  return;
_exit:
  fprintf(stderr, "OH NO!\n");
  for (int i=0; i < NUM_BMP_CURSORS; i++)
  {
    if (cursor[CURSOR_YOSHI_NORMAL + i]) {
      SDL_FreeCursor(cursor[CURSOR_YOSHI_NORMAL + i]);
      cursor[CURSOR_YOSHI_NORMAL + i] = NULL;
    }
    if (bci[i].surface) {
        SDL_FreeSurface(bci[i].surface);
        bci[i].surface = NULL;
    }
  }
}

void Cursors::next()
{
	if (++index >= NUM_CURSORS)
		index = 0;
	SDL_SetCursor(cursor[index]);
}
void Cursors::prev()
{
	if (--index < 0)
		index = NUM_CURSORS - 1;
	SDL_SetCursor(cursor[index]);
}
