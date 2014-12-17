/// @addtogroup Console
// @{
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Globals.h"

#include "boost.h"
#include "MiniEngine.h"
#include "ObjectManager.h"

class cConsole;
extern cConsole *pConsole;



/// @defgroup Console_Commands Console Commands
/// These functions are for the console commands
/// @note The function names may differ from the actual command names you type in the console
/// <br>Use the following code snippet to decipher the function names from their command names\
/// <br><br><b>Format</b>: cConsole::CMDList, "Command Name", function name, "help string", "syntax"
/// @snippet console.cpp Console Commands
// @{
SDL_bool clearcon( string &str );	///< Clears the console
SDL_bool loadmap( string &str );	///< Load a new Level from @link DIR_LEVEL @endlink
SDL_bool SetMx( string &str );		///< Set Meatball's X Coordinate
SDL_bool SetMy( string &str );		///< Set Meatball's Y Coordinate
SDL_bool SetMxy( string &str );		///< Set Meatball (X,Y) coordinate
SDL_bool play( string &str );		///< Play a song from the @link DIR_MUSIC @endlink
SDL_bool QuitAll( string &str);		///< Quit the Game
SDL_bool ShowFPS( string & str);	///< Toggle FPS framerate to be displayed
SDL_bool help( string &str );		///< bring up help on all commands or specific command
SDL_bool soundVol(string &str);		///< change the volume of a sound (0-128)
SDL_bool allSoundsVol(string &str);	///< Change the volume of All sounds (0-128)
SDL_bool musicVol(string &str);		///< Change the music volume (0-128)
SDL_bool cd(string &str);			///< change the current directory
SDL_bool ls(string &str);			///< list the directory
SDL_bool SetScreenScaleXY(string &str);
SDL_bool SetScreenScaleX(string &str);
SDL_bool SetScreenScaleY(string &str);
SDL_bool SaveLevel(string &str);
SDL_bool camx(string &str);
SDL_bool camy(string &str);
// @}

#define HISTORY_LINES 11
#define topHistoryLine_Y 23.0	// The top vertical coordinate for first line of input history. gets subtracted from console_input Y
// all lines are then seperated by HIST_VERTICAL_SEPERATION_PIXELS
#define CONSOLE_HISTORY_VERTICAL_SEPERATION_PIXELS 15.0

/// Move all output up one line
void moveConsoleHistoryLinesUp(int nlines=1);
/// Queries the user to hit enter key.. kinda like a cheap [less] cmd
void wait_for_input();
/// Self Explanatory
void console_print(const char *str);


/// This structure is for each Command to the Console
class cCMD
{
public:
	cCMD() : command(OM_OBLITERATE_OBJS_AT_DESTROY, OM_DELETE_OBJS) { next = NULL; }
	SDL_bool ( *handler )( string &str );	///< Function pointer to Command's function

	//string command;						///< The string(s) to identify the command
	ObjectManager<string> command;
	string helpstr;						///< This string is shown when you do "help [command]"
	string syntax;						///< The syntax to the command. Shown when you ask for help
	cCMD *next;							///< next command in the linked list
};

/** Linked List push function. */
/// This pushes a new cCMD onto the linked list: @see cConsole::CMDList
void Push(cCMD*& head, string command, SDL_bool (*handler)(string &), string helpstr, string syntax);

/// For Live In-game editing to the Game World and Variables
class cConsole : public MiniEngine
{
public:

	cConsole( void );
	~cConsole( void );
	
	/** Handles console input.
	It's as simple as that */
	/// @ingroup Console_Input
	int EventHandler( void );

	SDL_bool CMDHandler( string cInput );
	cCMD* FindMatch(string &cmd, string &match);
	void Update( void );
	
	void Draw( SDL_Renderer *renderer );
	void Draw();

	string ParseBase( string str );
	string ParseParm( string str );

	SDL_bool helpCMD( string &str );

	cCMD *CMDList;									// The commands

	string strcpy[HISTORY_LINES];					// history strings (past entered commands which are displayed)

	string consoleInput_str;						// main string where input is entered into.
	int histcounter;
	fs::path full_path;


	double consoleInput_x, consoleInput_y;			// x coord and y coord for str's, x coord for cursor
	//int mode;
	SDL_bool DrawCur;
	double ttDrawCur;

	cBasicSprite *BG;

	TTF_Font *Console_font;
	
	double history_y;								// = topHistoryLine_Y;
	
	SDL_Rect consoleInput_rect, strcpy_rect[11], cursor_rect;
	
	SDL_Surface *consoleInput_surface;				// input display
	SDL_Texture *consoleInput_tex;
	SDL_Surface *sc_surface[HISTORY_LINES];			// history displays
	SDL_Texture *sc_tex[HISTORY_LINES];				// history displays
	SDL_Surface *cursor_surface;					// Cursor Display
	SDL_Texture *cursor_tex;
	
	void ClearHistory_SurfacesAndTextures();
	void DrawCursor();
	
	void CreateTextOnSurfaces();
	void SetConsoleLineRects();
	void CreateTexturesAndRender(SDL_Renderer *renderer);
	// Free all used Surfaces
	void FreeAllUsedSurfaces();
};



#endif
// @}