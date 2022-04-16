#pragma once
#include "DEBUGLOG.h"
#include "SDL.h"
#include "colorkey.h"
#include "shared/Texture.h"

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
  CURSOR_YOSHI_TARGET,
  CURSOR_ANI_END,
  NUM_CURSORS=CURSOR_ANI_END
};

/////////////////////////////////////////////////////////////////////////
struct BmpCursor
{
  ~BmpCursor();
  static void load_bmp(BmpCursor *b);
  Colorkey colorkey;
  SDL_Point hotspot;
  const char *filename;
  SDL_Surface *surface;
  SDL_Cursor *cursor;
  bool loaded;// not yet impl'd :( would help bug-free runtime if for some reason a cursor failed to load
};
/////////////////////////////////////////////////////////////////////////
struct BmpCursorAniFrame
{
  ~BmpCursorAniFrame();
  Colorkey colorkey;
  const char *filename;
  int delay; // you can specify a per-frame delay :)
  SDL_Surface *surface;
  SDL_Cursor *cursor;
};

struct BmpCursorAni
{
  ~BmpCursorAni();
  static void load_bmps(BmpCursorAni *a);
  static void set_cursor(BmpCursorAni *b);
  static int handle_event(const SDL_Event &ev);
  
  static void stop(); /* stop animating. Used when changing the cursor from external
  API (MouseCursors::set_cursor), and also from destructor code, precluding object deletion */

  /* Use one hotspot for the whole animation. It would be possible to
   * have custom hotspot per animation frame, but I imagine that kind of
   * moving hotspot would be terrible to use! So we use one. */
  SDL_Point hotspot;
  int num_frames;
  bool loaded; // not yet impl'd :( would help bug-free runtime if for some reason a cursor failed to load

  BmpCursorAniFrame *frames;
private:
  /* called from the custom UserEvent "mouse_ani", this takes an index
  * into actively animating BmpCursorAni, not to be confused as an index into the
  * all-encompassing cursor enum found in Cursors.cpp. */
  static void set_frame(int i);
 
 /* Sends a custom SDL User event to update the cursor from the main
   * thread */
  static Uint32 push_cursor_ani_update_event(Uint32 interval=0, void *param=NULL);
  static int ani_idx;
  static Uint32 timerid; /* the SDL timer id for the active animated
                            cursor, 0 if no timer is active */
  static BmpCursorAni *animating; /* The currently animating cursor,
                                     if any. NULL when there is none */
};
/////////////////////////////////////////////////////////////////////////

struct MouseTextureAni : public TextureAni
{
  ~MouseTextureAni();
  static int handle_event (const SDL_Event &ev);
  static void stop();
  static void draw();
  static MouseTextureAni *selected;
  private:
  static void set_ani(MouseTextureAni *b);
  static Uint32 update_ani_idx(Uint32 interval, void *param);
  static Uint32 timerid;
  static int ani_idx;
  static MouseTextureAni *animating;
};

/////////////////////////////////////////////////////////////////////////

/* SIDE NOT IDEA!!! When the tracker opens, a rotating star sprite
 * horizontally traverses the screen, drizzling sparkles that fall down
 * the screen in its trail/wake. */

struct MouseCursors
{
	MouseCursors();
	~MouseCursors();

	void set_cursor(int i);
	void next();
	void prev();

  void handle_event(const SDL_Event &ev);
  void draw_aux();

private:
  SDL_Cursor **syscursors;
  int index=0;
  BmpCursor *bci;
  BmpCursorAni *bca;

  MouseTextureAni *mcaa;
};
