#pragma once
#include <SDL.h>
struct Audio
{
  Audio();
  static double calculate_linear_gain_from_db(double gain_db, double min_gain=-96.0);

  static double calculate_fullscale_db_from_postgain_sample(int *sample, double min_gain);
  //the value ranges from 0 to SDL_GetNumAudioDevices() - 1
  struct Devices
  {
    Devices();
    ~Devices();
    enum Type { playback=0, record=1 };
    static int how_many;
    void query();
    char **device_strings=NULL;
    SDL_AudioDeviceID id=0;
    static const char *selected_audio_out_dev;
  } devices;
};