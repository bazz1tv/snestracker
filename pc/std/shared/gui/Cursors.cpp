#include "gui/Cursors.h"
#include "shared/File_System.h"
#include "shared/Render.h"
#include "sdl_userevents.h"

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
  CURSOR_BMP_START, // BMP CURSORS
  CURSOR_YOSHI_NORMAL=CURSOR_BMP_START,
  CURSOR_YOSHI_REC,
  CURSOR_KIRBYSTAR,
  CURSOR_MKART_1UP,
  CURSOR_MKART_MUSHROOM,
  CURSOR_MKART_BLUEMUSHROOM,
  CURSOR_MKART_YELLOWMUSHROOM,
  CURSOR_THING,
  CURSOR_THING2,
  CURSOR_MPAINT_MARIO,
  CURSOR_MPAINT_FROGGY,
  CURSOR_MPAINT_GOLDEN_HAND,
  CURSOR_MPAINT_WHITE_HAND,
  CURSOR_ZSNES,
  CURSOR_ZSNES2,
  
  CURSOR_ANI_START,
  CURSOR_SMRPG_COIN1=CURSOR_ANI_START,
  CURSOR_SMRPG_COIN2,
  CURSOR_SMRPG_COIN3,
  CURSOR_SMRPG_COIN4,
  CURSOR_SMRPG_COIN5,
  CURSOR_SMRPG_COIN6,
  CURSOR_SMRPG_COIN7,
  CURSOR_SMRPG_COIN8,
  NUM_CURSORS
};

#define NUM_BMP_CURSORS (NUM_CURSORS - CURSOR_BMP_START)

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
    { {5, 5}, "cursor-mariokart-mushroom" },
    { {5, 5}, "cursor-mariokart-bluemushroom" },
    { {5, 5}, "cursor-mariokart-yellowmushroom" },
    { {8, 8}, "thing" },
    { {8, 8}, "thing2" },
    { {6, 7}, "mpaint-mario" },
    { {7, 8}, "mpaint-froggy" },
    { {1, 1}, "mpaint-golden-hand" },
    { {1, 1}, "mpaint-white-hand" },
    { {1, 1}, "cursor-zsnes" },
    { {1, 1}, "cursor-zsnes2" },
    { {8,6}, "smrpg-smallcoinani1" },
    { {8,6}, "smrpg-smallcoinani2" },
    { {8,6}, "smrpg-smallcoinani3" },
    { {8,6}, "smrpg-smallcoinani4" },
    { {8,6}, "smrpg-smallcoinani5" },
    { {8,6}, "smrpg-smallcoinani6" },
    { {8,6}, "smrpg-smallcoinani7" },
    { {8,6}, "smrpg-smallcoinani8" },
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
    cursor[CURSOR_BMP_START + i] = SDL_CreateColorCursor(bci[i].surface,
            bci[i].hotspot.x, bci[i].hotspot.y);
    if (!cursor[CURSOR_BMP_START + i])
      goto _exit;
  }

  index = CURSOR_MPAINT_WHITE_HAND;
  SDL_SetCursor(cursor[CURSOR_MPAINT_WHITE_HAND]);
  ani_idx = CURSOR_SMRPG_COIN1; 
  timerid = SDL_AddTimer(50, &push_cursor_ani_update_event, &ani_idx);

  return;
_exit:
  fprintf(stderr, "OH NO!\n");
  for (int i=0; i < NUM_BMP_CURSORS; i++)
  {
    if (cursor[CURSOR_BMP_START + i]) {
      SDL_FreeCursor(cursor[CURSOR_BMP_START + i]);
      cursor[CURSOR_BMP_START + i] = NULL;
    }
    if (bci[i].surface) {
        SDL_FreeSurface(bci[i].surface);
        bci[i].surface = NULL;
    }
  }
}

void Cursors::set_cursor(int i)
{
  //fprintf(stderr, "i = %d\n");
  SDL_SetCursor(cursor[i]);
}

Uint32 Cursors::push_cursor_ani_update_event(Uint32 interval/*=0*/, void *param/*=NULL*/)
{
  int *index = (int *)param;
  //fprintf(stderr, "in timer: index=%d\n", *index);

  *index += 1;
  if (*index >= (CURSOR_SMRPG_COIN1 + 8))
    *index = CURSOR_SMRPG_COIN1;
  //SDL_Cursor *c = cursor[CURSOR_SMRPG_COIN1 + *index];
  // check param for index of smrpg cursor
  SDL_Event event;
  event.type = SDL_USEREVENT;
  event.user.code = UserEvents::mouse_ani;
  event.user.data1 = (void *)*index;
  event.user.data2 = 0;
  SDL_PushEvent(&event);

  return interval;
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
