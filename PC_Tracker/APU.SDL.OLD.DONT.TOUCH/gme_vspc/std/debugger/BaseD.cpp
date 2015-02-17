#include "BaseD.h"
/*Music_Player *BaseD::player;
SDL_Surface *BaseD::screen;
uint8_t *BaseD::IAPURAM;*/
#include "Main_Window.h"
#include "Menu_Bar.h"
#include "File_System_Context.h"

//unsigned char *BaseD::Profile::orig_spc_state=NULL;
bool BaseD::Profile::is_profiling=false;
BaseD::Profile * BaseD::tmp_profile=NULL;

My_Nfd BaseD::nfd;
int BaseD::grand_mode=GrandMode::MAIN;
//int BaseD::submode=0;
BaseD::Cfg BaseD::g_cfg;// = { 0,0,0,0,0,0,DEFAULT_SONGTIME,0,0,0,0,0,NULL };

bool BaseD::is_first_run=true;
int BaseD::g_paused = 0;
uint8_t * BaseD::IAPURAM=NULL;
bool BaseD::quitting=false;
unsigned char BaseD::packed_mask[32];

int BaseD::g_cur_entry= 0;
bool BaseD::paused;

int BaseD::song_time;
track_info_t BaseD::tag;
char * BaseD::g_real_filename=NULL;
char BaseD::now_playing[1024];
//bool BaseD::is_onetime_draw_necessary=false;


Experience * BaseD::exp=NULL;
Main_Window * BaseD::main_window=NULL;
Instrument_Window * BaseD::instr_window=NULL;
Dsp_Window * BaseD::dsp_window=NULL;
Menu_Bar * BaseD::menu_bar=NULL;

const char * BaseD::path=NULL;
Voice_Control BaseD::voice_control;

void BaseD::Profile::process()
{
  BaseD::player->pause(0);

  elapsed_seconds = (int((BaseD::player->emu()->tell()/1000)));
  if (elapsed_seconds == seconds_covered+1)
  {
    BaseD::player->pause(1, true, false);
    DEBUGLOG("seconds elapsed: %d\n", elapsed_seconds);
    seconds_covered = elapsed_seconds;
  }
  if (seconds_covered == 7)
  {
    BaseD::Profile::is_profiling=false;
    delete this;
  }
}

int BaseD::switch_to_memory(void *data)
{
  BaseD::switch_mode(BaseD::GrandMode::MAIN);
}
int BaseD::switch_to_dsp(void *data)
{
  BaseD::switch_mode(BaseD::GrandMode::DSP_MAP);
}
int BaseD::switch_to_instrument(void *data)
{
  BaseD::switch_mode(BaseD::GrandMode::INSTRUMENT);
}

int BaseD::Clickable::toggle_echo(void *nada)
{
  player->spc_emu()->toggle_echo();
}

int BaseD::Clickable::inc_tempo(void *nada)
{
  player->inc_tempo();
}
int BaseD::Clickable::dec_tempo(void *nada)
{
  player->dec_tempo();
}

void BaseD::check_paths_and_reload(char **paths/*=g_cfg.playlist*/, 
  int numpaths/*=g_cfg.num_files*/, bool is_drop_event/*=false*/)
{
  bool rsn_found=false;
  // Check here if path is RSN
  for (size_t i=0; i < numpaths; i++)
  {
    char *path = paths[i];
    char *ext;
    char *name;
    ext = strrchr(path, '.');
#ifdef WIN32
name = strrchr(path, '\\'); // Windows might need backslash check
#else
name = strrchr(path, '/'); // Windows might need backslash check
#endif
  //assert(name);
  if (!name)
    name = path;
  else
  {
    name += 1; // move past '/' character
  }

    if (!strcmp(ext, ".rsn") || !strcmp(ext, ".rar"))
    {
      rsn_found = true;
      fprintf(stderr, "rsn found\n");
      char *mkdir_cmd = (char*) SDL_malloc(sizeof(char) * 
        (strlen("mkdir -p ")+
          strlen(File_System_Context::file_system->tmp_path_quoted)+((ext-name+1)+1)) );

      char *dir_quoted = (char*) SDL_malloc(sizeof(char) * 
        (strlen(File_System_Context::file_system->tmp_path_quoted)+((ext-name+1)+2)) );

      strcpy(mkdir_cmd, "mkdir ");
      char *dirp = mkdir_cmd + 6;
      char *p = mkdir_cmd + 6;
      strcpy(p, File_System_Context::file_system->tmp_path_quoted);
      p += strlen(File_System_Context::file_system->tmp_path_quoted) - 1;
      // folderp is the game folder inside the tmp dir
      for (char *folderp = name; folderp != ext; folderp++)
      {
        *(p++) = *folderp;
      }

      *(p++) = '/';
      *(p++) = '"';
      *p = 0;
      strcpy (dir_quoted, dirp);
      //strcat(dir_quoted, "/");
      fprintf(stderr, "mkdircmd = '%s'\n", mkdir_cmd);
      fprintf(stderr, "dir = %s\n", dir_quoted);
      system(mkdir_cmd);

      // data_path_quoted + "unrar e " + path + " " + dir_quoted
      char *unrar_cmd = (char*) SDL_malloc(sizeof(char) * 
        (
          strlen(File_System_Context::file_system->data_path_quoted) +
          strlen("unrar e -y") + 2 + strlen(path) + 1 + strlen(dir_quoted) + 1
        ));
      strcpy(unrar_cmd, File_System_Context::file_system->data_path_quoted);
      unrar_cmd[strlen(unrar_cmd)-1] = 0;
      strcat(unrar_cmd, "unrar\" e -y \"");
      strcat(unrar_cmd, path);
      strcat(unrar_cmd, "\" ");
      strcat(unrar_cmd, dir_quoted);
      fprintf(stderr, "unrar_cmd = %s\n", unrar_cmd);
      system(unrar_cmd);

      FILE *fp;
      int status;
      char spc_path[PATH_MAX];

      char *ls_cmd = (char*) SDL_malloc ( sizeof(char) * (strlen("ls ")+strlen(dir_quoted)+strlen("*.spc")+1));
      strcpy(ls_cmd, "ls ");
      strcat(ls_cmd, dir_quoted);
      //ls_cmd[strlen(ls_cmd)-1] = 0;
      strcat(ls_cmd, "*.spc");
      fprintf(stderr, "ls_cmd = %s\n", ls_cmd);
      fp = popen(ls_cmd, "r");
      if (fp == NULL)
          /* Handle error */;


      while (fgets(spc_path, PATH_MAX, fp) != NULL)
      {
        spc_path[strlen(spc_path)-1] = 0;
        char **tmp;
        if ( (tmp=(char**)SDL_realloc(BaseD::nfd.rsn_spc_paths, sizeof(char*) * (BaseD::nfd.num_rsn_spc_paths+1))) == NULL)
        {
          perror ("realloc");
          break;
        }
        BaseD::nfd.rsn_spc_paths = tmp;
        BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths] = (char*) SDL_calloc(strlen(spc_path)+3, sizeof(char));
        //strcpy(BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths], "\"");
        strcpy(BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths], spc_path);
        //strcat(BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths], "\"");
        //fprintf(stderr, "path = %s\n", BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths]);
        BaseD::nfd.num_rsn_spc_paths++;
      }


      status = pclose(fp);
      if (status == -1) {
          /* Error reported by pclose() */
          //...
      } else {
          /* Use macros described under wait() to inspect `status' in order
             to determine success/failure of command executed by popen() */
          //...
      }
      SDL_free(ls_cmd);
      SDL_free(unrar_cmd);
      SDL_free(mkdir_cmd);
      SDL_free(dir_quoted);
    }
    else
    {
      char **tmp;
      if ( (tmp=(char**)SDL_realloc(BaseD::nfd.rsn_spc_paths, sizeof(char*) * (BaseD::nfd.num_rsn_spc_paths+1))) == NULL)
      {
        perror ("realloc");
        break;
      }
      BaseD::nfd.rsn_spc_paths = tmp;
      BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths] = (char*) SDL_calloc(strlen(path)+3, sizeof(char));
      //strcpy(BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths], "\"");
      strcpy(BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths], path);
      //strcat(BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths], "\"");
      //fprintf(stderr, "path = %s\n", BaseD::nfd.rsn_spc_paths[BaseD::nfd.num_rsn_spc_paths]);
      BaseD::nfd.num_rsn_spc_paths++;
    }
  }

  if (!is_drop_event)
  {
    //if (rsn_found)
      BaseD::reload(BaseD::nfd.rsn_spc_paths,BaseD::nfd.num_rsn_spc_paths);
    //else
     // BaseD::reload((char**)BaseD::nfd.paths, BaseD::nfd.numpaths);
  }
  else
  {
    //if (rsn_found)
      BaseD::reload(BaseD::nfd.rsn_spc_paths,BaseD::nfd.num_rsn_spc_paths);
    //else
      //BaseD::reload((char**)BaseD::nfd.paths, BaseD::nfd.numpaths);
  }
}
bool BaseD::check_time()
{
  if (player->is_paused())
    return false;
  /* Check if it is time to change tune.
   */   
  if (player->emu()->tell()/1000 >= song_time) 
  {
    fprintf(stderr, "POOP");
    if (g_cfg.autowritemask) {
      write_mask(packed_mask);
      if (g_cfg.apply_block) {
        printf("Applying mask on file %s using $%02X as filler\n", g_cfg.playlist[g_cur_entry], g_cfg.filler);
        applyBlockMask(g_cfg.playlist[g_cur_entry]);
      }
    }
    g_cur_entry++;
    if (g_cur_entry>=g_cfg.num_files)
    {
      printf ("penis3\n");
      /*reload();*/

      // this would pause the player [[IMPORTANTE: pause with no fadeout]]
      //player->pause(true, false); 

      // but instead let's restart the playlist
      g_cur_entry=0;
      reload();
      return true;
    }
    //goto reload;
    reload();
    return true;
  }

  return false;
}

int BaseD::menu_bar_events(SDL_Event &ev)
{
  /*switch (ev.type)
  {
    case SDL_MOUSEBUTTONDOWN:*/
    /*if (
      ((ev.button.y >screen->h-12) && (ev.button.y<screen->h)))
    {
      int x = ev.button.x / CHAR_WIDTH;
      if (x>=1 && x<=4) { printf ("penis5\n"); quitting=true; } // exit
      if (x>=CHAR_WIDTH && x<=12) { 
        toggle_pause();
      } // pause

      if (x>=16 && x<=22) {  // restart
        restart_current_track();
      }

      if (x>=26 && x<=29) {  // prev
        SDL_PauseAudio(1);
        prev_track();
      }

      if (x>=33 && x<=36) { // next
        next_track();
      }

      if (x>=41 && x<=50) { // write mask
        //write_mask(packed_mask);
      }

      if (x>=53 && x<=54) { // Main
        //write_mask(packed_mask);
        //mode = MODE_DSP_MAP;
        switch_mode(GrandMode::MAIN);
      }
      if (x>=58 && x<=59) { // DSP MAP
        //write_mask(packed_mask);
        //mode = MODE_DSP_MAP;
        switch_mode(GrandMode::DSP_MAP);
      }
      if (x>=63 && x<=67) { // Instr
        //write_mask(packed_mask);
        //mode = MODE_DSP_MAP;
        switch_mode(GrandMode::INSTRUMENT);
      }
    }*/
  //}
  return menu_bar->receive_event(ev);
}

void BaseD::update_track_tag()
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

  now_playing[0] = 0;
  if (tag.song)
  {
    if (strlen((const char *)tag.song)) {
      sprintf(now_playing, "Now playing: %s (%s), dumped by %s\n", tag.song, tag.game, tag.dumper);
    }   
  }
  if (strlen(now_playing)==0) {
    sprintf(now_playing, "Now playing: %s\n", g_cfg.playlist[g_cur_entry]);
  }
}

void BaseD::start_track( int track, const char* path )
{
  //paused = false;
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

// YEAHH!!!
void BaseD::reload(char **paths/*=NULL*/, int numpaths/*=0*/)
{
  char *path=NULL;
  bool using_playlist=false;
  DEBUGLOG("g_cfg.playlist = %x\n", g_cfg.playlist);
  DEBUGLOG("g_cur_entry = %d", g_cur_entry);
  if (!paths)
  {
    DEBUGLOG("A");
    using_playlist=true;
    if (g_cfg.playlist[g_cur_entry])
      path = g_cfg.playlist[g_cur_entry];
    else path = NULL;
  }
  else
  {
    DEBUGLOG("B");
    g_cur_entry = 0;
    g_cfg.playlist = paths;
    g_cfg.num_files = numpaths;
    path = g_cfg.playlist[g_cur_entry];
  }

  DEBUGLOG("path = %d\n", path);
  if (path == NULL)
  {
    //using_playlist = false;
    player->has_no_song = true;
    return; 
  }

#ifdef WIN32
  g_real_filename = strrchr(path, '\\');
#else
  g_real_filename = strrchr(path, '/');
#endif
  if (!g_real_filename) {
    g_real_filename = path;
  }
  else {
    // skip path sep
    g_real_filename++;
  } 
  //main_window->reload();
  // Load file
  BaseD::path = path;
  handle_error( player->load_file( path ) );
  
  IAPURAM = player->spc_emu()->ram();
  //Memory::IAPURAM = IAPURAM;  
  // report::memsurface.init
  
  report::memsurface.clear();

  memset(report::used, 0, sizeof(report::used));
  memset(report::used2, 0, sizeof(report::used2));
  //if (!mouseover_hexdump_area.address)mouseover_hexdump_area.address =0;
  report::last_pc = -1;
  player->mute_voices(voice_control.muted);
  start_track( 1, path );
//  voice_control.was_keyed_on = 0;
  
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
  if (grand_mode == MAIN)
    main_window->draw_track_tag();

  char title [512];
  //if (using_playlist)
  //{
    sprintf( title, "%s: %d/%d %s (%ld:%02ld)",
        game, g_cur_entry+1, g_cfg.num_files, player->track_info().song,
        seconds / 60, seconds % 60 );
  /*}
  else
  {
    sprintf( title, "%s: %d/%d %s (%ld:%02ld)",
        game, 1, 1, player->track_info().song,
        seconds / 60, seconds % 60 );
  }*/
  SDL_SetWindowTitle(sdlWindow, title);

}

void BaseD::toggle_pause()
{
  // this means (if the player has finished playing its last song...)
  // the player stops, but if the user tries to resume with space_bar or play command
  // this will restart from beginning of playlist
  if (g_cur_entry>=g_cfg.num_files)
    restart_track();
  else 
  {
    if_exp_is_instr_window_then_restore_spc();
    player->toggle_pause();
  }
}

void BaseD::restart_track()
{
  player->fade_out();
  //SDL_PauseAudioDevice(Audio_Context::audio->devices.id, 1);
  g_cur_entry=0;
  player->pause(0);
  reload();
}

void BaseD::prev_track()
{
  player->fade_out();
  //SDL_PauseAudioDevice(Audio_Context::audio->devices.id, 1);
  g_cur_entry--;
  if (g_cur_entry<0) { g_cur_entry = g_cfg.num_files-1; }
  reload();
}

void BaseD::next_track25()
{
  int tmp = g_cfg.num_files - g_cur_entry -1;
              g_cur_entry+=25;
              if (g_cur_entry>=g_cfg.num_files)
              { 
                int derp =  g_cur_entry - g_cfg.num_files;
                if (derp < g_cfg.num_files)
                  g_cur_entry = derp;
                else g_cur_entry=g_cfg.num_files-1;
              }
}

void BaseD::prev_track25()
{
  g_cur_entry-=25;
              int tmp = abs(g_cur_entry);
              if ((unsigned)g_cur_entry>=g_cfg.num_files)
              { 
                if ((unsigned)(g_cfg.num_files-tmp) < g_cfg.num_files)
                  g_cur_entry=g_cfg.num_files-tmp;  
                else 
                  g_cur_entry = 0;
                
              }
}

void BaseD::next_track()
{
  player->fade_out();
  //SDL_PauseAudioDevice(Audio_Context::audio->devices.id, 1);
  g_cur_entry++;
  if (g_cur_entry>=g_cfg.num_files) { g_cur_entry = 0; }
  reload();
}

void BaseD::draw_menu_bar()
{
  //
  //fprintf(stderr, "TTTT");
  menu_bar->draw(screen);
}

void BaseD::restart_current_track()
{
  report::memsurface.clear();
  player->fade_out();
  player->start_track(0); // based on only having 1 track
  //player->pause(0);
  // in the program .. this would have to change otherwise
}

void BaseD::if_exp_is_instr_window_then_restore_spc()
{
  if (exp == (Experience*)instr_window)
  {
    instr_window->restore_spc(false); // restore SPC but don't necessarily resume playing
  }
}

void BaseD::switch_mode(int mode)
{
  if (grand_mode == mode)
    return;
  grand_mode = mode;
  clear_screen();
  draw_menu_bar();

  // If we switched from instrument window, need to re-enable regular spc playback
  if_exp_is_instr_window_then_restore_spc();

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
  else if (mode == GrandMode::INSTRUMENT)
  {
    if (instr_window)
      exp = (Experience*)instr_window;
    else
    {
      fprintf(stderr, "NO INSTRUMENT_WINDOW!?!\n");
      exit(2);
    }
    instr_window->one_time_draw();
  }
}


void BaseD::pack_mask(unsigned char packed_mask[32])
{
  int i;
  
  memset(packed_mask, 0, 32);
  for (i=0; i<256; i++)
  {
    if (report::used2[i])
    packed_mask[i/8] |= 128 >> (i%8);
  }
}

void BaseD::applyBlockMask(char *filename)
{
  FILE *fptr;
  unsigned char nul_arr[256];
  int i;

  memset(nul_arr, g_cfg.filler, 256);
  
  fptr = fopen(filename, "r+");
  if (!fptr) { perror("fopen"); }

  printf("[");
  for (i=0; i<256; i++)
  {
    fseek(fptr, 0x100+(i*256), SEEK_SET);
    
    if (!report::used2[i]) {
      printf(".");
      fwrite(nul_arr, 256, 1, fptr);
    } else {
      printf("o");
    }
    fflush(stdout);
  }
  printf("]\n");
  
  fclose(fptr);
}


void BaseD::write_mask(unsigned char packed_mask[32])
{
  FILE *msk_file;
  char *sep;
  char filename[1024];
  unsigned char tmp;
  int i;
  strncpy(filename, g_cfg.playlist[g_cur_entry], 1024);
#ifdef WIN32
  sep = strrchr(filename, '\\');
#else
  sep = strrchr(filename, '/');
#endif
  // keep only the path
  if (sep) { sep++; *sep = 0; } 
  else { 
    filename[0] = 0; 
  }

  // add the filename
  strncat(filename, g_real_filename, 1024);

  // but remove the extension if any
  sep = strrchr(filename, '.');
  if (sep) { *sep = 0; }

  // and use the .msk extension
  strncat(filename, ".msk", 1024);

  msk_file = fopen(filename, "wb");
  if (!msk_file) {
    perror("fopen");
  }
  else
  {
    fwrite(packed_mask, 32, 1, msk_file);
  }

  printf("Writing mask to '%s'\n", filename);

  // the first 32 bytes are for the 256BytesBlock mask
  printf("256 Bytes-wide block mask:\n");
  for (i=0; i<32; i++) {
    printf("%02X",packed_mask[i]);
  }
  printf("\n");

  printf("Byte level mask..."); fflush(stdout);
  memset(packed_mask, 0, 32);
  for (i=0; i<65536; i+=8)
  {
    tmp = 0;
    if (report::used[i]) tmp |= 128;
    if (report::used[i+1]) tmp |= 64;
    if (report::used[i+2]) tmp |= 32;
    if (report::used[i+3]) tmp |= 16;
    if (report::used[i+4]) tmp |= 8;
    if (report::used[i+5]) tmp |= 4;
    if (report::used[i+6]) tmp |= 2;
    if (report::used[i+7]) tmp |= 1;
    fwrite(&tmp, 1, 1, msk_file);
  }
  printf("Done.\n");
  fclose(msk_file);
}