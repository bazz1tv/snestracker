#pragma once

#include <map>
#include "RtMidi.h"
#include <cstdio>
struct Midi
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