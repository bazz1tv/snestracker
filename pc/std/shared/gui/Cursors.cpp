#include "gui/Cursors.h"
#include "shared/File_System.h"
#include "shared/Render.h"

Cursors::Cursors()
{
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
}

Cursors::~Cursors() {
	DEBUGLOG("~Cursors\n");
	for (int i=0; i < NUM_CURSORS; i++)
		SDL_FreeCursor(cursor[i]);

  for (int i=0; i < 2; i++)
    SDL_free(surfaces[i]);
}

void Cursors::set_yoshi()
{
  char *bmpfile[] = {"cursor.bmp", "cursor-rec.bmp"};
  char tb[200];
  int len;
  assert(::file_system);
  // quoted data path does not play nice here
  strcpy(tb, ::file_system->data_path);
  len = strlen(tb);
  for (int i=0; i < 2; i++)
  {
    tb[len] = 0;
    strcat(tb, bmpfile[i]);
    //strcat(tb, "\"");
    surfaces[i] = SDL_LoadBMP(tb);
    SDL_SetColorKey(surfaces[i], SDL_TRUE, SDL_MapRGB(surfaces[i]->format, 0, 0xff, 0));
    DEBUGLOG("path = %s\n", tb);
    if (!surfaces[i])
    {
      DEBUGLOG("SURFACE: %s\n", SDL_GetError());
      goto _exit;
    }
    cursor[CURSOR_YOSHI_NORMAL + i] = SDL_CreateColorCursor(surfaces[i], 8, 8);
    if (!cursor[CURSOR_YOSHI_NORMAL + i])
      goto _exit;
  }

  SDL_SetCursor(cursor[CURSOR_YOSHI_NORMAL]);
  return;
_exit:
  fprintf(stderr, "OH NO!\n");
  for (int i=0; i < 2; i++)
  {
    if (cursor[CURSOR_YOSHI_NORMAL + i]) {
      SDL_FreeCursor(cursor[CURSOR_YOSHI_NORMAL + i]);
      cursor[CURSOR_YOSHI_NORMAL] = NULL;
    }
    if (surfaces[i]) {
        SDL_FreeSurface(surfaces[i]);
        surfaces[i] = NULL;
    }
  }
}
