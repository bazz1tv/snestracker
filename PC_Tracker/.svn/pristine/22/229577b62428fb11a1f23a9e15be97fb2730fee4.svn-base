/// @addtogroup Game
// @{
#ifndef __GAME_H__
#define __GAME_H__

#include "Globals.h"

/** @brief A base class from which the main Steam Rollers can derive from.
	@detail MiniEngine has genericized processes for general gameplay. Like tracking events,
		(EventHandler()), Tracking Held Keys (HeldKey_Handler()), General Updates (Update()), and Drawing (Draw()).

	@note Draw and Update have attached pre and post Routines. These will allow you to
		A) Use my generalized routines for before+After your modules specific Code.
		B) Seperate your module specific code from the Main juice.
		This allows for more clarity in what code is specific and what isn't to the whole Shabang.
	@see Steam
*/
class MiniEngine
{
public:
	virtual void PreUpdate( void );
	virtual void PostUpdate( void );
	virtual void Update( void );
	virtual void PreDraw( void );
	virtual void PostDraw( void );
	virtual void Draw( void );
	virtual void Do();
	
	virtual void Input() { EventHandler(); HeldKey_Handler(); }
	virtual void EventHandler() {}
	virtual void HeldKey_Handler() {}
};

/** @brief The Game class houses the Specific Gameplay routines for the Actual Game!! A.k.A when you are in
	@link MODE_GAME @endlink
	@detail ..? 
*/
class Game : public MiniEngine
{
public:
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
