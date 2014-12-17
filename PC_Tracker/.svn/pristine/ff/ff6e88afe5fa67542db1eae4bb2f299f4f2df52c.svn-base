/// @addtogroup Game
// @{
#ifndef __GAME_H__
#define __GAME_H__

#include "MiniEngine.h"


class cPlayer;
class cLevel;
class cLevelEditor;
class cConsole;

/** @brief The Game class houses the Specific Gameplay routines for the Actual Game!! A.k.A when you are in
	@link MODE_GAME @endlink
	@detail ..? 
*/
class Game : public MiniEngine
{
public:
	~Game();
	static void Init();
	void Update();
	void EventHandler();
	void HeldKey_Handler();
	
	void Player_Input();
	void Keydown_Events(SDL_Event *event);
	void MouseButtonDown_Events(SDL_Event*);
	void MouseButtonUp_Events(SDL_Event*);
	
	void TurnOnCollisionRects();

};


#endif
