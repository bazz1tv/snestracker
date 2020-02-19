#pragma once
#include "DEBUGLOG.h"
#include "SDL.h"

/////////////////////////////////////////////////////////////////////////
struct BmpCursor
{
  ~BmpCursor();
  SDL_Point hotspot;
  const char *filename;
  SDL_Surface *surface;
  SDL_Cursor *cursor;
  bool loaded;
};

struct BmpCursorAniFrame
{
  ~BmpCursorAniFrame();
  const char *filename;
  int delay; // you can specify a per-frame delay :)
  SDL_Surface *surface;
  SDL_Cursor *cursor;
};

struct BmpCursorAni
{
  ~BmpCursorAni();
  static void set_cursor(BmpCursorAni *b);
  static int handle_event(const SDL_Event &ev);
  /* Sends a custom SDL User event to update the cursor from the main
   * thread */
  static Uint32 push_cursor_ani_update_event(Uint32 interval=0, void *param=NULL);

  /* called from the custom UserEvent "mouse_ani", this takes an index
   * into actively animating BmpCursorAni, not to be confused as an index into the
   * all-encompassing cursor enum found in Cursors.cpp. */
  static void set_frame(int i);
  
  static void stop(); /* stop animating. Used when changing the cursor from external
  API (set_cursor), and also from destructor code, precluding object deletion */

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

  static int ani_idx;
  BmpCursorAniFrame *frames;
};
/////////////////////////////////////////////////////////////////////////

/* An animation might require a collection of animations to occur
 * simultaneously. In this case we need to rethink the system. But for
 * now, let's just get one animation working. To get multiple working, we
 * can make a lot of those static variable instance variables, and have
 * those particularly instance-useful static functions take class object
 * as an argument, or just make it an instance function, duh */

struct Texture
{
  //Texture();
  ~Texture();
  static void load_bmp(Texture *t, const char *filename, SDL_Renderer *r);
  // you may optionally query the width and height from SDL
  static void queryXY(Texture *t);

  const char *filename; // no extension, e.g "filename"
  int w,h;
  SDL_Surface *surface; // keep the surface just in case
  SDL_Texture *sdltexture;
};

struct TextureFrame
{
  Texture *texture;
  SDL_Point coord; //relative coordinate from mouse
};

struct TextureAni
{
  ~TextureAni();
  //bool loaded;
  int num_frames;
  int num_sprites;
  int num_textures;
  int frametime; // in ms
  Texture *texture; // this will be pluralized
  TextureFrame *frames;
  Uint32 timeout;
  //static SDL_Mutex *mutex;
  static int handle_event (const SDL_Event &ev);
  static void stop();
  static void set_ani(TextureAni *b);
  static Uint32 update_ani_idx(Uint32 interval, void *param);
  static void draw();
  static TextureAni *animating, *selected;
  static Uint32 timerid;
  static int ani_idx;
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

  int handle_event(const SDL_Event &ev);
  void draw_aux();

  SDL_Cursor **syscursors;
  int index=0;

  BmpCursor *bci;
  BmpCursorAni *bca;

  TextureAni *mcaa;

private:
  void load_bmp();
  void load_ani();
};
