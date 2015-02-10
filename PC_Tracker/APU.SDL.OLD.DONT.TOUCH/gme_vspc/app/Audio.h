#pragma once
#include <SDL.h>
struct Audio
{
  //the value ranges from 0 to SDL_GetNumAudioDevices() - 1
  struct Devices
  {
    Devices() { query(); }
    ~Devices();
    enum Type { playback=0, record=1 };
    static int how_many;
    void query();
    char **device_strings=NULL;
  } devices;
};