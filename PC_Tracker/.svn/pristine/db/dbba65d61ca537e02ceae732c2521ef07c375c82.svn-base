
#include "Globals.h"

using namespace std;

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
	}

	/*if( Screen && ( ( Screen->flags & SDL_FULLSCREEN && !Fullscreen ) || ( !( Screen->flags & SDL_FULLSCREEN ) &&
		Fullscreen ) || ( Screen->format->BytesPerPixel != Screen_Bpp ) || ( window_width != Screen_W ) || 
		( window_height != Screen_H ) ) ) 
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
			DEBUGLOG( "Error : No Preferences File and no Default Preferences File found\n" );
			exit( 1 );
		}
	}
	else
	{
		Load( USER_PREF_NAME );
	}
}

void cPreferences :: Load( string filename )
{
	char *contents = new char[600];

	ifstream ifs( filename.c_str(), ios::in );

	if ( !ifs )
	{
		DEBUGLOG( "Could not open preferences file : %s\n", filename.c_str() );
		exit( 1 );
	}

	for( int i = 0; ifs.getline( contents, 600 ); i++ )
	{
		Parse( contents, i );
	}

	ifs.close();

	delete contents;
}

void cPreferences :: Save( void )
{
	//Update();

	ofstream ofs( USER_PREF_NAME, ios::out );

	char row[250];

	sprintf( row,  "### Preferences file V.%s for MeatBall V.0 #### \n\n", PREF_VERSION );
	ofs.write( row, strlen( row ) );

	sprintf( row, "Music %d\n", pSettings->Music );
	ofs.write(row, strlen(row));

	sprintf( row, "Sounds %d\n", pSettings->Sounds );
	ofs.write( row, strlen(row));

	sprintf( row, "Fullscreen %d\n", pSettings->Fullscreen );
	ofs.write( row, strlen(row));

	sprintf( row,"Screen_H %d\n", pSettings->Screen_H );
	ofs.write( row, strlen(row));

	sprintf( row,  "Screen_W %d\n", pSettings->Screen_W );
	ofs.write(row, strlen( row ) );

	sprintf( row, "Screen_Bpp %d\n", pSettings->Screen_Bpp );
	ofs.write( row, strlen( row ) );

	sprintf( row, "svol %d\n", pSettings->svol );
	ofs.write( row, strlen( row ) );

	sprintf( row, "mvol %d\n", pSettings->mvol );
	ofs.write( row, strlen( row ) );

	sprintf( row, "WalkLeft %d\n", pSettings->Key_walk_left );
	ofs.write( row, strlen( row ) );

	sprintf( row, "WalkRight %d\n", pSettings->Key_walk_right );
	ofs.write( row, strlen( row ) );

	sprintf( row, "Jump %d\n", pSettings->Key_jump );
	ofs.write( row, strlen( row ) );

	sprintf( row, "FirePrimary %d\n", pSettings->Key_shoot_primary );
	ofs.write( row, strlen( row ) );

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
	if ( strlen( command ) <= 5 || *command == '#')
	{
		return;
	}
	
	char* str = command;
	int count = 0;
	int i = 0;
	
	str += strspn(str," ");
	
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
		size_t len = strcspn( str," " );
		parts[i] = (char*)malloc( len + 1 );
		memcpy( parts[i], str, len );
		parts[i][len] = 0;
		str += len + strspn( str + len, " " );
		i++; 
	}
	
	// Level Message handler
	ParseLine( parts, count, line );
	
	for( i = 0; i < count ; i++ )
	{
		delete( parts[i] );
	}
	
	delete []parts;
}

int cPreferences :: ParseLine( char ** parts, unsigned int count, unsigned int line )
{
	if ( strcmp( parts[0], "Music") == 0 )
	{
		if( atoi( parts[1] ) == 0 )
		{
			pSettings->Music = SDL_FALSE;
		}
		else
		{
			pSettings->Music = SDL_TRUE;
		}
	}
	else if ( strcmp( parts[0], "Sounds") == 0 )
	{
		if(atoi(parts[1]) == 0)
		{
			pSettings->Sounds = SDL_FALSE;
		}
		else
		{
			pSettings->Sounds = SDL_TRUE;
		}
	}
	else if ( strcmp( parts[0], "Fullscreen") == 0 )
	{
		if(atoi(parts[1]) == 0)
		{
			pSettings->Fullscreen = SDL_FALSE;
		}
		else
		{
			pSettings->Fullscreen = SDL_TRUE;
		}
	}
	else if ( strcmp( parts[0], "Screen_H") == 0 )
	{
		if(atoi(parts[1]) < 0 || atoi(parts[1]) > 1600)
		{
			return 0; // error
		}
		else
		{
			pSettings->Screen_H = atoi( parts[1] );
		}
	}
	else if ( strcmp( parts[0], "Screen_W") == 0 )
	{
		if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > 1600 )
		{
			return 0; // error
		}
		else
		{
			pSettings->Screen_W = atoi(parts[1]);
		}
	}
	else if ( strcmp( parts[0], "Screen_Bpp") == 0 )
	{
		if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > 32 )
		{
			return 0; // error
		}
		else
		{
			pSettings->Screen_Bpp = atoi( parts[1] );
		}
	}
	else if ( strcmp( parts[0], "svol") == 0 )
	{
		if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > 128 )
		{
			return 0; // error
		}
		else
		{
			pSettings->svol = atoi( parts[1] );
		}
	}
	else if ( strcmp( parts[0], "mvol") == 0 )
	{
		if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > 128 )
		{
			return 0; // error
		}
		else
		{
			pSettings->mvol = atoi( parts[1] );
		}
	}
	// You will notice the following are for the Player's KEYboard preferences
	// According to SDL_keysym.h the bounds of the keys enum SDL_Keys is from 0 -> SDLK_LAST
	// So in this way, we perform bounds checking
	else if ( strcmp( parts[0], "WalkLeft") == 0 )
	{
		// See above description
		/*if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > SDLK_LAST )
		{
			return 0; // error
		}
		else
		{*/
			pSettings->Key_walk_left = (SDL_Keycode)atoi( parts[1] );
		//}
	}
	else if ( strcmp( parts[0], "WalkRight") == 0 )
	{
		// See above description
		/*if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > SDLK_LAST )
		{
			return 0; // error
		}
		else
		{*/
			pSettings->Key_walk_right = (SDL_Keycode)atoi( parts[1] );
		//}
	}
	else if ( strcmp( parts[0], "Jump") == 0 )
	{
		// See above description
		/*if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > SDLK_LAST )
		{
			return 0; // error
		}
		else
		{*/
			pSettings->Key_jump = (SDL_Keycode)atoi( parts[1] );
		//}
	}
	else if ( strcmp( parts[0], "FirePrimary") == 0 )
	{
		// Bounds checking for Keys
		// See above description
		/*if( atoi( parts[1] ) < 0 || atoi( parts[1] ) > SDLK_LAST )
		{
			return 0; // error
		}
		else
		{*/
			pSettings->Key_shoot_primary = (SDL_Keycode)atoi( parts[1] );
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