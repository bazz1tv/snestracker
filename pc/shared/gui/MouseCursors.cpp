#include "MouseCursors.h"
#include "shared/File_System.h"
#include "shared/Render.h"
#include "sdl_userevents.h"
#include "mouse.h"

/* These are animated cursor variables that only 1 instance is needed of.
 * Will be used for any specific animated cursor that becomes active */
BmpCursorAni * BmpCursorAni::animating = NULL;
Uint32 BmpCursorAni::timerid = 0;
int BmpCursorAni::ani_idx;

MouseTextureAni * MouseTextureAni::animating, *MouseTextureAni::selected;
Uint32 MouseTextureAni::timerid;
int MouseTextureAni::ani_idx;

#define NUM_BMP_CURSORS (NUM_SYS_AND_BMP_CURSORS - CURSOR_BMP_START)
#define NUM_ANI_CURSORS (NUM_CURSORS - CURSOR_ANI_START)
#define NUM_BMP_AND_ANI_CURSORS (NUM_BMP_CURSORS + NUM_ANI_CURSORS)
//#define BMP_CURSORS_ANI_START_IDX (CURSOR_ANI_START - CURSOR_BMP_START)
#define GET_BMP_IDX(x) (x - CURSOR_BMP_START)
#define GET_ANI_IDX(x) (x - CURSOR_ANI_START)

#define IS_BMP(x) (x >= CURSOR_BMP_START && x < CURSOR_BMP_END)
#define IS_ANI(x) (x >= CURSOR_ANI_START && x < CURSOR_ANI_END)

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
  stop();
  delete[] frames;
}

void BmpCursorAni::set_cursor(BmpCursorAni *b)
{
  BmpCursorAni **aa = &animating;
  // is there already a cursor animating?
  if (*aa == b)
    return;

  stop();
  // start the first frame
  *aa = b;
  /* Since SDL_SetCursor is called from this function, this function must
   * be called from the main thread, which fortunately is the case right
   * now. If not, this function could easily be modified to get the timer
   * callback to be the start of the animation. (call it with virtually no
   * delay and index of 0 - 1*/
  SDL_SetCursor(b->frames[0].cursor);
  timerid = SDL_AddTimer(b->frames[0].delay,
      &push_cursor_ani_update_event,
      &ani_idx);

}

int BmpCursorAni::handle_event(const SDL_Event &ev)
{
  switch (ev.type)
  {
    case SDL_USEREVENT:
    {
      switch (ev.user.code)
      {
        case UserEvents::mouse_ani:
          set_frame((int)(intptr_t)ev.user.data1);
          return 1;
      }
    } break;
    default:break;
  }
  return 0;
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
    { {0x00ff00}, {8, 8}, "cursor" },
    { {0x00ff00}, {8, 8}, "cursor-rec" },
    { {0x00ff00}, {7, 7}, "cursor-kirbystar" },
    { {0x00ff00}, {5, 5}, "cursor-mariokart-1up" },
    { {0x00ff00}, {5, 5}, "cursor-mariokart-mushroom" },
    { {0x00ff00}, {5, 5}, "cursor-mariokart-bluemushroom" },
    { {0x00ff00}, {5, 5}, "cursor-mariokart-yellowmushroom" },
    { {0x00ff00}, {8, 8}, "thing" },
    { {0x00ff00}, {8, 8}, "thing2" },
    { {0x00ff00}, {6, 7}, "mpaint-mario" },
    { {0x00ff00}, {7, 8}, "mpaint-froggy" },
    { {0x00ff00}, {1, 1}, "mpaint-golden-hand" },
    { {0x00ff00}, {1, 1}, "mpaint-white-hand" },
    { {0x00ff00}, {1, 1}, "cursor-zsnes" },
    { {0x00ff00}, {1, 1}, "cursor-zsnes2" },
  };

  bca = new BmpCursorAni[NUM_ANI_CURSORS];
  BmpCursorAni *bcap;

// ADD NEW ANIMATIONS LIKE FOLLOWS
  bcap = &bca[GET_ANI_IDX(CURSOR_SMRPG_COIN)];
  bcap->hotspot = {4, 5};
  bcap->num_frames = 8;
  bcap->frames = new BmpCursorAniFrame[bcap->num_frames]
  { // opportunity to optimize out the basename string
    { {0x00ff00}, "smrpg-smallcoinani1", 40 },
    { {0x00ff00}, "smrpg-smallcoinani2", 40 },
    { {0x00ff00}, "smrpg-smallcoinani3", 40 },
    { {0x00ff00}, "smrpg-smallcoinani4", 40 },
    { {0x00ff00}, "smrpg-smallcoinani5", 40 },
    { {0x00ff00}, "smrpg-smallcoinani6", 40 },
    { {0x00ff00}, "smrpg-smallcoinani7", 40 },
    { {0x00ff00}, "smrpg-smallcoinani8", 40 },
  };

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
  bcap = &bca[GET_ANI_IDX(CURSOR_YOSHI_TARGET)];
  bcap->hotspot = {8, 8};
  bcap->num_frames = 2;
  bcap->frames = new BmpCursorAniFrame[bcap->num_frames]
  { // opportunity to optimize out the basename string
    { {0x00ff00}, "cursor", 100 },
      { {0x00ff00}, "cursor-rec", 100 },
  };
////////////////// END ANIMATED CURSORS ////////////////////////


////// START ANIMATED TEXTURE EFFECTS ///////////////////////
  mcaa = new MouseTextureAni[1];
  MouseTextureAni *mcaap;
  mcaap = &mcaa[0];
  mcaap->num_textures = 3;
  mcaap->texture = new Texture[mcaap->num_textures] {
    { {0x00ff00}, "cursors/sparkle-big",    5, 5},
    { {0x00ff00}, "cursors/sparkle-med",    3, 3},
    { {0x00ff00}, "cursors/sparkle-small",  1, 1},
  };
  for (int i=0; i < 3; i++)
  {
    if (
      Texture::load_bmp(&mcaap->texture[i], mcaap->texture[i].filename,
        ::render->sdlRenderer) < 0 )
    {
      mcaap->loaded = false;
      /* should I notify the user or just silently let it go? */
    }
  }

  mcaap->num_frames = 15;
  mcaap->num_sprites = 4;
  mcaap->frametime = 27;
  mcaap->frames = new TextureFrame[mcaap->num_sprites * mcaap->num_frames]
  {
      { mcaap->texture, {162, 69}  },
      { mcaap->texture, {162, 67} },
      { mcaap->texture, {162, 67} },
      { mcaap->texture, {162, 70} },
      { mcaap->texture+1, {169, 67} },
      { mcaap->texture+1, {169, 67} },
      { mcaap->texture+1, {170, 70} },
      { mcaap->texture+1, {170, 69} },
      { mcaap->texture+2, {172, 74} },
      { mcaap->texture+2, {172, 74} },
      { mcaap->texture+2, {173, 80} },
      { mcaap->texture+2, {173, 80} },
      { mcaap->texture, {162, 82} },
      { mcaap->texture, {162, 82} },
      { mcaap->texture, {162, 82} },

      { mcaap->texture, {159, 68}  },
      { mcaap->texture, {158, 65} },
      { mcaap->texture, {158, 65} },
      { mcaap->texture, {157, 66} },
      { mcaap->texture+1, {162, 64} },
      { mcaap->texture+1, {162, 64} },
      { mcaap->texture+1, {162, 67} },
      { mcaap->texture+1, {162, 66} },
      { mcaap->texture+2, {162, 71} },
      { mcaap->texture+2, {162, 71} },
      { mcaap->texture+2, {162, 77} },
      { mcaap->texture+2, {162, 77} },
      { NULL },
      { NULL },
      { NULL },

      { mcaap->texture,   {165, 68}  },
      { mcaap->texture, {158+8, 65} },
      { mcaap->texture, {158+8, 65} },
      { mcaap->texture, {167, 66} },
      { mcaap->texture+1, {162, 64+8} },
      { mcaap->texture+1, {162, 64+8} },
      { mcaap->texture+1, {162, 67+8} },
      { mcaap->texture+1, {162, 66+8} },
      { mcaap->texture+2, {162, 71+8} },
      { mcaap->texture+2, {162, 71+8} },
      { mcaap->texture+2, {162, 77+8} },
      { mcaap->texture+2, {162, 77+8} },
      { NULL },
      { NULL },
      { NULL },

      { mcaap->texture, {162, 67} },
      { mcaap->texture, {162, 63} },
      { mcaap->texture, {162, 63} },
      { mcaap->texture, {162, 64} },
      { mcaap->texture+1, {155, 67} },
      { mcaap->texture+1, {155, 67} },
      { mcaap->texture+1, {154, 70} },
      { mcaap->texture+1, {154, 69} },
      { mcaap->texture+2, {152, 74} },
      { mcaap->texture+2, {152, 74} },
      { mcaap->texture+2, {151, 80} },
      { mcaap->texture+2, {151, 80} },
      { NULL },
      { NULL },
      { NULL },
  };

  for (int i=0; i < (mcaap->num_sprites * mcaap->num_frames); i++)
  {
    mcaap->frames[i].coord.x += -162 - (bca[GET_ANI_IDX(CURSOR_SMRPG_COIN)].hotspot.x - 2) + 1;
    mcaap->frames[i].coord.y += -67 - bca[GET_ANI_IDX(CURSOR_SMRPG_COIN)].hotspot.y + 2;
  }

  MouseTextureAni::selected = mcaap;

  //load_bmp();
  for (int i=0; i < NUM_BMP_CURSORS; i++)
    BmpCursor::load_bmp(&bci[i]);
  for (int i=0; i < NUM_ANI_CURSORS; i++)
    BmpCursorAni::load_bmps(&bca[i]);
}

MouseCursors::~MouseCursors() {
	DEBUGLOG("~Cursors\n");
	for (int i=0; i < NUM_SYS_CURSORS; i++)
    if (syscursors[i]) {
      SDL_FreeCursor(syscursors[i]);
      syscursors[i] = NULL;
    }

  delete[] mcaa;
  delete[] bca;
  delete[] bci;
  delete[] syscursors;
}

void BmpCursorAni::load_bmps(BmpCursorAni *a)
{
  char tb[260];
  int len;
  assert(::file_system);
  // quoted data path does not play nice here
  strcpy(tb, ::file_system->data_path);
  strcat(tb, "cursors/");
  len = strlen(tb);

    for (int j=0; j < a->num_frames; j++)
    {
      BmpCursorAniFrame *f = &a->frames[j];
      tb[len] = 0;
      strcat(tb, f->filename);
      strcat(tb, ".bmp");
      //strcat(tb, "\"");
      DEBUGLOG("path = %s\n", tb);
      
      f->surface = SDL_LoadBMP(tb);
      if (!f->surface)
      {
        DEBUGLOG("SURFACE: %s\n", SDL_GetError());
        a->loaded = false;
        return;
      }
      // hard coded color key should be pulled as a data member later
      SDL_SetColorKey(f->surface, SDL_TRUE,
        SDL_MapRGB(f->surface->format, f->colorkey.r, f->colorkey.g,
                   f->colorkey.b));

      f->cursor = SDL_CreateColorCursor(f->surface,
          a->hotspot.x, a->hotspot.y);

      if (!f->cursor)
      {
        if (f->surface)
        {
          SDL_FreeSurface(f->surface);
          f->surface = NULL;
        }
        a->loaded = false;
        return;
      }
      else
        a->loaded = true;
    }
}

void BmpCursor::load_bmp(BmpCursor *b)
{
  char tb[260];
  int len;
  assert(::file_system);
  // quoted data path does not play nice here
  strcpy(tb, ::file_system->data_path);
  strcat(tb, "cursors/");
  len = strlen(tb);

    tb[len] = 0;
    strcat(tb, b->filename);
    strcat(tb, ".bmp");
    //strcat(tb, "\"");
    b->surface = SDL_LoadBMP(tb);

    DEBUGLOG("path = %s\n", tb);
    if (!b->surface)
    {
      DEBUGLOG("\tcouldn't load, '%s'\n", SDL_GetError());
      b->loaded = false;
      return;
    }

    SDL_SetColorKey(b->surface, SDL_TRUE,
        SDL_MapRGB(b->surface->format,
                    b->colorkey.r, b->colorkey.g, b->colorkey.b));

    
    SDL_Cursor **c = &b->cursor;
    *c = SDL_CreateColorCursor(b->surface,
            b->hotspot.x, b->hotspot.y);
    if (!*c)
    {
      if(b->surface)
      {
        SDL_FreeSurface(b->surface);
        b->surface = NULL;
      }
      b->loaded = false;
      DEBUGLOG("\tcouldn't create cursor, '%s'\n", SDL_GetError());
      return;
    }
    b->loaded = true;
}

void MouseCursors::set_cursor(int i)
{
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

  if (i >= CURSOR_BMP_START && i < CURSOR_ANI_END && index >= CURSOR_BMP_START && index < CURSOR_ANI_END)
  {
    MouseTextureAni *selected = MouseTextureAni::selected->loaded ?
                                MouseTextureAni::selected : NULL;
    if (selected)
    {
      int hx, hy, ohx, ohy;

      if (IS_BMP(index))
      {
        ohx = bci[GET_BMP_IDX(index)].hotspot.x;// / 2;
        ohy = bci[GET_BMP_IDX(index)].hotspot.y;// / 2;
      }
      else if (IS_ANI(index))
      {
        ohx = bca[GET_ANI_IDX(index)].hotspot.x;// / 2;
        ohy = bca[GET_ANI_IDX(index)].hotspot.y;// / 2;
      }

      if (IS_BMP(i))
      {
        hx = bci[GET_BMP_IDX(i)].hotspot.x;// / 2;
        hy = bci[GET_BMP_IDX(i)].hotspot.y;// / 2;
      }
      else if (IS_ANI(i))
      {
        hx = bca[GET_ANI_IDX(i)].hotspot.x;// / 2;
        hy = bca[GET_ANI_IDX(i)].hotspot.y;// / 2;
      }

      for (int j=0; j < (selected->num_sprites * selected->num_frames); j++)
      {
        selected->frames[j].coord.x += ohx - hx;
        selected->frames[j].coord.y += ohy - hy;
      }
    }
  }
  index = i;
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

void MouseCursors::handle_event(const SDL_Event &ev)
{
  BmpCursorAni::handle_event(ev);
  MouseTextureAni::handle_event(ev);
}

/////////////////////// MOUSETEXTUREANI START ///////////////////
MouseTextureAni::~MouseTextureAni()
{
  stop();
}

int MouseTextureAni::handle_event(const SDL_Event &ev)
{
  switch (ev.type)
  {
    case SDL_MOUSEBUTTONDOWN:
      {
        // trigger an auxiliary animation. this should later be made
        // conditional.
        /* The animation should re-trigger even if already animating.
         * Anything more complex is TOO much. Little by little buddy */
        //if (rand() % 8 == 0)
      } break;
    case SDL_USEREVENT:
      {
        switch (ev.user.code)
        {
          case UserEvents::mouse_react:
            //if (rand() % 4 == 3)
              set_ani(selected);
            break;
        }
      } break;
    default:break;
  }
  return 0;
}

void MouseTextureAni::stop()
{
  if (timerid)
    SDL_RemoveTimer(timerid);
  timerid = 0;
  animating = NULL;
  ani_idx = 0;
}

void MouseTextureAni::set_ani(MouseTextureAni *b)
{
  MouseTextureAni **aa = &animating;
  /* Dont care if same ani is already runnning. we restart it. */
  /*
  // is there already a cursor animating?
  if (*aa == b)
    return;
  */
  stop();
  // start the first frame
  *aa = b;

  //b->timeout = SDL_GetTicks() + b->frametime;
  timerid = SDL_AddTimer(b->frametime,
      &update_ani_idx,
      &ani_idx);
}

Uint32 MouseTextureAni::update_ani_idx(Uint32 interval, void *param)
{
  if (!animating)
    return 0;
  int *i = (int *)param;
  *i += 1;
  if (*i >= animating->num_frames)
  {
    stop();
    return 0;
  }
  return animating->frametime; //frames[*i].delay;
}

void MouseTextureAni::draw()
{
  MouseTextureAni *aa = animating;
  /* also addressing an SDL bug where if the program is opened and the
   * mouse is in the window but hasn't been moved yet, the coords are
   * reported as (0,0) from SDL_GetMouseState. So, let's just not render
   * the animation until the mouse has been moved. Note that this bug also
   * causes the coordinates of normal MOUSEBUTTON{DOWN,UP} events to
   * misleadingly believe a click is happening at (0,0) before the mouse
   * is moved. Not a big deal... */
  if (!aa || (!mouse::x && !mouse::y))
    return;

  /*Uint32 now = SDL_GetTicks();
  if (SDL_TICKS_PASSED(now, aa->timeout))
  {
    ani_idx++;
    if (ani_idx >= animating->num_frames)
    {
      stop();
      return;
    }

    aa->timeout = now + aa->frametime;
  }*/
  for (int i=0; i < aa->num_sprites; i++)
  {
    TextureFrame *tf = &aa->frames[(aa->num_frames * i) + ani_idx];
    Texture *t = tf->texture;
    if (!t) continue;
    SDL_Rect dr {
      mouse::prescaled_x + static_cast<int> ( (float) ( tf->coord.x * 1) ),
      mouse::prescaled_y + static_cast<int> ( (float) ( tf->coord.y * 1) ),
      t->w, t->h
    };
    SDL_RenderCopy(::render->sdlRenderer, t->sdltexture, NULL, &dr);
  }
}
////////////////////////////////////////////////////////////////////
void MouseCursors::draw_aux()
{
  MouseTextureAni::draw();
  // Here, we should check if there is a cursor with auxiliary drawing
  // feature enabled. In that case, the only feature is going to be a
  // mouse click animation triggered from a mouse button down left click
  // event (it's sounds so specific!! lol) but yeah... so let's do that!
  /* Since it's not coming to mind immediately, let's just assume to the
   * effect is enabled, and code it in to get it working! */

  // OK pretend we checked if the feature is enabled for this cursor. and
  // now let's draw that animation effect!
  //
  // Assume the animation has already been started. We never have to start
  // the animation from here. But we need to check that it's been
  // triggered. Could copy the cursor ani code, like checking for a
  // "animating" != NULL
  //if (aux_animating != NULL)
    // in the case we are animating, we're in the main thread so we can
    // just.. oh wait.. this is not going to use SDL_SetCursor because we
    // want to overlay the existing cursor (at least in this case..) So
    // ..? we are going to blit to the renderer the current animation
    // frame. Since there is no process function or (run), we need to
    // update the animation frame HERE before drawing
    
    /* PLOT TWIST: this function doesn't need to be called every time from
     * the main game loop, but can simply be invoked from the timer as a
     * UserEvent. :) . Another plot twist, it DOES need to be called from
     * the game loop to ensure that it is drawn LAST. and the MainWindow
     * and other such windows should NOT be calling SDL_RenderPresent, but
     * rather the game loop owner should do it. The Window is not the end
     * all be all, the Tracker owner is~!*/
}
