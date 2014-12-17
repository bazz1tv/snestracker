
#include "Globals.h"
#include "console.h"
#include <string.h>
#include <sstream>
#include "Misc.h"
#include "bullet.h"
#include "enemy.h"
#include "effects.h"
#include "player.h"

extern cConsole *pConsole;


using namespace std;


void Push(cCMD*& head, string command, SDL_bool (*handler)(string &), string helpstr, string syntax)
{
	cCMD* newCmd = new cCMD;
	newCmd->handler = handler;
	newCmd->syntax = syntax;
	newCmd->helpstr = helpstr;
	newCmd->command.add(new string(command));
	newCmd->next = head; 
	head = newCmd;
}

void CmdAddAlias(cCMD* &cmd, string str)
{
	cmd->command.add(new string(str));
}

/// Gets the current working path, Loads the Background image to the Image Manager,
/// Creates the console font, Push() all cCMDs to cConsole::CMDList, 
cConsole :: cConsole( void )
{
	// clear surfaces
	consoleInput_surface = NULL;	// input display
	consoleInput_tex = NULL;
	for (Uint8 n=0; n < HISTORY_LINES; n++)
	{
		sc_surface[n] = NULL;
		sc_tex[n] = NULL;
	}
	cursor_surface = NULL;	// Cursor Display
	cursor_tex = NULL;
	
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

	// No Limits
	
	/// [Console Commands]
	Push(CMDList,	"clear",	clearcon,				"Clears all strings in console",						"clear"												);
		CmdAddAlias(CMDList, "cls");
	Push(CMDList,	"loadmap",	loadmap,				"Loads a map file into the game",						"loadmap [mapfile]"									);
	Push(CMDList,	"mx",		SetMx,					"Sets Meatball's X coordinate",							"Mx [x]"											);
	Push(CMDList,	"my",		SetMy,					"Sets Meatball's Y coordinate",							"my [y]"											);
	Push(CMDList,	"mxy",		SetMxy,					"Sets Meatball's X & Y coordinate",						"Mxy [x y]"											);
	Push(CMDList,	"playmus",		play,					"Plays a music file",									"play [musicfile]"									);
	Push(CMDList,	"quit",		QuitAll,				"Quits the game",										"quit"												);
		CmdAddAlias(CMDList, "q");
	Push(CMDList,	"fps",		ShowFPS,				"Displays or hides FPS",								"fps"												);
	Push(CMDList,	"help",		help,					"Displays all commands or help for a specific command",	"help [cmd]"										);
		CmdAddAlias(CMDList, "h");
	Push(CMDList,	"svol",		soundVol,				"Set Sounds Volumes",									"svol [string_id] [0-128]"							);
	Push(CMDList,	"mvol",		musicVol,				"Set Music Volumes",									"mvol [0-128]"										);
	Push(CMDList,	"allsvol",	allSoundsVol,			"Set ALL Sounds Channel Volumes",						"allsvol [0-128]"									);
	Push(CMDList,	"cd",		cd,						"change directory",										"cd [dir]"											);
	Push(CMDList,	"ls",		ls,						"List Directoy Contents",								"ls [dir]"											);
		CmdAddAlias(CMDList, "l");
	Push(CMDList,	"sxy",		SetScreenScaleXY,		"Set X/Y Screen Scale Factor",							"sxy [x] [y]"										);
	Push(CMDList,	"sx",		SetScreenScaleX,		"Set X Screen Scale Factor",							"sx [x]"											);
	Push(CMDList,	"sy",		SetScreenScaleY,		"Set Y Screen Scale Factor",							"sy [y]"											);
	/// [Console Commands]

	consoleInput_x = 10.0;
	consoleInput_y = BG->height - 14;

	DrawCur = SDL_FALSE;
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
					if ( !consoleInput_str.empty() )
					{
						consoleInput_str.erase( consoleInput_str.end()-1 , consoleInput_str.end() );
					}
				}	
				else if ( event.key.keysym.sym == SDLK_RETURN )
				{	
					if ( !consoleInput_str.empty() )
					{
						console_print(consoleInput_str.c_str());
						consoleInput_str.clear(); // Clear console input line
						CMDHandler( strcpy[0] );
					}
				}
				else if ( event.key.keysym.sym == SDLK_UP )
				{	
					if (histcounter >= NUM_LINES)
					{	histcounter = NUM_LINES; }
					else consoleInput_str = strcpy[++histcounter];
					
				}
				else if ( event.key.keysym.sym == SDLK_DOWN )
				{	
					if (histcounter <= 0)
					{	consoleInput_str.clear(); histcounter = -1;}
					else consoleInput_str = strcpy[--histcounter];
					
				}
				
				
				break;
			}
			case SDL_TEXTINPUT:
			{
				consoleInput_str += event.text.text; // += (char)event.key.keysym.unicode;
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

	
	// display console BG
	BG->Draw( renderer );
	
	BlinkCursor();
	CreateTextOnSurfaces();
	SetRects();
	CreateTexturesAndRender(renderer);
	FreeAllUsedSurfaces();

	PostDraw();
	
	// Free all used Surfaces
}

void cConsole::BlinkCursor()
{
	// The blinking Cursor
	if( DrawCur == SDL_TRUE)
	{
		cursor_surface = pFont->CreateText( "_", Console_font );
	}
	else
	{
		cursor_surface = pFont->CreateText( " ", Console_font );
	}
}

void cConsole :: CreateTextOnSurfaces()
{
	// Text Creation
	if( !consoleInput_str.empty() )	// if we have input in our String...
	{
		// Use pfont to create Text to the consoleInput SDL_Surface
		consoleInput_surface = pFont->CreateText( consoleInput_str.c_str(), Console_font );
	}
	
	// Fill the history Surfaces with any Text we have
	for(int i=0; i < 11; i++ )
	{
		if ( !strcpy[i].empty() )
		{
			sc_surface[i] = pFont->CreateText( strcpy[i].c_str(), Console_font );
		}
	}
}

void cConsole :: SetRects()
{
	history_y = topHistoryLine_Y;
	// Rect specification
	int cursorx = 0;
	// If we have input displayed
	if( consoleInput_surface )
	{
		cursorx += consoleInput_surface->w;	// Set cursor X coordinate to the end of that Surface
		consoleInput_rect = SetRect( (int)consoleInput_x, (int)consoleInput_y, consoleInput_surface->w, consoleInput_surface->h );
	}
	
	cursor_rect = SetRect( (int)(consoleInput_x + cursorx), (int)consoleInput_y-2, cursor_surface->w, cursor_surface->h );
	
	for(int i=0; i < 11; i++ )
	{
		if ( sc_surface[i] )
		{
			strcpy_rect[i] = SetRect( (int)consoleInput_x, (int)(consoleInput_y - history_y), sc_surface[i]->w, sc_surface[i]->h );
			history_y += CONSOLE_HISTORY_VERTICAL_SEPERATION_PIXELS;
		}
	}
	
}


void cConsole :: CreateTexturesAndRender(SDL_Renderer *renderer)
{
	cursor_tex = SDL_CreateTextureFromSurface(renderer, cursor_surface);
	SDL_RenderCopy(renderer, cursor_tex, NULL, &cursor_rect);
	
	if ( consoleInput_surface )
	{
		//SDL_BlitSurface( consoleInput, NULL, target, &consoleInput_rect );
		consoleInput_tex = SDL_CreateTextureFromSurface(renderer, consoleInput_surface);
		SDL_RenderCopy(renderer, consoleInput_tex, NULL, &consoleInput_rect);
	}
	
	for (int i=0; i < 11; i++ )
	{
		if ( sc_surface[i] )
		{
			//SDL_BlitSurface( sc[i], NULL, target, &strcpy_rect[i] );
			sc_tex[i]  = SDL_CreateTextureFromSurface(renderer, sc_surface[i]);
			SDL_RenderCopy(renderer, sc_tex[i], NULL, &strcpy_rect[i]);
		}
	}
}

void cConsole :: FreeAllUsedSurfaces()
{
	if ( cursor_surface )
	{
		SDL_FreeSurface( cursor_surface);
		cursor_surface = NULL;
	}
	if (cursor_tex)
	{
		SDL_DestroyTexture(cursor_tex);
		cursor_tex = NULL;
	}
	
	if ( consoleInput_surface )
	{
		SDL_FreeSurface( consoleInput_surface );
		consoleInput_surface = NULL;
	}
	if (consoleInput_tex)
	{
		SDL_DestroyTexture(consoleInput_tex);
		consoleInput_tex = NULL;
	}
	
	ClearHistory_SurfacesAndTextures();
	
}


void cConsole :: ClearHistory_SurfacesAndTextures()
{
	for (int i=0; i < HISTORY_LINES; i++ )
	{
		if ( sc_surface[i] )
		{
			SDL_FreeSurface( sc_surface[i] );
			sc_surface[i] = NULL;
		}
		if (sc_tex[i])
		{
			SDL_DestroyTexture(sc_tex[i]);
			sc_tex[i] = NULL;
		}
	}
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

	cCMD *cmd_match = FindMatch(base);

	if (cmd_match)
	{
		cmd_match->handler( parm );
		return SDL_TRUE;
	}
		
	return SDL_FALSE;
}

cCMD* cConsole::FindMatch(string &cmd)
{
	cCMD *ptr = CMDList;
	
	while ( ptr != NULL )
	{
		for (unsigned int i=0; i < ptr->command.objcount; i++)
		{
			if ( cmd == (string)*(ptr->command.objects[i]) )
			{
				return ptr;
			}
			else
			{
				if (cmd.c_str()[0] == '/')
					if (cmd.substr(1) == (string)*(ptr->command.objects[i]))
						return ptr;
			}
		}
		
		ptr = ptr->next;
	}
	
	return NULL;
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
		for (unsigned int i=0; i < ptr->command.objcount; i++)
		{
			if ( (string)*ptr->command.objects[i] == str)
			{
				console_print(buffer);
				console_print(ptr->helpstr.c_str());
				
				string usage = "usage: " + ptr->syntax;
				console_print(usage.c_str());
			
				return SDL_TRUE;
			}
		}

		ptr = ptr->next;
	}

	return SDL_FALSE;
}






SDL_bool clearcon( string &str )
{
	pConsole->consoleInput_str.clear();

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
	pLevel->pLevelData_Layer1->Musicfile = str;

	string file = MUSIC_DIR + str;

	if ( !FileValid( file ) )
	{
		console_print("Error: Music File does not Exist!");
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
		// We print a screenful, then one line at a time after that, each line after use presses a key, like less
		// To-Do : add "held" key feature to speed up scrolling
		cCMD *ptr = pConsole->CMDList;
		while ( ptr != NULL )
		{
			// if we have printed >= a screenful..let's stop to let the user analyze, and press a key to continue
			// Todo: add code to process Holding a key instead of just pressing
			if (curline >= (NUM_LINES))
			{
				
				pConsole->consoleInput_str = "Press any key..";
				
				wait_for_input();
				
				
				pConsole->consoleInput_str.clear();
			}
			 
			// The following code will create a single-line string of all the aliases for the one command
			string allcmds="";
			unsigned int i;
			for (i=0; i < ptr->command.objcount-1; i++)
			{
				
				allcmds = *ptr->command.objects[i] + ", ";
				
			}
			allcmds += *ptr->command.objects[i];
			console_print((char*)allcmds.c_str());
			// print that one line, then move on
			
			curline++;
			ptr = ptr->next;
			
		}

		
		return SDL_TRUE;

	}
	else
	{
		return pConsole->helpCMD( str );
	}
}

void moveConsoleHistoryLinesUp(int nlines/*=1*/) // number of lines to move up by
{
	int i;
	
	for (i=NUM_LINES-1; i >= nlines; i--)
	{
		pConsole->strcpy[i] = pConsole->strcpy[i-nlines];
	}
		
		
	while(--nlines >= 0)
	{
		pConsole->strcpy[nlines--] = " ";
	}
}

SDL_bool QuitAll( string &str )
{
	done = 1;
	mode = MODE_QUIT;
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
		
		// Todo: Check for held down keys as well..
		
	
	}
}

void cConsole::Draw()
{
	Draw(Renderer);
}

void console_print(const char *str)
{
	moveConsoleHistoryLinesUp(1);
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
				
				pConsole->consoleInput_str = "Press any key..";
				
				wait_for_input();
				
				pConsole->consoleInput_str.clear();
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

SDL_bool SetScreenScaleXY(string &str)
{
	// parse command, seperate arguments
	if (str.empty())
	{
		stringstream x,y;
		
		x << "X: "<<ScreenScale.x;
		console_print(x.str().c_str());
		//y.clear();
		y <<"Y: "<<ScreenScale.y;
		
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
		
		SetScreenScale(atoi( x.c_str() ), atoi( y.c_str() ));
		
		
		return SDL_TRUE;
	}
}
SDL_bool SetScreenScaleX(string &str)
{
	// use first argument and only argument
	if (str.empty())
	{
		//console_print(pConsole->full_path.string().c_str());
		return SDL_FALSE;
	}
	int ScreenScale_x_logged = ScreenScale.x;
	ScreenScale.x = atoi(str.c_str());
	
	char result[300];
	sprintf(result,"Changed ScreenScale_x from %d to %d", ScreenScale_x_logged, ScreenScale.x);
	
	console_print(result);
	return SDL_TRUE;
}
SDL_bool SetScreenScaleY(string &str)
{
	// use first argument and only argument
	if (str.empty())
	{
		//console_print(pConsole->full_path.string().c_str());
		
		return SDL_FALSE;
	}
	int ScreenScale_y_logged = ScreenScale.y;
	ScreenScale.y = atoi(str.c_str());
	
	char result[300];
	sprintf(result,"Changed ScreenScale_y from %d to %d", ScreenScale_y_logged, ScreenScale.y);
	
	console_print(result);
	return SDL_TRUE;
}