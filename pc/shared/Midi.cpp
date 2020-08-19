#include "Midi.h"
#include "Instrument_Window.h"
#include "Notes.h"

Midi *midi;

void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
  Midi *midi = (Midi*) userData;
  midi->internal_callback(deltatime, &message);
}

void Midi::internal_callback(double &deltatime, std::vector< unsigned char > **message)
{
  unsigned int nBytes = (*message)->size();
  for ( unsigned int i=0; i<nBytes; i++ )
  {
    if( parser.Parse( (uchar)(*message)->at(i), &msg ) )
    {     
      if( msg.IsSysEx() )
      {
        PrintSysEx( stderr, parser.GetSystemExclusive() );
      }     
      else
      {
        PrintMsg( stderr, &msg );
      }         
    }     
    //fprintf(stderr, "Byte %d = 0x%02X, ", i, (int)message->at(i) );
  }
  /*if ( nBytes > 0 )
  {

    //std::cout << "stamp = " << deltatime << std::endl;
  }*/
}

void Midi::PrintSysEx( FILE *f, jdkmidi::MIDISystemExclusive *ex )
{
  int l = ex->GetLength();
  
  fprintf( f, "Sysex Len=%d", l );
  
  for( int i=0; i<l; ++i )
  {
    if( ((i)%20) == 0 )
    {
      fprintf( f, "\n" );
    }   
    fprintf( f, "%02x ", (int)ex->GetData( i ) );   
  }
  fprintf( f, "\n" );
  fflush(f);
}


void Midi::PrintMsg( FILE *f, jdkmidi::MIDIMessage *m )
{
  // SNES
  if (m->IsNoteOn())
  {
      SDL_Event event2;

      event2.type = SDL_USEREVENT;
      event2.user.code = UserEvents::play_pitch;
      event2.user.data1 = (void*)m->GetNote();
      event2.user.data2 = 0;
      SDL_PushEvent(&event2);
      last_note_on = m->GetNote();
  }
  else if (m->IsNoteOff())
  {
    if (m->GetNote() == last_note_on)
    {
      SDL_Event event2;

      event2.type = SDL_USEREVENT;
      event2.user.code = UserEvents::keyoff;
      event2.user.data1 = (void*)m->GetNote();
      event2.user.data2 = 0;
      SDL_PushEvent(&event2);
    }
  }
  else if (m->IsProgramChange())
  {
    /* FIXME
    if (BaseD::grand_mode == BaseD::GrandMode::INSTRUMENT)
    {
      BaseD::instr_window->set_voice(m->GetPGValue());
    }  */
  }

  int l = m->GetLength();
  
  fprintf( f, "Msg : " );
  
  if( l==1 )
  {
    fprintf( f, " %02x \t\t=", m->GetStatus() );
  }
  else if( l==2 )
  {
    fprintf( f, " %02x %02x \t\t=", m->GetStatus(), m->GetByte1() );  
  }
  else if( l==3 )
  {
    fprintf( f, " %02x %02x %02x \t=", m->GetStatus(), m->GetByte1(), m->GetByte2() );
  }
  
  char buf[256];
  
  m->MsgToText( buf );
  
  fprintf( f, "%s\n", buf );
  fflush(f);
}

Midi::~Midi()
{
  if (is_available && in.isPortOpen())
    in.closePort();
}
Midi::Midi() : parser(32*1024)
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

    std::cout << "\nCurrent input API: " << apiMap[ in.getCurrentApi() ] << std::endl;

    // Check inputs.
    unsigned int nPorts = in.getPortCount();
    fprintf(stderr,"\nThere are %d MIDI input sources available.\n", nPorts);

    if (nPorts==0)
    {
      std::cout << "No input ports available!" << std::endl;
      is_available=false;
      return;
    }

    is_available=true;
    for ( unsigned i=0; i<nPorts; i++ ) {
      std::string portName = in.getPortName(i);
      available_devices.push_back(portName);
      std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
    }

    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue instead of sent to the callback function.
    in.setCallback( &mycallback, this);

    // Don't ignore sysex, timing, or active sensing messages.
    in.ignoreTypes( false, false, false );


    if (::app_settings->vars.midi_port < nPorts)
    {
      // load default
      in.openPort(::app_settings->vars.midi_port);
    }
    else in.openPort(0);


  } catch ( RtMidiError &error ) {
    error.printMessage();
  }

  ::midi = this;
}
