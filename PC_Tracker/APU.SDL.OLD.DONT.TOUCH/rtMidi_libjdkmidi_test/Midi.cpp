#include "Midi.h"
#include <unistd.h>
void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
  unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
  {
    write(1,&message->at(i), 1);
  }
    //putchar((int)message->at(i) );
  if ( nBytes > 0 )
    ;//std::cout << "stamp = " << deltatime << std::endl;
}

Midi::~Midi()
{
  in.closePort();
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
    //in = new RtMidiIn();

    //std::cout << "\nCurrent input API: " << apiMap[ in.getCurrentApi() ] << std::endl;

    // Check inputs.
    unsigned int nPorts = in.getPortCount();
    //fprintf(stderr,"\nThere are %d MIDI input sources available.\n", nPorts);

    if (nPorts==0)
    {
      //std::cout << "No input ports available!" << std::endl;
      is_available=false;
      return;
    }

    is_available=true;
    for ( unsigned i=0; i<nPorts; i++ ) {
      std::string portName = in.getPortName(i);
      available_devices.push_back(portName);
      //std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
    }

    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue instead of sent to the callback function.
    in.setCallback( &mycallback, NULL );

    // Don't ignore sysex, timing, or active sensing messages.
    in.ignoreTypes( false, false, false );


    in.openPort(0);


  } catch ( RtMidiError &error ) {
    error.printMessage();
  }
}