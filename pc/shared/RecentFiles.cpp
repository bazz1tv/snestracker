#include "RecentFiles.h"
#include "SDL.h"
#include "utility.h"
#include "font.h"
#include "DEBUGLOG.h"
#include <assert.h>

char * RecentFiles::paths[NUM_RECENTFILES];
char * RecentFiles::dnames[NUM_RECENTFILES];

void RecentFiles::init()
{
  for (int i = 0; i < NUM_RECENTFILES; i++)
  {
    paths[i] = NULL;
    dnames[i] = NULL;
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

      SDL_free(dnames[i]);
      dnames[i] = NULL;
    }
  }
}

void RecentFiles::remove(int idx)
{
  if (paths[idx])
    SDL_free(paths[idx]);
  if (dnames[idx])
    SDL_free(dnames[idx]);

  for (int i = idx; i < (NUM_RECENTFILES - 1); i++)
  {
    paths[i] = paths[i+1];
    dnames[i] = dnames[i+1];
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
  //// 1b. loaded another file already in the list,
  else if (match_idx > 0)
  {
    fprintf(stderr, "\tmatch_idx > 0\n");
    ////// place file in first slot
    ////// move files thru the list
    char *pathPtr = paths[match_idx];
    char *dnamePtr = dnames[match_idx];

    for (int i = match_idx - 1; i >= 0; i--)
    {
      //fprintf(stderr,"%d\n",i);
      paths[i+1] = paths[i];
      dnames[i+1] = dnames[i];
    }
    paths[0] = pathPtr;
    dnames[0] = dnamePtr;
  }
  else if (match_idx < 0)
  {
    fprintf(stderr, "\tmatch_idx < 0\n");
    // 2. loaded file not yet in list
    //add to list

    /* If the last item is not NULL, free it */
    if (paths[NUM_RECENTFILES - 1] != NULL)
    {
      const char *p = paths[NUM_RECENTFILES - 1];
      const char *d = dnames[NUM_RECENTFILES - 1];
      SDL_free(dnames[NUM_RECENTFILES - 1]);
      SDL_free(paths[NUM_RECENTFILES - 1]);
      //paths[NUM_RECENTFILES - 1] = NULL;
    }

    // move everything up the list
    for (int i = NUM_RECENTFILES - 1; i > 0; i--)
    {
      paths[i] = paths[i-1];
      dnames[i] = dnames[i - 1];
    }

    paths[0] = (char *) SDL_malloc( sizeof(char) * ( strlen(path) + 1) );
    strcpy(paths[0], path);

    char *filename = (char *) Utility::getFileName(paths[0]);
    dnames[0] = (char *) SDL_malloc( sizeof(char) * ( strlen(filename) + 1 + 1) );
    strcpy(dnames[0], FONT_BULLETPOINT_STR);
    strcat(dnames[0], filename);

    /* Now, check if there are entries with the same filename but in different folders */
    bool change = false;
    for (int i=1; i < ( NUM_RECENTFILES ); i++)
    {
        if (dnames[i] && strcmp(dnames[0], dnames[i]) == 0)
        {
          char *dirname = Utility::getDirectoryName(paths[i]);
          char *dirfile = (char *) SDL_malloc( 
            sizeof(char) * ( strlen(dirname) + sizeof(" -- - ") + strlen(dnames[i] ) + 1 ) );

          strcpy(dirfile, dnames[i]);
          strcat(dirfile, " " FONT_LONGDASH_STR FONT_LONGDASH_STR " " FONT_FOLDER_STR);
          strcat(dirfile, dirname);

          if (dnames[i] != NULL)
            SDL_free(dnames[NUM_RECENTFILES - 1]);

          dnames[i] = dirfile;
          change = true;
        }
    }
    if ( change )
    {
      char *dirname = Utility::getDirectoryName(paths[0]);
      char *dirfile = (char *) SDL_malloc( 
        sizeof(char) * ( strlen(dirname) + sizeof(" -- - ") + strlen(dnames[0] ) + 1 ) );

      strcpy(dirfile, dnames[0]);
      strcat(dirfile, " " FONT_LONGDASH_STR FONT_LONGDASH_STR " " FONT_FOLDER_STR);
      strcat(dirfile, dirname);

      dnames[0] = dirfile;
    }
  }
}

void RecentFiles::createDisplayNames()
{
  for (int i=0; i < NUM_RECENTFILES; i++)
  {
    DEBUGLOG("\tpath: %s\n", paths[i]);
    if (paths[i] != NULL)
    {
      char *filename = (char *) Utility::getFileName(paths[i]);

      // Fixes #139
      if (filename == NULL)
      {
        paths[i] = NULL;
        continue;
      }

      dnames[i] = (char *) SDL_malloc( sizeof(char) * ( strlen(filename) + 1 + 1) );
      strcpy(dnames[i], FONT_BULLETPOINT_STR);
      strcat(dnames[i], filename);
    }
  }

  for (int i=0; i < ( NUM_RECENTFILES - 1 ); i++)
  {
    bool change = false;
    for (int j=i+1; j < NUM_RECENTFILES; j++)
    {
      if (dnames[i] && dnames[j] && strcmp(dnames[i], dnames[j]) == 0)
      {
        char *dirname = Utility::getDirectoryName(paths[j]);
        char *dirfile = (char *) SDL_malloc( 
          sizeof(char) * ( strlen(dirname) + sizeof(" -- - ") + strlen(dnames[j] ) + 1 ) );

        change = true;

        strcpy(dirfile, dnames[j]);
        strcat(dirfile, " " FONT_LONGDASH_STR FONT_LONGDASH_STR " " FONT_FOLDER_STR);
        strcat(dirfile, dirname);

        dnames[j] = dirfile;
      }
    }
    if (change)
    {
      char *dirname = Utility::getDirectoryName(paths[i]);
      char *dirfile = (char *) SDL_malloc( 
        sizeof(char) * ( strlen(dirname) + sizeof(" -- - ") + strlen(dnames[i] ) + 1 ) );

      strcpy(dirfile, dnames[i]);
      strcat(dirfile, " " FONT_LONGDASH_STR FONT_LONGDASH_STR " " FONT_FOLDER_STR);
      strcat(dirfile, dirname);

      dnames[i] = dirfile;
    }
  }
}