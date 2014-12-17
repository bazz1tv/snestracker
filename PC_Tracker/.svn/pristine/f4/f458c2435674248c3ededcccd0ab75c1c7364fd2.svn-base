#ifndef __GLOBALS_H__
#define __GLOBALS_H__

// You can define DEMO mode to do specific things for when demoing to your friends :) 
//#define DEMO

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fstream>

#include "OS.h"
// Global Big Definitions (important)
#include "defines.h"

#ifdef _DEBUG
// Update the following if needed…
#define DEBUGLOG(...) printf(__VA_ARGS__)
#else
#define DEBUGLOG(...) do {} while (0)
#endif


// Class declarations
class TextObject;
//class MiniEngine;

// Icon
//extern SDL_Surface* icon;



/// @addtogroup Engine_Meatball
/// These are classes Meatball uses from the SDL_ep Engine Library.
/// SDL_ep is the Home-grown engine that Meatball is built on. IT's still
/// in development. Since it kind of develops alongside of meatball. It stands for Engine Primitives
// @{
extern cFramerate *pFramerate;
extern cImageManager *IMan;
extern cSoundManager *SMan;
extern cFont *pFont;
extern cAudio *pAudio;
extern cTCPNet *pTCP;
// @}

/// finished with game or not?
extern int done;

/// game paused?
extern SDL_bool paused;

// Fonts
extern TTF_Font *Menu_Font;
extern TTF_Font *bold_16;

/// the Screen
extern SDL_Window *Window;
extern int window_width;
extern int window_height;

extern SDL_Renderer *Renderer;

/// Keyboard Keystate
extern Uint8 *keys;

/// events
extern SDL_Event event;

/// fps display SDL_boolean
extern SDL_bool fps_display;

/// Magenta (default) Colorkey
extern Uint32 colorkey;

// Game Global Functions
void InitObjects( void );
void QuitGame( void );


/// Checks if the file exists
SDL_bool FileValid( string filename );
/// Checks if it's an Number
SDL_bool is_valid_number( char *c );
/// Draws the Framerate
void FramerateDraw( SDL_Renderer *renderer, double posx = 5.0, double posy = 5.0 );

// includes


#include "Game.h"
#include "MiniEngine.h"
#include "Misc.h"
#include "Preferences.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "Range.h"
#include "Collision.h"
#include "sprite.h"
#include "effects.h"
#include "level.h"
#include "weapon.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "leveleditor.h"
#include "console.h"

#include "GraphicsObject.h"
#include "TextObject.h"
#include "ButtonObject.h"

#include "Slider.h"
#include "OptionsMenu.h"
#include "MainMenu.h"





//Enemies
#include "AF-373.h"

// MeatBall
extern cPreferences *pPreferences;	// The Preferences
extern cSettings *pGameSettings;	// The Game Settings
extern cMouseCursor *pMouse;		// The Mouse
extern cCamera *pCamera;			// The Camera
extern cPlayer *pPlayer;			// The Player
extern cLevel *pLevel;				// The Level
extern cLevelEditor *pLevelEditor;	// The Leveleditor
extern cConsole *pConsole;			// The Console

/// The Current mode
extern Uint8 mode, oldmode;

#endif