/// @addtogroup Console
// @{
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Globals.h"

#include "boost.h"

/// @defgroup Console_Commands Console Commands
/// These functions are for the console commands
/// @note The function names may differ from the actual command names you type in the console
/// <br>Use the following code snippet to decipher the function names from their command names\
/// <br><br><b>Format</b>: cConsole::CMDList, "Command Name", function name, "help string", "syntax"
/// @snippet console.cpp Console Commands
// @{
bool clearcon( string &str );	///< Clears the console
bool loadmap( string &str );	///< Load a new Level from @link LEVEL_DIR @endlink
bool SetMx( string &str );		///< Set Meatball's X Coordinate
bool SetMy( string &str );		///< Set Meatball's Y Coordinate
bool SetMxy( string &str );		///< Set Meatball (X,Y) coordinate
bool play( string &str );		///< Play a song from the @link MUSIC_DIR @endlink
bool QuitAll( string &str);		///< Quit the Game
bool ShowFPS( string & str);	///< Toggle FPS framerate to be displayed
bool help( string &str );		///< bring up help on all commands or specific command
bool soundVol(string &str);		///< change the volume of a sound (0-128)
bool allSoundsVol(string &str);	///< Change the volume of All sounds (0-128)
bool musicVol(string &str);		///< Change the music volume (0-128)
bool cd(string &str);			///< change the current directory
bool ls(string &str);			///< list the directory
// @}

/// Number of Output Lines to the Console
#define NUM_LINES 11

/// Move all output up one line
void moveup();
/// Queries the user to hit enter key.. kinda like a cheap [less] cmd
void wait_for_input();
/// Self Explanatory
void console_print(const char *str);


/// This structure is for each Command to the Console
struct cCMD
{
	bool ( *handler )( string &str );	///< Function pointer to Command's function

	string command;						///< The string to identify the command
	string helpstr;						///< This string is shown when you do "help [command]"
	string syntax;						///< The syntax to the command. Shown when you ask for help
	cCMD *next;							///< next command in the linked list
};

/** Linked List push function. */
/// This pushes a new cCMD onto the linked list: @see cConsole::CMDList
void Push(cCMD*& head, string command, bool (*handler)(string &), string helpstr, string syntax);

/// For Live In-game editing to the Game World and Variables
class cConsole : public MiniEngine
{
public:

	cConsole( void );
	~cConsole( void );
	
	/** Handles console input.
	It's as simple as that */
	/// @ingroup Console_Input
	void EventHandler( void );

	bool CMDHandler( string cInput );
	void Update( void );
	
	void Draw( SDL_Surface *target );
	void Draw();

	string ParseBase( string str );
	string ParseParm( string str );

	bool helpCMD( string &str );

	cCMD *CMDList;				// The commands

	string strcpy[NUM_LINES];	// history strings (past entered commands which are displayed)

	string constr;				// main string where input is entered into.
	int histcounter;
	fs::path full_path;


	double conx, cony;			// x coord and y coord for str's, x coord for cursor

	bool DrawCur;
	double ttDrawCur;

	cBasicSprite *BG;

	TTF_Font *Console_font;
};



#endif
// @}