#include "track.h"
#include "globals.h"
namespace track
{
  char now_playing[1024];
  int song_time;
  track_info_t tag;

// update window title with track info
void update_window_title()
{

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
  
  char title [512];
  sprintf( title, "%s: %d/%d %s (%ld:%02ld)",
      game, g_cur_entry+1, g_cfg_num_files, player->track_info().song,
      seconds / 60, seconds % 60 );
  SDL_WM_SetCaption( title, title );
}
void update_tag()
{
  update_window_title();
  tag = player->track_info();


  /* decide how much time the song will play */
  if (!g_cfg_ignoretagtime) {
    song_time = (int)tag.length / 1000; //atoi((const char *)tag.seconds_til_fadeout);
    if (song_time <= 0) {
      song_time = g_cfg_defaultsongtime;
    }
  }
  else {
    song_time = g_cfg_defaultsongtime;
  }

  song_time += g_cfg_extratime;

  now_playing[0] = 0;
  if (tag.song)
  {
    if (strlen((const char *)tag.song)) {
      sprintf(now_playing, "Now playing: %s (%s), dumped by %s\n", tag.song, tag.game, tag.dumper);
    }   
  }
  if (strlen(now_playing)==0) {
    sprintf(now_playing, "Now playing: %s\n", g_cfg_playlist[g_cur_entry]);
  }
  /* information */

  // CLEAR THE BACKGROUND FIRST
  //536,465
  //800,519
  SDL_Rect r;
  r.x = 536;
  r.y = 465;
  r.w = 800-536;
  r.h = 519-465;
  SDL_FillRect(screen, &r, colors::black);

  //fprintf(stderr, "comment = %s\n", tag.comment);
  //fprintf(stderr, "path = %s\nsong = %s\ngame = %s\ndumper = %s\ncomment = %s")
  
  sprintf(tmpbuf, "Filename: %s", path);
  sdlfont_drawString(screen, INFO_X, INFO_Y+8, tmpbuf, colors::white);
  sprintf(tmpbuf, "Title...: %s", track::tag.song);
  sdlfont_drawString(screen, INFO_X, INFO_Y+16, tmpbuf, colors::white);
  sprintf(tmpbuf, "Game....: %s", track::tag.game);
  sdlfont_drawString(screen, INFO_X, INFO_Y+24, tmpbuf, colors::white);
  sprintf(tmpbuf, "Dumper..: %s", track::tag.dumper);
  sdlfont_drawString(screen, INFO_X, INFO_Y+32, tmpbuf, colors::white);
  sprintf(tmpbuf, "Comment.: %s", track::tag.comment);
  sdlfont_drawString(screen, INFO_X, INFO_Y+40, tmpbuf, colors::white);
}
}