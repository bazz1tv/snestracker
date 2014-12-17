#ifndef __DEFINES_H__
#define __DEFINES_H__

#define APP_TITLE "MeatBall - Vegetable Destruction"
// GAME-WIDE

// DEMO MODE | IN GAME LEVEL EDITOR
//#define DEMO
#define INGAME_LEVEL_EDITOR

#define MEATBALL_CUSTOM_FLAGS (SDL_HWSURFACE | SDL_HWACCEL | SDL_RLEACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE)
//#define MEATBALL_WIN_SETTINGS pGameSettings->Screen_W, pGameSettings->Screen_H, pGameSettings->Screen_Bpp
//#define MEATBALL_FULLSCREEN_SETTINGS pGameSettings->Screen_W, pGameSettings->Screen_H, pGameSettings->Screen_Bpp

/* The PLAYER */
/// The player won't fall any faster than this (pixels) per frame
#define MAX_FALL_VELOCITY 8
#define JUMP_KEYS_HELD		(keys[SDL_GetScancodeFromKey(pGameSettings->Key_jump)]) // || keys[SDLK_RALT] || keys[SDLK_SEMICOLON] || keys[SDLK_PERIOD] || keys[SDLK_m])
#define MOVELEFT_KEYS_HELD	(keys[SDL_GetScancodeFromKey(pGameSettings->Key_walk_left)]) //  || keys[SDLK_a])
#define MOVERIGHT_KEYS_HELD (keys[SDL_GetScancodeFromKey(pGameSettings->Key_walk_right)]) //|| keys[SDLK_d])
#define JUMP_KEYS_TRIGGERED (event->key.keysym.sym == (pGameSettings->Key_jump)) // || event.key.keysym.sym == (SDLK_RALT) ||event.key.keysym.sym == SDLK_SEMICOLON || event.key.keysym.sym == SDLK_PERIOD || event.key.keysym.sym == SDLK_m)
#define FIRE_KEY_TRIGGERED	(event->key.keysym.sym == (pGameSettings->Key_shoot_primary))
#define FIRE_KEY_HELD	(keys[SDL_GetScancodeFromKey(pGameSettings->Key_shoot_primary)])

// The Directorys
#define PIXMAPS_DIR "data/pixmaps/"
#define MUSIC_DIR "data/music/"
#define SOUNDS_DIR "data/sound/"
#define FONT_DIR "data/font/"
#define LEVEL_DIR "data/levels/"

// Sprite Directions
/// @ingroup PLAYER_DIRECTIONS
/// @{
#define LEFT	0
#define RIGHT	1
#define UP		2
#define DOWN	3
#define NONE	4
// @}

/// @defgroup PLAYER_STATES
// @{
#define STATE_STAYING 0
#define STATE_WALKING 1
#define STATE_JUMPING 2
#define STATE_FALLING 3
#define STATE_FLYING  4	///< Not implemented
// @}

// Modes
/// @addtogroup GAME_MODE
/// This is a list of the current different types of Game Modes available
// @{
#define MODE_MAINMENU		0
#define MODE_GAME			1
#define MODE_LEVELEDITOR	2
#define MODE_CONSOLE		3
#define MODE_OPTIONS		4
#define MODE_QUIT			5
#define MODE_DEBUG			6		///< Not implemented

#define NUM_GAMEMODES		7
// @}

#endif