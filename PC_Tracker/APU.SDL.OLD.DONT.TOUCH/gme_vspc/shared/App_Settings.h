
#pragma once

#include "SDL.h"
#include <string>
#include "File_System.h"

/// Current Settings Revision
#define PREF_VERSION "0.6"

/// User Settings File
#define USER_PREF_FILE_NAME "Preferences.ini"
/// Default Settings File
//#define DEF_PREF_NAME "Default.ini"

// The application refers to this in-operation
class App_Settings
{
public:
  App_Settings(File_System *file_system);
  ~App_Settings();
  static int MAXLINE;
  //void apply( void );
  //void update( cSettings *Settings );
  void load_default();
  void load();
  void load(char *filename);
  void save(char *filename);
  void save();
  int parse_line( char ** parts, unsigned int count, unsigned int line_num );
  void parse(char *line, int line_num);
  struct 
  {
    Uint8 midi_port=0;
    char *audio_out_dev=NULL;
  } vars;

  std::string filepath;
  File_System *file_system;
};

