#pragma once

#include <map>
#include "RtMidi.h"
#include "App_Settings.h"

#include "jdkmidi/world.h"
#include "jdkmidi/midi.h"
#include "jdkmidi/msg.h"
#include "jdkmidi/sysex.h"
#include "jdkmidi/parser.h"

struct Midi
{
  Midi();
  ~Midi();

  void PrintSysEx( FILE *f, jdkmidi::MIDISystemExclusive *ex );
  void PrintMsg( FILE *f, jdkmidi::MIDIMessage *m );

  void internal_callback(double &deltatime, std::vector< unsigned char > **message);

  /* RtMidi Stuffz */
  std::map<int, std::string> apiMap;
  std::vector< RtMidi::Api > apis;
  std::vector<std::string> available_devices;
  RtMidiIn  in;
  bool is_available=true;
  //RtMidiOut *midiout = 0;
  Uint8 last_note_on=0;

  // jdkmidi stuffz
  jdkmidi::MIDIParser parser;
  jdkmidi::MIDIMessage msg;
};
