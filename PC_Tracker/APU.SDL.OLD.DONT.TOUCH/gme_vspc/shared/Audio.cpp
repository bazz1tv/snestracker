#include "Audio.h"
#include "DEBUGLOG.h"
#include "Audio_Context.h"
int Audio::Devices::how_many=0;

const char * Audio::Devices::selected_audio_out_dev=NULL;


double Audio::calculate_linear_gain_from_db(double gain_db, double min_gain/*=-96.0*/)
{
  if (gain_db <= min_gain) return 0.0;  // -INF dB
  else return pow ( 10.0, (0.05 * gain_db) );
}
/*
dBvalue = 20.0 * log10 ( linear );
// dB = 20 * log (linear)
// NOT 10 * log (linear)!!!

// conversely...
linear = pow ( 10.0, (0.05 * dBvalue) );
// linear = 10^(dB/20)
*/
double Audio::calculate_fullscale_db_from_postgain_sample(int *sample, double min_gain/*=96.0*/)
{
  if (*sample == 0) return min_gain;

  if (*sample > 0)
    return (double)20.0 * (double)log10(*sample / 32767);
  //if (*sample < 0)
    return (double)20.0 * (double)log10(abs(*sample) / 32768);
}

Audio::Audio()
{
  Audio_Context::audio = this;
}

Audio::Devices::Devices()
{
  query();
  
}

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
    const char *audio_device_name = SDL_GetAudioDeviceName(i, Devices::Type::playback);
    printf("Audio device %d: %s\n", i, audio_device_name);
    device_strings[i] = (char*) SDL_malloc(sizeof(char) * (strlen(audio_device_name)+1));
    strcpy(device_strings[i], audio_device_name);
  }
}