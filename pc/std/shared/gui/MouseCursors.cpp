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

TextureAni * TextureAni::animating, *TextureAni::selected;
Uint32 TextureAni::timerid;
int TextureAni::ani_idx;
//SDL_Mutex *TextureAni::mutex;

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
          set_frame((int)ev.user.data1);
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
  bcap->hotspot = {4, 5};
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

  mcaa = new TextureAni[1];
  TextureAni *mcaap;
  mcaap = &mcaa[0];
  mcaap->num_textures = 3;
  mcaap->texture = new Texture[mcaap->num_textures] {
    {"sparkle-big", 5, 5},
    {"sparkle-med", 3, 3},
    {"sparkle-small", 1, 1},
  };
  Texture::load_bmp(mcaap->texture, mcaap->texture->filename, ::render->sdlRenderer);
  Texture::load_bmp(mcaap->texture+1, mcaap->texture->filename, ::render->sdlRenderer);
  Texture::load_bmp(mcaap->texture+2, mcaap->texture->filename, ::render->sdlRenderer);

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
    mcaap->frames[i].coord.x += -162 - (bca[GET_ANI_IDX(CURSOR_SMRPG_COIN)].hotspot.x - 2);
    mcaap->frames[i].coord.y += -67 - bca[GET_ANI_IDX(CURSOR_SMRPG_COIN)].hotspot.y + 2;
  }

  TextureAni::selected = mcaap;

  load_bmp();
  load_ani();

  set_cursor(CURSOR_SMRPG_COIN);
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
    TextureAni *selected = TextureAni::selected;
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

int MouseCursors::handle_event(const SDL_Event &ev)
{
  BmpCursorAni::handle_event(ev);
  TextureAni::handle_event(ev);
}


//////////////////////// START TEXTUREANI ///////////////////////////////
TextureAni::~TextureAni()
{
  stop();
  if (texture)
    delete[] texture;
  texture = NULL;
  if (frames)
    delete[] frames;
  frames = NULL;
}

int TextureAni::handle_event(const SDL_Event &ev)
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

void TextureAni::stop()
{
  if (timerid)
    SDL_RemoveTimer(timerid);
  timerid = 0;
  animating = NULL;
  ani_idx = 0;
}

void TextureAni::set_ani(TextureAni *b)
{
  TextureAni **aa = &animating;
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

Uint32 TextureAni::update_ani_idx(Uint32 interval, void *param)
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

void TextureAni::draw()
{
  TextureAni *aa = animating;
  if (aa)
  {
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
        mouse::x + tf->coord.x,
        mouse::y + tf->coord.y,
        t->w, t->h
      };
      SDL_RenderCopy(::render->sdlRenderer, t->sdltexture, NULL, &dr);
    }
  }
}
////////////////////////////////////////////////////////////////////
void MouseCursors::draw_aux()
{
  TextureAni::draw();
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


//////////////////////// START TEXTURE CODE ///////////////////////////////

/*Texture::Texture() : surface(NULL), texture(NULL)
{
}*/
Texture::~Texture()
{
  if (sdltexture)
  {
    SDL_DestroyTexture(sdltexture);
    sdltexture = NULL;
  }
  if (surface)
  {
    SDL_FreeSurface(surface);
    surface = NULL;
  }
}

void Texture::load_bmp(Texture *t, const char *filename, SDL_Renderer *r)
{
  char path[260];
  assert(::file_system);
  // quoted data path does not play nice here
  strcpy(path, ::file_system->data_path);
  strcat(path, filename);
  strcat(path, ".bmp");

  t->surface = SDL_LoadBMP(path);
  if (t->surface == NULL)
  {
    printf("couldn't load %s: %s\n", path, SDL_GetError());
  }

  SDL_SetColorKey(t->surface, SDL_TRUE, SDL_MapRGB(t->surface->format, 0, 0xff, 0));

  t->sdltexture = SDL_CreateTextureFromSurface( r, t->surface);
  if (t->sdltexture == NULL)
  {
    printf("Unabled to create texture from %s: %s\n",
        path, SDL_GetError());
  }
}

void Texture::queryXY(Texture *t)
{
  int r = SDL_QueryTexture(t->sdltexture, NULL, NULL, &t->w, &t->h);
  if (r < 0)
    DEBUGLOG("couldn't query texture \"%s\": %s\n", t->filename, SDL_GetError());
}

//////////////////////// END TEXTURE CODE ///////////////////////////////
