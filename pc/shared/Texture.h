#pragma once
#include "SDL.h"
#include "colorkey.h"

struct Texture
{
  //Texture();
  ~Texture();
  static int load_bmp(Texture *t, const char *filename, SDL_Renderer *r);
  // you may optionally query the width and height from SDL
  static void queryXY(Texture *t);

  Colorkey colorkey;

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
  bool loaded;
  int num_frames;
  int num_sprites;
  int num_textures;
  int frametime; // in ms
  Texture *texture; // this will be pluralized
  TextureFrame *frames;
};
