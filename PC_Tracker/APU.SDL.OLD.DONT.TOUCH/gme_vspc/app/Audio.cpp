#include "Audio.h"
#include "DEBUGLOG.h"
int Audio::Devices::how_many=0;

Audio::Devices::~Devices()
{
  DEBUGLOG("~Devices;");
  if (device_strings) 
  {
    for (int i = 0; i < Devices::how_many; ++i)
    {
      SDL_free(device_strings[i]);
      //printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }
    SDL_free(device_strings); 
  }
}
void Audio::Devices::query()
{
  //the value ranges from 0 to SDL_GetNumAudioDevices() - 1
  Devices::how_many = SDL_GetNumAudioDevices(Devices::Type::playback);
  DEBUGLOG("num audio devices: %d\n", Devices::how_many);
  device_strings = (char**) SDL_malloc(sizeof(char*) * Devices::how_many);
  for (int i = 0; i < Devices::how_many; ++i)
  {
    const char *audio_device_name = SDL_GetAudioDeviceName(i, 0);
    printf("Audio device %d: %s\n", i, audio_device_name);
    device_strings[i] = (char*) SDL_malloc(sizeof(char) * (strlen(audio_device_name)+1));
    strcpy(device_strings[i], audio_device_name);
  }
}