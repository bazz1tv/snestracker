#include "Debugger_Base.h"
/*Music_Player *Debugger_Base::player;
SDL_Surface *Debugger_Base::screen;
uint8_t *Debugger_Base::IAPURAM;*/
#include "Main_Window.h"

int Debugger_Base::grand_mode=GrandMode::MAIN;
//int Debugger_Base::submode=0;
Debugger_Base::Cfg Debugger_Base::g_cfg;// = { 0,0,0,0,0,0,DEFAULT_SONGTIME,0,0,0,0,0,NULL };

int Debugger_Base::g_paused = 0;
uint8_t * Debugger_Base::IAPURAM=NULL;
bool Debugger_Base::quitting=false;
unsigned char Debugger_Base::packed_mask[32];

int Debugger_Base::g_cur_entry= 0;
bool Debugger_Base::paused;

int Debugger_Base::song_time;
track_info_t Debugger_Base::tag;
char * Debugger_Base::g_real_filename=NULL;
//bool Debugger_Base::is_onetime_draw_necessary=false;


Experience * Debugger_Base::exp=NULL;
Main_Window * Debugger_Base::main_window=NULL;
Dsp_Window * Debugger_Base::dsp_window=NULL;

const char * Debugger_Base::path=NULL;
Voice_Control Debugger_Base::voice_control;

void Debugger_Base::update_track_tag()
{
  //update_window_title();
  tag = player->track_info();


  /* decide how much time the song will play */
  if (!g_cfg.ignoretagtime) {
    song_time = (int)tag.length / 1000; //atoi((const char *)tag.seconds_til_fadeout);
    if (song_time <= 0) {
      song_time = g_cfg.defaultsongtime;
    }
  }
  else {
    song_time = g_cfg.defaultsongtime;
  }

  song_time += g_cfg.extratime;
}

void Debugger_Base::start_track( int track, const char* path )
{
  paused = false;
  //if (!player->is_paused())
  handle_error( player->start_track( track - 1 ) );
  // update window title with track info
  
  long seconds = player->track_info().length / 1000;
  const char* game = player->track_info().game;
  if ( !*game )
  {
    // extract filename
    game = strrchr( path, '\\' ); // DOS
    if ( !game )
      game = strrchr( path, '/' ); // UNIX
    if ( !game )
    {
      if (path)
        game = path;
      else game = "";
    }
    else
      game++; // skip path separator
  }
  
  char title [512];
  sprintf( title, "%s: %d/%d %s (%ld:%02ld)",
      game, track, player->track_count(), player->track_info().song,
      seconds / 60, seconds % 60 );
  SDL_SetWindowTitle(sdlWindow, title);
}

void Debugger_Base::reload()
{
  if (g_cfg.playlist[g_cur_entry] == NULL)
    exit (2);

  #ifdef WIN32
  g_real_filename = strrchr(g_cfg.playlist[g_cur_entry], '\\');
#else
  g_real_filename = strrchr(g_cfg.playlist[g_cur_entry], '/');
#endif
  if (!g_real_filename) {
    g_real_filename = g_cfg.playlist[g_cur_entry];
  }
  else {
    // skip path sep
    g_real_filename++;
  } 
  //main_window->reload();
  // Load file
  path = g_cfg.playlist[g_cur_entry];
  handle_error( player->load_file( g_cfg.playlist[g_cur_entry] ) );
  
  IAPURAM = player->spc_emu()->ram();
  //Memory::IAPURAM = IAPURAM;
  
  // report::memsurface.init
  report::memsurface.clear();

  memset(report::used, 0, sizeof(report::used));
  memset(report::used2, 0, sizeof(report::used2));
  //if (!mouseover_hexdump_area.address)mouseover_hexdump_area.address =0;
  report::last_pc = -1;
  
  start_track( 1, path );
  voice_control.was_keyed_on = 0;
  player->mute_voices(voice_control.muted);
  player->ignore_silence();

  // update Window Title
  long seconds = player->track_info().length / 1000;
  const char* game = player->track_info().game;
  if ( !*game )
  {
    // extract filename
    game = strrchr( path, '\\' ); // DOS
    if ( !game )
      game = strrchr( path, '/' ); // UNIX
    if ( !game )
      game = path;
    else
      game++; // skip path separator
  }

  //main_window->is_onetime_draw_necessary=true;
  update_track_tag();

  char title [512];
  sprintf( title, "%s: %d/%d %s (%ld:%02ld)",
      game, g_cur_entry+1, g_cfg.num_files, player->track_info().song,
      seconds / 60, seconds % 60 );
  SDL_SetWindowTitle(sdlWindow, title);
}

void Debugger_Base::toggle_pause()
{
  player->toggle_pause();
}

void Debugger_Base::restart_track()
{
  SDL_PauseAudio(1);
  g_cur_entry=0;
  player->pause(0);
  reload();
}

void Debugger_Base::prev_track()
{
  SDL_PauseAudio(true);
  g_cur_entry--;
  if (g_cur_entry<0) { g_cur_entry = g_cfg.num_files-1; }
  reload();
}

void Debugger_Base::next_track()
{
  SDL_PauseAudio(true);
  g_cur_entry++;
  if (g_cur_entry>=g_cfg.num_files) { g_cur_entry = 0; }
  reload();
}

void Debugger_Base::draw_menu_bar()
{
  sprintf(tmpbuf, " QUIT - PAUSE - RESTART - PREV - NEXT - WRITE MASK - DSP MAP");
  sdlfont_drawString(screen, 0, screen->h-9, tmpbuf, Colors::yellow);
}

void Debugger_Base::restart_current_track()
{
  report::memsurface.clear();
  player->start_track(0); // based on only having 1 track
  player->pause(0);
  // in the program .. this would have to change otherwise
}

void Debugger_Base::change_grand_mode(int mode)
{
  grand_mode = mode;
  clear_screen();
  draw_menu_bar();
  if (mode == GrandMode::MAIN)
  {
    if (main_window)
      exp = (Experience*)main_window;
    else 
    {
      fprintf(stderr, "NO MAIN WINDOW!?!\n");
      exit(2);
    }
    main_window->one_time_draw();
  }
  else if (mode == GrandMode::DSP_MAP)
  {
    if (dsp_window)
      exp = (Experience*)dsp_window;
    else 
    {
      fprintf(stderr, "NO DSPWINDOW!?!\n");
      exit(2);
    }
  }
}