//#include "console.h"
#include "Globals.h"
#include <string.h>
#include <sstream>

#define HISTORY_LINES 11

using namespace std;


void Push(cCMD*& head, string command, SDL_bool (*handler)(string &), string helpstr, string syntax)
{
	cCMD* newCmd = new cCMD;
	newCmd->handler = handler;
	newCmd->syntax = syntax;
	newCmd->helpstr = helpstr;
	newCmd->command = command;
	newCmd->next = head; 
	head = newCmd;
}

/// Gets the current working path, Loads the Background image to the Image Manager,
/// Creates the console font, Push() all cCMDs to cConsole::CMDList, 
cConsole :: cConsole( void )
{	
	full_path = fs::current_path();

	histcounter = -1;
	// Init Background //////////
	IMan->Add( LoadImage( PIXMAPS_DIR "game/background/conBG.png", colorkey, 140 ), "Console_BG" );

	BG = new cBasicSprite( Renderer, IMan->GetPointer( "Console_BG" ), 0, 0 );

	BG->SetSize( (double)window_width, window_height / 3.1);
	/////////////////

	// Init Font ////
	Console_font = pFont->CreateFont( FONT_DIR "NIMBU14.TTF", 13, TTF_STYLE_BOLD );
	//

	CMDList = NULL;

	/// [Console Commands]
	Push(CMDList,	"clear",	clearcon,		"Clears all strings in console",	"clear"						);
	Push(CMDList,	"loadmap",	loadmap,		"Loads a map file into the game",	"loadmap [mapfile]"			);
	Push(CMDList,	"mx",		SetMx,			"Sets Meatball's X coordinate",		"Mx [x]"					);
	Push(CMDList,	"my",		SetMy,			"Sets Meatball's Y coordinate",		"my [y]"					);
	Push(CMDList,	"mxy",		SetMxy,			"Sets Meatball's X & Y coordinate",	"Mxy [x y]"					);
	Push(CMDList,	"play",		play,			"Plays a music file",				"play [musicfile]"			);
	Push(CMDList,	"quit",		QuitAll,		"Quits the game","quit"											);
	Push(CMDList,	"fps",		ShowFPS,		"Displays or hides FPS",			"fps"					);
	Push(CMDList,	"help",		help,			"",									""							);
	Push(CMDList,	"svol",		soundVol,		"Set Sounds Volumes",				"svol [string_id] [0-128]"	);
	Push(CMDList,	"mvol",		musicVol,		"Set Music Volumes",				"mvol [0-128]"				);
	Push(CMDList,	"allsvol",	allSoundsVol,	"Set ALL Sounds Channel Volumes",	"allsvol [0-128]"			);
	Push(CMDList,	"cd",		cd,				"change directory",					"cd [dir]"					);
	Push(CMDList,	"ls",		ls,				"List Directoy Contents",			"ls [dir]"					);
	/// [Console Commands]

	conx = 10.0;
	cony = BG->height - 14;

	DrawCur =SDL_FALSE;
	ttDrawCur = SDL_GetTicks() + 500;
}

/// Delete the Background, close the font, Delete all commands from CMDList
cConsole :: ~cConsole( void )
{
	//free(full_path);
	if( BG ) 
	{
		delete BG;
	}

	if( Console_font ) 
	{
		TTF_CloseFont( Console_font );
	}

	cCMD *ptr = CMDList;
	while (ptr != NULL)
	{
		cCMD *ptr2 = ptr->next;
		delete ptr;
		ptr = ptr2;
	}

	//full_path;

}

/// The input Handler
/// There is support for up/down input history,
/// When you hit enter, sends the input to CMDHandler()
void cConsole :: EventHandler( void )
{
	SDL_StartTextInput();
	
	while ( SDL_PollEvent( &event ) )
	{
		UniversalEventHandler(&event);
		switch ( event.type )
		{
		case SDL_QUIT:
			{
				done = 2;
				break;
			}
		case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_BACKQUOTE)
				{
					//done = 1;
					mode = oldmode;
				}

				else if ( event.key.keysym.sym == SDLK_BACKSPACE )
				{
					if ( !constr.empty() )
					{
						constr.erase( constr.end()-1 , constr.end() );
					}
				}	
				else if ( event.key.keysym.sym == SDLK_RETURN )
				{	
					if ( !constr.empty() )
					{
						for ( int i = 9; i >= 0; i-- )
						{
							if ( !strcpy[i].empty() )
							{
								strcpy[i + 1] = strcpy[i];
							}
						}

						strcpy[0] = constr;

						constr.clear(); // Clear console input line
						CMDHandler( strcpy[0] );

						
					}
				}
				else if ( event.key.keysym.sym == SDLK_UP )
				{	
					if (histcounter >= NUM_LINES)
					{	histcounter = NUM_LINES; }
					else constr = strcpy[++histcounter];
					
				}
				else if ( event.key.keysym.sym == SDLK_DOWN )
				{	
					if (histcounter <= 0)
					{	constr.clear(); histcounter = -1;}
					else constr = strcpy[--histcounter];
					
				}
				
				
				break;
			}
			case SDL_TEXTINPUT:
			{
				constr += event.text.text; // += (char)event.key.keysym.unicode;
				break;
			}
		default:
			{
				
				break;
			}
		}
	}

	SDL_StopTextInput();
}

/// Updates background, Updates logic to draw the cursor or not (blinking underscore)
void cConsole :: Update( void )
{
	PreUpdate();

	BG->Update(Renderer);

	if ( SDL_GetTicks() > ttDrawCur )
	{
		ttDrawCur = SDL_GetTicks() + 500;
		if (DrawCur == SDL_FALSE)
		{
			DrawCur = SDL_TRUE;
		}
		else{
			DrawCur = SDL_FALSE;
		}
	}

	PostUpdate();
}

/// Draws the BG, the input display and history displays, the blinking cursor
void cConsole :: Draw( SDL_Renderer *renderer )
{
	PreDraw();

	DrawBullets(renderer);
	DrawParticleEmitter(renderer);

	pPlayer->Draw( renderer );
	
	DrawEnemies(renderer);

	int i;
	// display console BG
	BG->Draw( renderer );
	
	#define topHistoryLine_Y 23.0;		// The top vertical coordinate for first line of input history
	double history_y = topHistoryLine_Y;
										// all lines are then seperated by HIST_VERTICAL_SEPERATION_PIXELS
	#define CONSOLE_HIST_VERTICAL_SEPERATION_PIXELS 15.0
	SDL_Rect conput_rect, strcpy_rect[11], cur_rect;
	
	SDL_Surface *conput = NULL;	// input display
	SDL_Texture *conput_tex = NULL;
	SDL_Surface *sc[HISTORY_LINES];		// history displays
	SDL_Texture *sc_tex[HISTORY_LINES];		// history displays
	SDL_Surface *Cur = NULL;	// Cursor Display
	SDL_Texture *Cur_tex = NULL;
	
	// Init all history displays to NULL
	for( i = 0; i < HISTORY_LINES; i++ )
	{
		sc[i] = NULL;
	}
	
	// The blinking Cursor
	if( DrawCur == SDL_TRUE)
	{
		Cur = pFont->CreateText( "_", Console_font );
	}
	else
	{
		Cur = pFont->CreateText( " ", Console_font );
	}

	// Text Creation
	if( !constr.empty() )	// if we have input in our String...
	{
		// Use pfont to create Text to the conput SDL_Surface
		conput = pFont->CreateText( constr.c_str(), Console_font );
	}

	// Fill the history Surfaces with any Text we have
	for( i=0; i < 11; i++ )
	{
		if ( !strcpy[i].empty() )
		{
			sc[i] = pFont->CreateText( strcpy[i].c_str(), Console_font );
		}
	}

	// Rect specification
	int curx = 0;
	
	// If we have input displayed
	if( conput ) 
	{
		curx += conput->w;	// Set cursor X coordinate to the end of that Surface
		conput_rect = SetRect( (int)conx, (int)cony, conput->w, conput->h );
	}
	
	cur_rect = SetRect( (int)(conx + curx), (int)cony-2, Cur->w, Cur->h );

	for( i=0; i < 11; i++ )
	{
		if ( sc[i] )
		{
			strcpy_rect[i] = SetRect( (int)conx, (int)(cony - history_y), sc[i]->w, sc[i]->h );
			history_y += CONSOLE_HIST_VERTICAL_SEPERATION_PIXELS;
		}
	}


	// the actual drawing
	//SDL_BlitSurface( Cur, NULL, target, &cur_rect );
	Cur_tex = SDL_CreateTextureFromSurface(renderer, Cur);
	SDL_RenderCopy(renderer, Cur_tex, NULL, &cur_rect);
	
	if ( conput )
	{
		//SDL_BlitSurface( conput, NULL, target, &conput_rect );
		conput_tex = SDL_CreateTextureFromSurface(renderer, conput);
		SDL_RenderCopy(renderer, conput_tex, NULL, &conput_rect);
	}

	for ( i=0; i < 11; i++ )
	{
		if ( sc[i] )
		{
			//SDL_BlitSurface( sc[i], NULL, target, &strcpy_rect[i] );
			sc_tex[i]  = SDL_CreateTextureFromSurface(renderer, sc[i]);
			SDL_RenderCopy(renderer, sc_tex[i], NULL, &strcpy_rect[i]);
		}
	}
	

	// Free all used Surfaces
	if ( Cur )
	{
		SDL_FreeSurface( Cur );
		SDL_DestroyTexture(Cur_tex);
	}

	if ( conput )
	{
		SDL_FreeSurface( conput );
		SDL_DestroyTexture(conput_tex);
	}

	for ( i=0; i < 11; i++ )
	{
		if ( sc[i] )
		{
			SDL_FreeSurface( sc[i] );
			SDL_DestroyTexture(sc_tex[i]);
		}
	}

	PostDraw();
}

/// The Command Handler parses the input line for it's base (command) and it's parameters
/// It then checks with all registered commands for a match
/// If there's a match it will call the registered command's handler function with the parameters as a string argument

/// @returnsSDL_FALSE on no match or base empty.SDL_TRUE on a match
SDL_bool cConsole :: CMDHandler( string cInput )
{
	string base = ParseBase( cInput );
	string parm = ParseParm( cInput );

	if ( base.empty() )
	{
		cout<<"base empty\n";
		return SDL_FALSE;
	}

	cCMD *ptr = CMDList;

	while ( ptr != NULL )
	{
		if ( base == ptr->command || base.substr(1) == ptr->command )
		{
			ptr->handler( parm );
			return SDL_TRUE;
		}

		ptr = ptr->next;
	}

	return SDL_FALSE;
}

/// Strips the input line for only the base (first word)
/// Strips by finding the first space character
string cConsole :: ParseBase( string str )
{
	size_t found = str.find_first_of( ' ' );
	if (found == string::npos)
		return str;

	string::iterator beginning = str.begin() + found;
	string::iterator ending	= str.end();

	str.erase( beginning, ending );
	cout<<str;
	return str;
}

/// Finds the parameter by skipping the first word and first space character in the string
/// The rest goes into the Parameter string, which is returned
string cConsole :: ParseParm( string str )
{
	string empty("");
	size_t found = str.find_first_of( ' ' );

	if (found == string::npos)
		return empty;
	string::iterator beginning = str.begin();
	string::iterator ending = str.begin() + found+1;

	str.erase( beginning, ending );
	cout<<str;
	return str;
}

SDL_bool cConsole :: helpCMD( string &str )
{
	char buffer[1000];

	//string::iterator pos = str.end() - 1;

	//cout<<(int)*pos;
	//str.erase( pos );
	
	sprintf( buffer, "Revealing information on CMD %s:", str.c_str() );

	//int j = 0;
	cCMD *ptr = CMDList;
	while ( ptr != NULL )
	{
		if ( ptr->command == str)
		{
			console_print(buffer);
			console_print(ptr->helpstr.c_str());

			string usage = "usage: " + ptr->syntax;
			console_print(usage.c_str());
			
			return SDL_TRUE;
		}

		ptr = ptr->next;
	}

	return SDL_FALSE;
}






SDL_bool clearcon( string &str )
{
	pConsole->constr.clear();

	for ( int i = 0; i < 11; i++ )
	{
		pConsole->strcpy[i].clear();
	}

	pConsole->histcounter = -1;
	return SDL_TRUE;
}

SDL_bool loadmap( string &str )
{
	pLevel->Load( str );

	return SDL_TRUE;
}

SDL_bool SetMx( string &str )
{
	if (str.empty())
	{
		stringstream var;
		
		var << pPlayer->posx;
		console_print(var.str().c_str());
	}
	else{
		pPlayer->posx = atoi( str.c_str() );
	}
	return SDL_TRUE;
}

SDL_bool SetMy( string &str )
{
	if (str.empty())
	{
		stringstream var;
		
		var << pPlayer->posy;
		console_print(var.str().c_str());
	}
	else{
		pPlayer->posy = atoi( str.c_str() );
	}

	return SDL_TRUE;
}

SDL_bool SetMxy( string &str )
{
	if (str.empty())
	{
		stringstream x,y;
		
		x << "X: "<<pPlayer->posx;
		console_print(x.str().c_str());
		//y.clear();
		y <<"Y: "<<pPlayer->posy;
		
		console_print(y.str().c_str());
		
		return SDL_TRUE;
	}
	else{
	
		size_t found;
		string x,y;
		string::iterator xi, yi;

		// parsing a int,int string combo
		xi = str.begin();

		found = str.find_first_of( ' ' );
		if (found == string::npos)
		{
			return SDL_FALSE;
		}
		yi = str.begin() + found;

	// Very incompetent String parsing here. The prog will crash if you suck at typing

		x.assign(xi,yi++);
		y.assign(yi, str.end());

		cout << x << " " << y;

		pPlayer->SetPos( atoi( x.c_str() ), atoi( y.c_str() ) );
	

		return SDL_TRUE;
	}
}

SDL_bool allSoundsVol(string &str)
{
	int avg = pAudio->SetAllSoundsVolume(atoi(str.c_str()));

	if (avg == -1)
	{
		console_print("Error. Volume invalid");
		return SDL_FALSE;
	}
	char result[300];
	sprintf(result,"Successful. Avg/Old volume = %d", avg);
	console_print(result);
	return SDL_TRUE;
}

SDL_bool soundVol( string &str)
{
	if (str.empty())
	{
		stringstream var;
		
		var << pAudio->Sound_Volume;
		console_print(var.str().c_str());
		
		return SDL_TRUE;
	}
	else
	{
		size_t found;
		string sound,vol;
		string::iterator soundi, voli;

		// parsing a int,int string combo
		soundi = str.begin();
		sound.assign(soundi,str.end());

		found = str.find_first_of( ' ' );
		if (found == string::npos)
		{
			stringstream soundvol;
			
			soundvol<<pAudio->GetSoundVolume(SMan->GetPointer(sound));
			console_print(soundvol.str().c_str());
			
			return SDL_TRUE;
		}
		
		
		
		voli = str.begin() + found;

		// Very incompetent String parsing here. The prog will crash if you suck at typing

		
		vol.assign(voli, str.end());

		cout << sound << " " << vol;

		int oldvol = pAudio->SetSoundVolume(SMan->GetPointer(sound), atoi(vol.c_str()));

		if (oldvol == -1)
		{
			console_print("Error Changing Sound Volume!");
			return SDL_FALSE;
		}
		else 
		{
			char Result[200]; // string which will contain the number

			sprintf(Result,"%s_vol : %d => %s", sound.c_str(), oldvol,vol.c_str() ); // %d makes the result be a decimal integer 

			console_print(Result);
		}
		//pAudio->SetSoundsVolume(atoi( str.c_str() ));
		return SDL_TRUE;
	}
}

SDL_bool musicVol( string &str)
{
	if (str.empty())
	{
		stringstream var;
		
		var << pAudio->Music_Volume;
		console_print(var.str().c_str());
		return SDL_TRUE;
	}
	else{
		int oldvol = pAudio->SetMusicVolume(atoi( str.c_str() ));
		char result[100];

		if (oldvol == -1)
		{
			console_print("Invalid Volume!");
		}
		else
		{
			sprintf(result,"Music_vol : %d => %s", oldvol, str.c_str());
			console_print(result);
		}
		return SDL_TRUE;
	}
}

SDL_bool play( string &str )
{
	pLevel->pLevelData->Musicfile = str;

	string file = MUSIC_DIR + str;

	if ( !FileValid( file ) )
	{
		console_print("Error: Music File does not Exist! OR it's already being played ;)");
		return SDL_FALSE;
	}
	else
	{
			pAudio->PlayMusik( file.c_str(), SDL_TRUE, SDL_TRUE );
	}

	return SDL_TRUE;
}

SDL_bool ShowFPS( string &str )
{

	if (fps_display == SDL_FALSE)
	{
		fps_display = SDL_TRUE;
	}
	else{
		fps_display = SDL_FALSE;
	}
	
	return SDL_TRUE;
}

SDL_bool help( string &str )
{
	int curline = 0;

	if ( str.empty() )
	{		
		/** there are now more than 10 commands... too much for a screen full...
			let's print 10 commands at a time then */
		cCMD *ptr = pConsole->CMDList;
		while ( ptr != NULL )
		{
			if (curline >= (NUM_LINES))
			{
				
				pConsole->constr = "Press any key..";
				
				wait_for_input();
				
				
				pConsole->constr.clear();
			}
			 

			console_print((char*)ptr->command.c_str());
			ptr = ptr->next;
			curline++;
		}

		
		return SDL_TRUE;

	}
	else
	{
		return pConsole->helpCMD( str );
	}
}

void moveup()
{
	int i;
	for (i=NUM_LINES-2; i >= 0; i--)
	{
		pConsole->strcpy[i+1] = pConsole->strcpy[i];
	}
}

SDL_bool QuitAll( string &str )
{
	done = 1;

	return SDL_TRUE;
}

void wait_for_input()
{
	SDL_Event local_event;
	while (1)
	{
		pConsole->Update();
		pConsole->Draw(Renderer);
		SDL_PollEvent( &local_event );

		if ( local_event.type == SDL_KEYDOWN )
		{
			break;
		}
	
	}
}

void cConsole::Draw()
{
	Draw(Renderer);
}

void console_print(const char *str)
{
	moveup();
	pConsole->strcpy[0] = str;
}


SDL_bool cd(string &str)
{
	if (str.empty())
	{
		console_print(pConsole->full_path.string().c_str());
		return SDL_TRUE;
	}
	pConsole->full_path = fs::system_complete(fs::path(pConsole->full_path.string() +"/"+str));
	console_print((char*)pConsole->full_path.string().c_str());
	return SDL_TRUE;
}

SDL_bool ls(string &str)
{
	// 1st we won't factor the String parameter
	int curline = 0;
	if ( fs::is_directory( pConsole->full_path ) )
	{
		fs::directory_iterator end_iter;
		char result[500];
		for ( fs::directory_iterator dir_itr( pConsole->full_path ); dir_itr != end_iter; ++dir_itr )
		{
			if (curline >= (NUM_LINES))
			{
				
				pConsole->constr = "Press any key..";
				
				wait_for_input();
				
				pConsole->constr.clear();
			}

			string s(dir_itr->path().filename().string());
			try
			{
				if ( fs::is_directory( dir_itr->status() ) )
				{
					sprintf(result,"%s [directory]", s.c_str());
					console_print(result);
				}
				else if ( fs::is_regular_file( dir_itr->status() ) )
				{
					
					console_print((char*)s.c_str());
				}
				else
				{
					sprintf(result,"%s [other]", s.c_str());
					console_print(result);
				}
				curline++;
	
			}
			  catch ( const std::exception & ex )
			  {
				std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
			  }
		}
		
	}

	return SDL_TRUE;
}