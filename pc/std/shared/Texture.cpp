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
