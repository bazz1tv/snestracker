/// @addtogroup Game
// @{
#ifndef __GAME_H__
#define __GAME_H__

#include "MiniEngine.h"


class cPlayer;
class cLevel;
class cLevelEditor;
class cConsole;
class cSettings;
class cPreferences;

extern cPlayer *pPlayer;
extern cSettings *pGameSettings;
extern cPreferences *pPreferences;

/** @brief The Game class houses the Specific Gameplay routines for the Actual Game!! A.k.A when you are in
	@link MODE_GAME @endlink
	@detail ..? 
*/
class cGame : public MiniEngine
{
public:
	cGame() : MiniEngine() { }
	~cGame();
	void Init();
	void Update();
	int EventHandler();
	void HeldKey_Handler();
	
	void Player_Input();
	int Keydown_Events(SDL_Event *event);
	int MouseButtonDown_Events(SDL_Event*);
	int MouseButtonUp_Events(SDL_Event*);
	
	void TurnOnCollisionRects();

};


#endif

// @}
