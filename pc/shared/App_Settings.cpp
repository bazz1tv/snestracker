
#include "App_Settings.h"
#include "File_System.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include "utility.h"
#include "Audio.h"
#include "gme/player/Music_Player.h"
#include "DEBUGLOG.h"
#include "platform.h"
#include "RecentFiles.h"
#include "Audio_Options.h"

int App_Settings::MAXLINE=600;
App_Settings *app_settings;

App_Settings::App_Settings(File_System *file_system) : file_system(file_system)
{
  if (!file_system->is_loaded)
    file_system->init();
  load();
  ::app_settings = this;
}
App_Settings::~App_Settings()
{
  save();
  if (vars.audio_out_dev)
  {
    SDL_free(vars.audio_out_dev);
  }
}

void App_Settings::save()
{
  std::ofstream ofs( filepath.c_str(),std::ios::out );

  // This is bad, taking out
  //char row[250];
  std::stringstream row;
  
  //sprintf( row,  "### Preferences file V.%s for MeatBall V.0 #### \n\n", PREF_VERSION );
  row << "### Preferences file V." << PREF_VERSION << " for " << APP_NAME_VERSION << " #### \n\n";
  ofs.write( row.str().c_str(), row.str().length() );
  Utility::clearsstream(row);

  //sprintf( row, "Music %d\n", pSettings->Music );
  row << "midi_port " << (int)vars.midi_port << std::endl;
  ofs.write( row.str().c_str(), row.str().length() );
  Utility::clearsstream(row);

  row << "audio_out_dev " << Audio::Devices::selected_audio_out_dev << std::endl;
  ofs.write( row.str().c_str(), row.str().length() );
  Utility::clearsstream(row);

  row << "sample_frame_size " << ::player->sample_frame_size << std::endl;
  ofs.write( row.str().c_str(), row.str().length() );
  Utility::clearsstream(row);

  for (int i=0; i < NUM_RECENTFILES; i++)
  {
    char str[sizeof("01")];
    fprintf(stderr, "Almost Writing Path!\n");
    if ( RecentFiles::paths[i] != NULL)
    {
      fprintf(stderr, "Writing Path!\n");
      sprintf(str, "%02d", i);
      row << "recent_file" << " " << str << " " << RecentFiles::paths[i] << std::endl;
      ofs.write( row.str().c_str(), row.str().length() );
      Utility::clearsstream(row);
    }
  }

  ofs.close();
}

void App_Settings::load_default()
{
  // nothing for now, all is taken care of at initializor
}
void App_Settings::load()
{
  //char tmpbuf[200];
  char line[MAXLINE];
  // quickest way off the top of my head to correctly concat the strings
  filepath.append(file_system->pref_path);
  filepath.append(USER_PREF_FILE_NAME);


  
  if (Utility::file_is_valid(filepath))
  {
    // load the file
    std::ifstream ifs( filepath.c_str(), std::ios::in );
    if ( !ifs )
    {
      sprintf(line,"Could not open preferences file : %s\n", filepath.c_str() );
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "Could not open FILE!",
        line,
        NULL);
      load_default();
    }
    else
    {
      for( int i = 0; ifs.getline( line, MAXLINE ); i++ )
      {
        parse( line, i );
      }

      ifs.close();
    }
  }
  else
  {
    fprintf(stderr, "preferences file doesnt exist, loading default preferences\n");
    // don't do anything / load default settings (initializer should take care of this if possible)
    load_default();
  }
}

void App_Settings::parse(char *line, int line_num)
{

  // Note: Doesn't process Tabs
  char* str = line;
  int count = 0;
  int i = 0;
  
  // purge initial spaces...
  str += strspn(str," ");
  // comment or blank space
  if ( strlen( str ) <= 5 || *str == '#')
  {
    return;
  }

  while (*str)
  {
    str += strcspn(str," ");
    str += strspn(str," ");
    count++;
  }

  str = line; // reset

  fprintf(stderr, "count = %d\n", count);
  
  char** parts = new char*[ count ];
  
  str += strspn(str," ");
  
  while(*str)
  {
    // allocate a string for each part
    size_t len = strcspn( str," " );
    parts[i] = (char*)SDL_malloc( len + 1 );
    memcpy( parts[i], str, len );
    parts[i][len] = 0;
    
    // place str at the beginning of the next part.
    str += len + strspn( str + len, " " );
    // increase 2d pointer
    i++; 
  }
  
  if (count == 1)
  {
    fprintf(stderr, "line %d, [%s] is improper: ", line_num, line);
    // do not return, let parse_line deal with it
  }
  // Level Message handler
  // count = numparts
  parse_line( parts, count, line_num );
  
  for( i = 0; i < count ; i++ )
  {
    SDL_free(parts[i]);
    //delete( parts[i] );
  }
  
  delete []parts;
}

int App_Settings :: parse_line( char ** parts, unsigned int count, unsigned int line_num )
{
  
  std::istringstream atoi;

  int ibuffer;
  if (count != 1)
    atoi.str(parts[1]);
  if ( strcmp( parts[0], "midi_port") == 0 )
  {
    if (count == 1)
    {
      fprintf(stderr, "using default %d\n",vars.midi_port);
      return 0;
    }
    else if (!(atoi >> ibuffer))
    {
      fprintf(stderr, "Preferences: Couldn't Parse [%s]\n",  parts[0]);
    }
    vars.midi_port = ibuffer;
    fprintf(stderr, "midi_port set to %d\n", vars.midi_port);
  }
  else if ( strcmp( parts[0], "audio_out_dev") == 0 )
  {
    unsigned int i;

    if (count < 2) return 0;
    if (strlen(parts[1]) > 1000)
      return 0;
    size_t total_strlen=0;
 
    for (i=1; i < count; i++)
    {
      total_strlen += strlen(parts[i]);
      total_strlen += 1;
    }

    vars.audio_out_dev = (char *) SDL_malloc(sizeof(char) * (total_strlen));
    strcpy(vars.audio_out_dev, parts[1]);
    if (count > 2)
    {
      for (i=2; i < count; i++)
      {
        strcat(vars.audio_out_dev, " ");
        strcat(vars.audio_out_dev, parts[i]);
      }
    }
    fprintf(stderr, "Audio Out Device set to %s\n", vars.audio_out_dev);
  }
  else if ( strcmp( parts[0], "sample_frame_size") == 0 )
  {
    if (count == 1)
    {
      fprintf(stderr, "using default %d\n", DEFAULT_SAMPLE_FRAME_SIZE);
      return 0;
    }
    else if (!(atoi >> ibuffer))
    {
      fprintf(stderr, "Preferences: Couldn't Parse [%s]\n",  parts[0]);
    }

    // number is out of range or not power of 2
    if(ibuffer < AUDIO_BUFFER_MIN || ibuffer > AUDIO_BUFFER_MAX ||
        ( ibuffer != 0 && (ibuffer & (ibuffer-1)) != 0 ) )
    {
      DEBUGLOG("incorrect sample_frame_size in preferences: %d\n", ibuffer);
      ::player->sample_frame_size = DEFAULT_SAMPLE_FRAME_SIZE;
    }
    else
      ::player->sample_frame_size = ibuffer;
    fprintf(stderr, "sample_frame_size set to %d\n", ::player->sample_frame_size);
  }
  else if ( strcmp( parts[0], "recent_file") == 0 )
  {
    unsigned int i;

    if (count < 3)
    {
      //DEBUGLOG("COUNT < 3\n");
      return 0;
    }

    if (!(atoi >> ibuffer))
    {
      fprintf(stderr, "Preferences: Couldn't Parse [%s]\n",  parts[0]);
    }

    if (strlen(parts[2]) > PATH_MAX)
    {
      //DEBUGLOG("strlen(parts[2]) > PATH_MAX!\n");
      return 0;
    }
    size_t total_strlen=0;
 
    for (i=2; i < count; i++)
    {
      total_strlen += strlen(parts[i]);
      total_strlen += 1;  // Add 1 for the SPACE or end-of-line (null byte)
      //DEBUGLOG("\ttotal_strlen = %d\n", total_strlen);
    }
    //DEBUGLOG("total_strlen = %d\n", total_strlen);

    if (RecentFiles::paths[ibuffer] != NULL)
    {
      fprintf(stderr, "\tAlready loaded recentFile%02d! Overwriting...\n", ibuffer);
      SDL_free(RecentFiles::paths[ibuffer]);
    }

    RecentFiles::paths[ibuffer] = (char *) SDL_malloc(sizeof(char) * (total_strlen));
    strcpy(RecentFiles::paths[ibuffer], parts[2]);
    if (count > 3)
    {
      for (i=3; i < count; i++)
      {
        strcat(RecentFiles::paths[ibuffer], " ");
        strcat(RecentFiles::paths[ibuffer], parts[i]);
      }
    }

    RecentFiles::createDisplayNames();
    fprintf(stderr, "Recent file %02d set to %s\n", ibuffer, RecentFiles::paths[ibuffer]);
  }
  else
  {
    DEBUGLOG("Preferences : line_num %d Error : ", line_num );
    DEBUGLOG(" Unknown definition : %s\n", parts[0] );
    return 0; // error
  }

  return 1; // Succesfull 
}
