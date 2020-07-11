#include "RecentFiles.h"
#include "SDL.h"

char * RecentFiles::paths[NUM_RECENTFILES];

void RecentFiles::init()
{
  for (int i = 0; i < NUM_RECENTFILES; i++)
  {
    paths[i] = NULL;
  }
}

void RecentFiles::deinit()
{
  for (int i=0; i < NUM_RECENTFILES; i++)
  {
    if (paths[i] != NULL)
    {
      SDL_free(paths[i]);
      paths[i] = NULL;
    }
  }
}

void RecentFiles::push(const char *path)
{
  fprintf(stderr, "RecentFiles::push()!\n");
	/* First, check for existence */
  int match_idx = -1;
  bool match = false;
	for (int i=0; i < NUM_RECENTFILES; i++)
	{
    if (paths[i] && strcmp(path, paths[i]) == 0)
    {
      match = true;
      match_idx = i;
      break;
    }
	}

	// 1. load file already in recent list
  //// 1a. loaded the most recent file, which case do nothing
  if (match_idx == 0)
  {
    fprintf(stderr, "\tReturning early\n");
    return;
  }
  //// 1b. loaded another file,
  else if (match_idx > 0)
  {
    fprintf(stderr, "\tmatch_idx > 0\n");
    ////// place file in first slot
    ////// move files thru the list
    char *strPtr = paths[match_idx];

    for (int i = match_idx - 1; i >= 0; i--)
    {
      //fprintf(stderr,"%d\n",i);
      paths[i+1] = paths[i];
    }
    paths[0] = strPtr;
  }
  else if (match_idx < 0)
  {
    fprintf(stderr, "\tmatch_idx < 0\n");
    // 2. loaded file not yet in list
    //add to list

    /* If the last item is not NULL, free it */
    if (paths[NUM_RECENTFILES - 1] != NULL)
    {
      SDL_free(paths[NUM_RECENTFILES]);
    }

    // move everything up the list
    for (int i = NUM_RECENTFILES - 1; i > 0; i--)
    {
      paths[i] = paths[i-1];
    }

    char *newstr = (char *) SDL_malloc( sizeof(char) * ( strlen(path) + 1) );
    strcpy(newstr, path);
    paths[0] = newstr;
  }
}