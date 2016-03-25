#include "File_System.h"
#include "File_System_Context.h"

//TEMP LOCATION
void error( const char* str )
{
  if ( str )
  {
    fprintf( stderr, "Error: %s\n", str );
    exit( EXIT_FAILURE );
  }
}

File_System::File_System()
{
  File_System_Context::file_system = this;
  //this->init();
}
File_System::~File_System()
{
  fprintf(stderr, "~File_System");
  if (pref_path)
    SDL_free(pref_path);
  if (pref_path_quoted)
    SDL_free(pref_path_quoted);
  if (data_path)
    SDL_free(data_path);
  if (data_path_quoted)
    SDL_free(data_path_quoted);
  if (tmp_path)
  {
    SDL_free(tmp_path);
  }
  if (tmp_path_quoted)
  {
    char *rm_cmd = (char*)SDL_malloc(sizeof(char) * (strlen(RM_CMD)+strlen(tmp_path_quoted)+2));
    strcpy (rm_cmd, RM_CMD);
    strcat (rm_cmd, tmp_path_quoted);
    printf("rm_cmd = %s\n", rm_cmd);
    system(rm_cmd);
    //#undef RM_CMD
    //#undef RM_PARM

    SDL_free(rm_cmd);
    SDL_free(tmp_path_quoted);
  }
}
int File_System::init()
{
  is_loaded=true;
  char tmpbuf[100];
  char *base_path = SDL_GetBasePath();
  if (base_path)
  {
    data_path = base_path;
    data_path_quoted = (char*) SDL_malloc(sizeof(char) * (strlen(data_path)+strlen("\"\"")+1) );
    strcpy (data_path_quoted, "\"");
    strcat (data_path_quoted, data_path);
    strcat (data_path_quoted, "\"");
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
    pref_path = pref_pathp;
    tmp_path = (char*)SDL_malloc(sizeof(char) * (strlen(pref_path)+strlen("tmp/")+5) );
    tmp_path_quoted = (char*)SDL_malloc(sizeof(char) * (strlen(pref_path)+strlen("\"tmp/\"")+1+5) );
    strcpy(tmp_path, pref_path);
    char *p = tmp_path + strlen(pref_path);
    strcpy(p, "tmp");
    p[3] = PATH_SEP;
    p[4] = 0;
    fprintf(stderr, "tmppath = %s\n", tmp_path);
    strcpy (tmp_path_quoted, "\"");
    strcat (tmp_path_quoted, tmp_path);
    strcat (tmp_path_quoted, "\"");
    fprintf (stderr, "tmp_path_quoted = %s\n", tmp_path_quoted);

    char *mkdir_cmd = (char*)SDL_malloc(sizeof(char) * (strlen(MKDIR_CMD)+strlen(tmp_path_quoted)+2+ 5) );
    strcpy (mkdir_cmd, MKDIR_CMD);
    strcat (mkdir_cmd, tmp_path_quoted);
    fprintf (stderr, "mkdir_cmd = %s\n", mkdir_cmd);
    system(mkdir_cmd);
    SDL_free(mkdir_cmd);
//    #undef MKDIR_CMD
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

void File_System::write_file( const char* path, void const* in, long size )
{
  FILE* out = fopen( path, "wb" );
  if ( !out ) error( "Couldn't create file" );
  if ( (long) fwrite( in, 1, size, out ) < size ) error( "Couldn't write file" );
  fclose( out );
}


