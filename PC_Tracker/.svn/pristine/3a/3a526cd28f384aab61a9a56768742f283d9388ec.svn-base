//
//  MiniEngine.h
//  MeatBall
//
//  Created by bazz on 2/13/13.
//  Copyright (c) 2013 Student. All rights reserved.
//

#pragma once

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
	MiniEngine();
	virtual void PreUpdate( void );
	virtual void PostUpdate( void );
	virtual void Update( void );
	virtual void PreDraw( void );
	virtual void PostDraw( void );
	virtual void Draw( void );
	virtual int Do();
	
	virtual int Input();
	virtual int EventHandler() { return 0; }
	virtual void HeldKey_Handler() {}
	
	Uint8 mode;
};

