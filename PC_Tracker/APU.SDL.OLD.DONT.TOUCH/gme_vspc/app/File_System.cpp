#include "File_System.h"

File_System::~File_System()
{
  if (pref_path)
    SDL_free(pref_path);
  if (data_path)
    SDL_free(data_path);
}
int File_System::init()
{
  is_loaded=true;
  char tmpbuf[100];
  char *base_path = SDL_GetBasePath();
  if (base_path)
  {
      data_path = base_path;
      //SDL_free(base_path);
  } 
  else 
  {
    data_path = SDL_strdup("./");
  }
  fprintf(stderr, "data_path = %s\n", data_path);

  // Turn the company name / App name to underlined' strings [no spaces]
  char *c = SDL_strdup(COMPANY_NAME);
  char *a = SDL_strdup(APP_NAME);

  char *p = c;
  for (int i=0; i < strlen(COMPANY_NAME); i++)
  {
    if (*p == ' ')
      *p = '_';
    p++;
  }
  p = a;
  for (int i=0; i < strlen(APP_NAME); i++)
  {
    if (*p == ' ')
      *p = '_';
    p++;
  }
  char *pref_pathp = SDL_GetPrefPath(c, a);
  SDL_free(c);
  SDL_free(a);
  if (pref_pathp)
  {
      //pref_path = SDL_strdup(pref_pathp);
    pref_path = pref_pathp;
    is_writing_enabled=true;
  } 
  else 
  {
    pref_path = SDL_strdup("./");
    is_writing_enabled=true;
    sprintf(tmpbuf, "SDL Was unable to provide me a preferences directory. "
       "No preferences will be saved!\n"
       "I blame it all on SDL!!!\n %s", SDL_GetError() );
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                       "Warning!",
                       tmpbuf,
                       NULL);
    return -1;
  }

  fprintf(stderr, "pref_path = %s\n", pref_path);

  return 0;
}


