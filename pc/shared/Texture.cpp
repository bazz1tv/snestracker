#include "Texture.h"
#include "DEBUGLOG.h"
#include <assert.h>
#include "File_System.h"
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

/* IN: an uninitialized texture pointer */
int Texture::load_bmp(Texture *t, const char *filename, SDL_Renderer *r)
{
  char path[260];
  assert(::file_system);
  // quoted data path does not play nice here
  strcpy(path, ::file_system->data_path);
  strcat(path, filename);
  strcat(path, ".bmp");

  DEBUGLOG("loading texture: %s\n", path);

  t->surface = SDL_LoadBMP(path);
  if (t->surface == NULL)
  {
    DEBUGLOG("\tcouldn't load, '%s'\n", SDL_GetError());
    return -1;
  }

  SDL_SetColorKey(t->surface, SDL_TRUE, SDL_MapRGB(t->surface->format,
                  t->colorkey.r, t->colorkey.g, t->colorkey.b));

  t->sdltexture = SDL_CreateTextureFromSurface( r, t->surface);
  if (t->sdltexture == NULL)
  {
    DEBUGLOG("\tUnable to create texture, '%s'\n", SDL_GetError());
    return -2;
  }

  return 0;
}

/* untested */
void Texture::queryXY(Texture *t)
{
  int r = SDL_QueryTexture(t->sdltexture, NULL, NULL, &t->w, &t->h);
  if (r < 0)
    DEBUGLOG("couldn't query texture \"%s\": %s\n", t->filename, SDL_GetError());
}

//////////////////////// END TEXTURE CODE ///////////////////////////////
//////////////////////// START TEXTUREANI ///////////////////////////////
TextureAni::~TextureAni()
{
  if (texture)
    delete[] texture;
  texture = NULL;
  if (frames)
    delete[] frames;
  frames = NULL;
}
