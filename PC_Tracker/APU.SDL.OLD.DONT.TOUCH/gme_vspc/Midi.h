#pragma once

#include <map>
#include "RtMidi.h"
#include "App_Settings_Context.h"

struct Midi : public App_Settings_Context
{
  Midi();
  ~Midi();
  std::map<int, std::string> apiMap;
  std::vector< RtMidi::Api > apis;
  std::vector<std::string> available_devices;
  RtMidiIn  in;
  bool is_available=true;
  //RtMidiOut *midiout = 0;
};