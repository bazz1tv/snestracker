
#include "preferences.h"
#include <fstream>
#include <sstream>

using namespace std;

// Those sweet Globals
cSettings *pGameSettings;
cPreferences *pPreferences;

std::string default_settings[] =
{ "Music 1",
  "Sounds 1",
  "Fullscreen 1",
  "Screen_W 800",
  "Screen_H 600",
  "Screen_Bpp 32",
  "svol 113",
  "mvol 128",
  "WalkLeft 97",
  "WalkRight 100",
  "Jump 307",
  "FirePrimary 305",
 "" };


cSettings :: cSettings( void )
{
	Screen_Bpp			= 32;
	Screen_W			= 800;
	Screen_H			= 600;

	Fullscreen			= SDL_FALSE;

	Music				= SDL_TRUE;
	Sounds				= SDL_TRUE;
	svol				= 40;
	mvol				= 128;

	Key_walk_left		= SDLK_a;
	Key_walk_right		= SDLK_d;
	Key_shoot_primary	= SDLK_RCTRL;
	Key_jump			= SDLK_RALT;


}

cSettings :: ~cSettings( void )
{
	//
}

void cSettings :: Apply( void )
{
	if( pAudio ) 
	{
		pAudio->bMusic = Music;
		pAudio->bSounds = Sounds;
		pAudio->Sound_Volume = svol;
		pAudio->Music_Volume = mvol;
	}

	/*if( Screen && ( ( Screen->flags & SDL_FULLSCREEN && !Fullscreen ) || ( !( Screen->flags & SDL_FULLSCREEN ) &&
		Fullscreen ) || ( Screen->format->BytesPerPixel != Screen_Bpp ) || ( window.w != Screen_W ) || 
		( window.h != Screen_H ) ) ) 
	{
		// Todo : Reload the images and reset the Screen
	}*/
}

void cSettings :: Update( cSettings *Settings )
{
	Fullscreen = Settings->Fullscreen;
	Music = Settings->Music;
	Sounds = Settings->Sounds;
	Screen_Bpp = Settings->Screen_Bpp;
	Screen_H = Settings->Screen_H;
	Screen_W = Settings->Screen_W;
	svol = Settings->svol;
	mvol = Settings->mvol;

	Key_walk_left = Settings->Key_walk_left;
	Key_walk_right = Settings->Key_walk_right;
	Key_shoot_primary = Settings->Key_shoot_primary;
	Key_jump = Settings->Key_jump;
	//Key_shoot_secondary = Settings->Key_shoot_secondary;
}

cPreferences :: cPreferences( void )
{
	pSettings = new cSettings();

}

cPreferences :: ~cPreferences( void )
{
	Save();
	
	if( pSettings ) 
	{
		delete pSettings;
		pSettings = NULL;
	}
}

void cPreferences :: Load( void )
{
	if( !FileValid( USER_PREF_NAME ) ) 
	{
		if( FileValid( DEF_PREF_NAME ) )
		{
			DEBUGLOG( "Warning : No Preferences File found using default settings\n" );

			Load( DEF_PREF_NAME );
		}
		else
		{
			//DEBUGLOG( "Error : No Preferences File and no Default Preferences File found\n" );
			//exit( 1 );
      for (int i=0; default_settings[i] != ""; i++)
      {
        Parse((char *)default_settings[i].c_str(), i);
      }
		}
	}
	else
	{
		Load( USER_PREF_NAME );
	}
}

void cPreferences :: Load( string filename )
{
	this->filename = filename;
	// this looks bad but it's not, because we parse by array size! 
	char *contents = new char[600];

	ifstream ifs( filename.c_str(), ios::in );

	if ( !ifs )
	{
		DEBUGLOG( "Could not open preferences file : %s\n", filename.c_str() );
		exit( 1 );
	}

	for( register int i = 0; ifs.getline( contents, 600 ); i++ )
	{
		Parse( contents, i );
	}

	ifs.close();

	delete contents;
}

void clearsstream(stringstream &s)
{
	s.clear();//clear any bits set
	s.str(std::string());
}

void cPreferences:: Save()
{
	Save(USER_PREF_NAME);
}

void cPreferences :: Save( string filename )
{
	Update();

	ofstream ofs( filename.c_str(), ios::out );

	// This is bad, taking out
	//char row[250];
	stringstream row;
	
	//sprintf( row,  "### Preferences file V.%s for MeatBall V.0 #### \n\n", PREF_VERSION );
	row << "### Preferences file V." << PREF_VERSION << " for MeatBall V.0 #### \n\n";
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);

	//sprintf( row, "Music %d\n", pSettings->Music );
	row << "Music " << pSettings->Music << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
	//sprintf( row, "Sounds %d\n", pSettings->Sounds );
	row << "Sounds " << pSettings->Sounds << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
	//sprintf( row, "Fullscreen %d\n", pSettings->Fullscreen );
	row << "Fullscreen " << pSettings->Fullscreen << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
	//sprintf( row,"Screen_H %d\n", pSettings->Screen_H );
	row << "Screen_H " << pSettings->Screen_H << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
	//sprintf( row,  "Screen_W %d\n", pSettings->Screen_W );
	row << "Screen_W " << pSettings->Screen_W << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
	//sprintf( row, "Screen_Bpp %d\n", pSettings->Screen_Bpp );
	row << "Screen_Bpp " << (int)pSettings->Screen_Bpp << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
	//sprintf( row, "svol %d\n", pSettings->svol );
	row << "svol " << (int)pSettings->svol << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
	//sprintf( row, "mvol %d\n", pSettings->mvol );
	row << "mvol " << (int)pSettings->mvol << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
//	sprintf( row, "WalkLeft %d\n", pSettings->Key_walk_left );
	row << "WalkLeft " << pSettings->Key_walk_left << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
//	sprintf( row, "WalkRight %d\n", pSettings->Key_walk_right );
	row << "WalkRight " << pSettings->Key_walk_right << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
//	sprintf( row, "Jump %d\n", pSettings->Key_jump );
	row << "Jump " << pSettings->Key_jump << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
	//sprintf( row, "FirePrimary %d\n", pSettings->Key_shoot_primary );
	row << "FirePrimary " << pSettings->Key_shoot_primary << endl;
	ofs.write( row.str().c_str(), row.str().length() );
	clearsstream(row);
	//Key_walk_left,Key_walk_right,Key_shoot_primary,Key_shoot_secondary;

	ofs.close();
}

void cPreferences :: Default( void )
{
	if( FileValid( DEF_PREF_NAME ) )
	{
		Load( DEF_PREF_NAME );
	}
	else
	{
		DEBUGLOG( "Error : No Default Preferences File found\n" );
		exit( 1 );
	}
}

/// Set Preferences to be Game Settings
void cPreferences :: Update( void )
{
	pSettings->Update( pGameSettings ); // Looks dirty
}


void cPreferences :: Apply( void )
{
	pGameSettings->Update( pSettings );
	
	pGameSettings->Apply();
}

void cPreferences :: Parse( char* command, int line )
{
	// Note: Doesn't process Tabs
	
	// comment or blank space
	if ( strlen( command ) <= 5 || *command == '#')
	{
		return;
	}
	
	char* str = command;
	int count = 0;
	int i = 0;
	
	// purge initial spaces...
	str += strspn(str," ");
	
	// get the number of parts
	while (*str)
	{
		count++;
		str += strcspn(str," ");
		str += strspn(str," ");
	}
	
	str = command; // reset
	
	char** parts = new char*[ count ];
	
	str += strspn(str," ");
	
	while(*str)
	{
		// allocate a string for each part
		size_t len = strcspn( str," " );
		parts[i] = (char*)SDL_malloc( len + 1 );
		memcpy( parts[i], str, len );
		parts[i][len] = 0;
		
		// place str at the beginning of the next part.
		str += len + strspn( str + len, " " );
		// increase 2d pointer
		i++; 
	}
	
	// Level Message handler
	// count = numparts
	ParseLine( parts, count, line );
	
	for( i = 0; i < count ; i++ )
	{
		SDL_free(parts[i]);
		//delete( parts[i] );
	}
	
	delete []parts;
}

int cPreferences :: ParseLine( char ** parts, unsigned int count, unsigned int line )
{
	istringstream atoi(parts[1]);
	int ibuffer;
	
	if ( strcmp( parts[0], "Music") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
		}
		if( ibuffer == 0 )
		{
			pSettings->Music = SDL_FALSE;
		}
		else // this takes care of malformed entries
		{
			pSettings->Music = SDL_TRUE;
		}
	}
	else if ( strcmp( parts[0], "Sounds") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
		}
		if(ibuffer == 0)
		{
			pSettings->Sounds = SDL_FALSE;
		}
		else // this takes care of malformed entries
		{
			pSettings->Sounds = SDL_TRUE;
		}
	}
	else if ( strcmp( parts[0], "Fullscreen") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
		}
		if(ibuffer == 0)
		{
			pSettings->Fullscreen = SDL_FALSE;
		}
		else // this takes care of malformed entries
		{
			pSettings->Fullscreen = SDL_TRUE;
		}
	}
	else if ( strcmp( parts[0], "Screen_H") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
			// Let user Select Resolution
		}
		if(ibuffer < 0 /*|| ibuffer > 1600*/)
		{
			return 0; // error
		}
		else
		{
			pSettings->Screen_H = ibuffer;
		}
	}
	else if ( strcmp( parts[0], "Screen_W") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
			// Let user Select Resolution
		}
		if( ibuffer < 0 /*|| atoi( parts[1] ) > 1600*/ )
		{
			return 0; // error
		}
		else
		{
			pSettings->Screen_W = ibuffer;
		}
	}
	else if ( strcmp( parts[0], "Screen_Bpp") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
			// Let user Select Resolution
		}
		if( ibuffer < 0 || ibuffer > 32 )
		{
			return 0; // error
		}
		else
		{
			pSettings->Screen_Bpp = ibuffer;
		}
	}
	else if ( strcmp( parts[0], "svol") == 0 )
	{
		if (!(atoi >> ibuffer))
		{	
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
			ibuffer = 128;
		}
		if( ibuffer < 0 || ibuffer > 128 )
		{
			return 0; // error
		}
		else
		{
			pSettings->svol = ibuffer;
		}
	}
	else if ( strcmp( parts[0], "mvol") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
			ibuffer = 128;
		}
		if( ibuffer < 0 || ibuffer > 128 )
		{
			return 0; // error
		}
		else
		{
			pSettings->mvol = ibuffer;
		}
	}
	// The note is only for SDL 1.2
	// You will notice the following are for the Player's KEYboard preferences
	// According to SDL_keysym.h the bounds of the keys enum SDL_Keys is from 0 -> SDLK_LAST
	// So in this way, we perform bounds checking
	else if ( strcmp( parts[0], "WalkLeft") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
			// Let user Select
		}
		// See above description
		/*if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > SDLK_LAST )
		{
			return 0; // error
		}
		else
		{*/
			pSettings->Key_walk_left = (SDL_Keycode)ibuffer;
		//}
	}
	else if ( strcmp( parts[0], "WalkRight") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
			// Let user Select
		}
		// See above description
		/*if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > SDLK_LAST )
		{
			return 0; // error
		}
		else
		{*/
			pSettings->Key_walk_right = (SDL_Keycode)ibuffer;
		//}
	}
	else if ( strcmp( parts[0], "Jump") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << " In file " << filename << endl;
			PrintErrorToConsole();
			// Let user Select
		}
		// See above description
		/*if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > SDLK_LAST )
		{
			return 0; // error
		}
		else
		{*/
			pSettings->Key_jump = (SDL_Keycode)ibuffer;
		//}
	}
	else if ( strcmp( parts[0], "FirePrimary") == 0 )
	{
		if (!(atoi >> ibuffer))
		{
			error << "Preferences: Couldn't Parse " << parts[0] << " Section!" << "In file " << filename << endl;
			PrintErrorToConsole();
			// Let user Select
		}
		// Bounds checking for Keys
		// See above description
		/*if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > SDLK_LAST )
		{
			return 0; // error
		}
		else
		{*/
			pSettings->Key_shoot_primary = (SDL_Keycode)ibuffer;
		//}
	}
	else
	{
		DEBUGLOG("Preferences : line %d Error : ", line );
		DEBUGLOG(" Unknown definition : %s\n", parts[0] );
		return 0; // error
	}

	return 1; // Succesfull	
}