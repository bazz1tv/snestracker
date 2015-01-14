#include "Midi.h"

Midi::~Midi()
{
  in->closePort();
  if (in)
    delete in;
}
Midi::Midi()
{
  apiMap[RtMidi::MACOSX_CORE] = "OS-X CoreMidi";
  apiMap[RtMidi::WINDOWS_MM] = "Windows MultiMedia";
  apiMap[RtMidi::UNIX_JACK] = "Jack Client";
  apiMap[RtMidi::LINUX_ALSA] = "Linux ALSA";
  apiMap[RtMidi::RTMIDI_DUMMY] = "RtMidi Dummy";

  RtMidi :: getCompiledApi( apis );
  try {

    // Rtin constructor ... exception possible
    in = new RtMidiIn();

    std::cout << "\nCurrent input API: " << apiMap[ in->getCurrentApi() ] << std::endl;

    // Check inputs.
    unsigned int nPorts = in->getPortCount();
    fprintf(stderr,"\nThere are %d MIDI input sources available.\n", nPorts);

    if (nPorts==0)
    {
      std::cout << "No input ports available!" << std::endl;
      is_available=false;
      return;
    }

    is_available=true;
    for ( unsigned i=0; i<nPorts; i++ ) {
      std::string portName = in->getPortName(i);
      available_devices.push_back(portName);
      std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
    }

    //

    // at construction we will need to open a config file and determine
    // the default port to open if it is available

    /*for (int i=0; i < available_devices.size(); i++)
    {
      //static int i=1;
      std::cout << "  Input Port #" << i << ": " << available_devices[i] << '\n';
    
    }*/

    // RtMidiOut constructor ... exception possible
    /*midiout = new RtMidiOut();

    std::cout << "\nCurrent output API: " << apiMap[ midiout->getCurrentApi() ] << std::endl;

    // Check outputs.
    nPorts = midiout->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";

    for ( unsigned i=0; i<nPorts; i++ ) {
      std::string portName = midiout->getPortName(i);
      std::cout << "  Output Port #" << i+1 << ": " << portName << std::endl;
    }
    std::cout << std::endl;
*/
  } catch ( RtMidiError &error ) {
    error.printMessage();
  }
}