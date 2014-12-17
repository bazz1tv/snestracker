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
#define JUMP_KEYS_HELD		(keys[SDL_GetScancodeFromKey(pGameSettings->Key_jump)]) 
#define MOVELEFT_KEYS_HELD	(keys[SDL_GetScancodeFromKey(pGameSettings->Key_walk_left)]) 
#define MOVERIGHT_KEYS_HELD (keys[SDL_GetScancodeFromKey(pGameSettings->Key_walk_right)]) 
#define JUMP_KEYS_TRIGGERED (event->key.keysym.sym == (pGameSettings->Key_jump)) 
#define FIRE_KEY_TRIGGERED	(event->key.keysym.sym == (pGameSettings->Key_shoot_primary))
#define FIRE_KEY_HELD	(keys[SDL_GetScancodeFromKey(pGameSettings->Key_shoot_primary)])

// The Directorys
#define PIXMAPS_DIR "data/pixmaps/"
#define MUSIC_DIR "data/music/"
#define SOUNDS_DIR "data/sound/"
#define SOUNDS_DIR_GUNS SOUNDS_DIR "guns/"
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
enum GameMode {
MODE_MAINMENU=1,
MODE_GAME,
MODE_LEVELEDITOR,
MODE_CONSOLE,
MODE_OPTIONS,
MODE_QUIT,
MODE_DEBUG,				///< Not implemented
MODE_TEST,

NUM_GAMEMODES
};
// @}

#endif