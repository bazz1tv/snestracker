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


// Class declarations
class TextObject;
class TextObjectEngine;

// Icon
extern SDL_Surface* icon;



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
extern bool paused;

// Fonts
extern TTF_Font *Menu_Font;
extern TTF_Font *bold_16;

/// the Screen
extern SDL_Surface *Screen;

/// Keyboard Keystate
extern Uint8 *keys;

/// events
extern SDL_Event event;

/// fps display boolean
extern bool fps_display;

/// Magneta (default) Colorkey
extern Uint32 colorkey;

// Game Global Functions
void InitObjects( void );
void QuitGame( void );


/// Checks if the file exists
bool FileValid( string filename );
/// Checks if it's an Number
bool is_valid_number( char *c );
/// Draws the Framerate
void FramerateDraw( SDL_Surface *target, double posx = 5.0, double posy = 5.0 );

// includes

#include "Game.h"
#include "Misc.h"
#include "Preferences.h"
#include "Camera.h"
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

#include "ObjectManager.h"
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


/// screen settings
extern const SDL_VideoInfo* screeninfo;

/// The Current mode
extern Uint8 mode, oldmode;

#endif