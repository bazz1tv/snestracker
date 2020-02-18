#include "MouseCursors.h"
#include "shared/File_System.h"
#include "shared/Render.h"
#include "sdl_userevents.h"

/* These are animated cursor variables that only 1 instance is needed of.
 * Will be used for any specific animated cursor that becomes active */
BmpCursorAni * BmpCursorAni::animating = NULL;
Uint32 BmpCursorAni::timerid = 0;
int BmpCursorAni::ani_idx;


/* enum of all cursors, including BMP, and ANImated BMP */
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
  NUM_SYS_CURSORS,
  CURSOR_BMP_START=NUM_SYS_CURSORS, // BMP CURSORS
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
  /* Track the num of SYS and BMP cursors because the same function is
   * used to load cursors of these types. On the other hand, we have the
   * animated cursors */
  CURSOR_BMP_END,
  NUM_SYS_AND_BMP_CURSORS=CURSOR_BMP_END,
  CURSOR_ANI_START=NUM_SYS_AND_BMP_CURSORS,
  CURSOR_SMRPG_COIN=CURSOR_ANI_START,
  CURSOR_ANI_END,
  NUM_CURSORS=CURSOR_ANI_END
};

#define NUM_BMP_CURSORS (NUM_SYS_AND_BMP_CURSORS - CURSOR_BMP_START)
#define NUM_ANI_CURSORS (NUM_CURSORS - CURSOR_ANI_START)
#define NUM_BMP_AND_ANI_CURSORS (NUM_BMP_CURSORS + NUM_ANI_CURSORS)
//#define BMP_CURSORS_ANI_START_IDX (CURSOR_ANI_START - CURSOR_BMP_START)
#define GET_BMP_IDX(x) (x - CURSOR_BMP_START)
#define GET_ANI_IDX(x) (x - CURSOR_ANI_START)











BmpCursor::~BmpCursor()
{
  if (cursor)
  {
    SDL_FreeCursor(cursor);
    cursor = NULL;
  }
  if (surface)
  {
    SDL_FreeSurface(surface);
    surface = NULL;
  }
}

BmpCursorAniFrame::~BmpCursorAniFrame()
{
  if (cursor)
  {
    SDL_FreeCursor(cursor);
    cursor = NULL;
  }
  if (surface)
  {
    SDL_FreeSurface(surface);
    surface = NULL;
  }
}

BmpCursorAni::~BmpCursorAni()
{
  delete[] frames;
}

void BmpCursorAni::set_cursor(BmpCursorAni *b)
{
  BmpCursorAni **aa = &BmpCursorAni::animating;
  // is there already a cursor animating?
  if (*aa == b)
    return;

  BmpCursorAni::stop();
  // start the first frame
  *aa = b;
  SDL_SetCursor(b->frames[0].cursor);
  BmpCursorAni::timerid = SDL_AddTimer(b->frames[0].delay,
      &BmpCursorAni::push_cursor_ani_update_event,
      &BmpCursorAni::ani_idx);

}

Uint32 BmpCursorAni::push_cursor_ani_update_event(Uint32 interval/*=0*/, void *param/*=NULL*/)
{
  int *i = (int *)param;
  //fprintf(stderr, "in timer: index=%d\n", *index);
  assert(animating);
  *i += 1;
  if (*i >= animating->num_frames)
    *i = 0;

  SDL_Event event;
  event.type = SDL_USEREVENT;
  event.user.code = UserEvents::mouse_ani;
  event.user.data1 = (void *)(*i);
  event.user.data2 = 0;
  SDL_PushEvent(&event);

  return animating->frames[*i].delay;
}

void BmpCursorAni::set_frame(int i)
{
  if (animating)
    SDL_SetCursor(animating->frames[i].cursor);
  else
    fprintf(stderr, "BmpCursorAni::set() called while no animating cursor set?!\n");
}

void BmpCursorAni::stop()
{
  if (timerid)
    SDL_RemoveTimer(timerid);
  timerid = 0;
  animating = NULL;
  ani_idx = 0;
}

MouseCursors::MouseCursors()
{
  // cursor is just a collection of sys and bmp cursors. animated cursors
  // have a different system
  syscursors = new SDL_Cursor*[NUM_SYS_CURSORS];
	syscursors[CURSOR_ARROW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	syscursors[CURSOR_IBEAM] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	syscursors[CURSOR_WAIT] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
	syscursors[CURSOR_CROSSHAIR] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
	syscursors[CURSOR_WAITARROW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
	syscursors[CURSOR_SIZENWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	syscursors[CURSOR_SIZENESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	syscursors[CURSOR_SIZEWE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
	syscursors[CURSOR_SIZENS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	syscursors[CURSOR_SIZEALL] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	syscursors[CURSOR_NO] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
	syscursors[CURSOR_HAND] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

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
  };

  bca = new BmpCursorAni[NUM_ANI_CURSORS];
  BmpCursorAni *bcap;

// ADD NEW ANIMATIONS LIKE FOLLOWS
  bcap = &bca[GET_ANI_IDX(CURSOR_SMRPG_COIN)];
  bcap->hotspot = {8, 6};
  bcap->num_frames = 8;
  bcap->frames = new BmpCursorAniFrame[bcap->num_frames]
  { // opportunity to optimize out the basename string
    { "smrpg-smallcoinani1", 40 },
    { "smrpg-smallcoinani2", 40 },
    { "smrpg-smallcoinani3", 40 },
    { "smrpg-smallcoinani4", 40 },
    { "smrpg-smallcoinani5", 40 },
    { "smrpg-smallcoinani6", 40 },
    { "smrpg-smallcoinani7", 40 },
    { "smrpg-smallcoinani8", 40 },
  };

  load_bmp();
  load_ani();
}

MouseCursors::~MouseCursors() {
	DEBUGLOG("~Cursors\n");
	for (int i=0; i < NUM_SYS_CURSORS; i++)
    if (syscursors[i]) {
      SDL_FreeCursor(syscursors[i]);
      syscursors[i] = NULL;
    }

  BmpCursorAni::stop();

  delete[] bca;
  delete[] bci;
  delete[] syscursors;
}

void MouseCursors::load_ani()
{
  char tb[260];
  int len;
  assert(::file_system);
  // quoted data path does not play nice here
  strcpy(tb, ::file_system->data_path);
  len = strlen(tb);

  for (int i=0; i < NUM_ANI_CURSORS; i++)
  {
    for (int j=0; j < bca[i].num_frames; j++)
    {
      BmpCursorAniFrame *f = &bca[i].frames[j];
      tb[len] = 0;
      strcat(tb, f->filename);
      strcat(tb, ".bmp");
      //strcat(tb, "\"");
      f->surface = SDL_LoadBMP(tb);
      // hard coded color key should be pulled as a data member later
      SDL_SetColorKey(f->surface, SDL_TRUE, SDL_MapRGB(f->surface->format, 0, 0xff, 0));
      DEBUGLOG("path = %s\n", tb);
      if (!f->surface)
      {
        DEBUGLOG("SURFACE: %s\n", SDL_GetError());
        bca[i].loaded = false;
        continue;
      }

      f->cursor = SDL_CreateColorCursor(f->surface,
          bca[i].hotspot.x, bca[i].hotspot.y);

      if (!f->cursor)
      {
        if (f->surface)
        {
          SDL_FreeSurface(f->surface);
          f->surface = NULL;
        }
        bca[i].loaded = false;
        continue;
      }
      else
        bca[i].loaded = true;
    }
  }
}

void MouseCursors::load_bmp()
{
  char tb[260];
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
      bci[i].loaded = false;
      continue;
    }
    SDL_Cursor **c = &bci[i].cursor;
    *c = SDL_CreateColorCursor(bci[i].surface,
            bci[i].hotspot.x, bci[i].hotspot.y);
    if (!*c)
    {
      if(bci[i].surface)
      {
        SDL_FreeSurface(bci[i].surface);
        bci[i].surface = NULL;
      }
      bci[i].loaded = false;
      continue;
    }
    bci[i].loaded = true;
  }
}

void MouseCursors::set_cursor(int i)
{
  // IF NECESSARY, could turn this func into a switch so that index
  // assignment goes at end of func (if you need to use index)
  index = i;

  if (i < NUM_SYS_CURSORS)
  {
    // just set the cursor. but before that, we need check if we were
    // animating a cursor, and if so, stop the timer and cancel the
    // variablle
    BmpCursorAni::stop();
    SDL_SetCursor(syscursors[i]);
  }
  else if (i >= CURSOR_BMP_START && i < CURSOR_BMP_END)
  {
    BmpCursorAni::stop();
    SDL_SetCursor(bci[GET_BMP_IDX(i)].cursor);
  }
  else if (i >= CURSOR_ANI_START && i < CURSOR_ANI_END)
  {
    // get the BmpCursorAni related to this index
    BmpCursorAni::set_cursor(&bca[GET_ANI_IDX(i)]);
  }
}

void MouseCursors::next()
{
	if (++index >= NUM_CURSORS)
		index = 0;
  set_cursor(index);
}

void MouseCursors::prev()
{
	if (--index < 0)
		index = NUM_CURSORS - 1;
  set_cursor(index);
}
