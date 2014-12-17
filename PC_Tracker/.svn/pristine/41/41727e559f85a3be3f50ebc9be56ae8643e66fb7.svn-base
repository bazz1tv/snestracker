

#include "Globals.h"

// This is the amount of time to SLEEP when doing HELD_KEY events (so we don't work at the speed of light)
#define LEVELEDIT_SLEEPWAIT 100000

// Definitions for easy portability
#define SAVE_KEY SDLK_s
#define COPY_KEY SDLK_c
#define PASTE_KEY SDLK_v
#define FASTCOPY_KEY SDLK_f



cLevelEditor :: cLevelEditor( void )
{
	Mouse_command = MOUSE_COMMAND_NOTHING;

	Mouse_w = 0;
	Mouse_h = 0;

	Object = NULL;
	CopyObject = NULL;
	lastCopiedObject = NULL;
}

cLevelEditor :: ~cLevelEditor( void )
{

}

void cLevelEditor :: Update( void )
{
	PreUpdate();
	HoveredObject = SetRect( 0, 0, 0, 0 );

	if( Mouse_command == MOUSE_COMMAND_NOTHING || Mouse_command == MOUSE_COMMAND_FASTCOPY ) 
	{
		cMVelSprite *Collision_Object = GetCollidingObject( pMouse->posx, pMouse->posy );

		if( Collision_Object ) 
		{
			HoveredObject = Collision_Object->GetRect( 1 );
		}
	}
	else if( Mouse_command == MOUSE_COMMAND_MOVING )
	{
		Object->SetPos( floor(pMouse->posx + Mouse_w + pCamera->x), floor(pMouse->posy + Mouse_h + pCamera->y) );
		
		Object->Startposx = floor(pMouse->posx + Mouse_w + pCamera->x);
		Object->Startposy = floor(pMouse->posy + Mouse_h + pCamera->y);
	}

	PostUpdate();

}


void cLevelEditor :: Draw()
{
	Draw(Screen);
}
void cLevelEditor :: Draw( SDL_Surface *target )
{
	Uint32 Color;

	PreDraw();

	pPlayer->Draw( Screen );
	
	DrawEnemies();
	pMouse->Draw( Screen );


	if( Mouse_command != MOUSE_COMMAND_FASTCOPY ) 
	{
		Color = SDL_MapRGB( target->format, 255, 255, 255 ); // White
	}
	else
	{
		Color = SDL_MapRGB( target->format, 120, 120, 255 ); // Blue
	}
	

	// The HoveredObject Shadow
	// top line
	FillRect(Screen, HoveredObject.x+1, HoveredObject.y+1, HoveredObject.w, 1, 0);
	// Left Line
	FillRect(Screen, HoveredObject.x+1, HoveredObject.y+1, 1, HoveredObject.h, 0);
	// Bottom Line
	FillRect(Screen, HoveredObject.x+1, HoveredObject.y+1+HoveredObject.h, HoveredObject.w, 1, 0);
	// Right Line
	FillRect(Screen, HoveredObject.x+1+HoveredObject.w, HoveredObject.y+1, 1, HoveredObject.h, 0);

	// The HoveredObject Outline (white or blue depending on Fastcopy or just a selection)
	// top line
	FillRect(Screen, HoveredObject.x, HoveredObject.y, HoveredObject.w, 1, Color);
	// Left Line
	FillRect(Screen, HoveredObject.x, HoveredObject.y, 1, HoveredObject.h, Color);
	// Bottom Line
	FillRect(Screen, HoveredObject.x, HoveredObject.y+HoveredObject.h, HoveredObject.w, 1, Color);
	// Right Line
	FillRect(Screen, HoveredObject.x+HoveredObject.w, HoveredObject.y, 1, HoveredObject.h, Color);

	if( CopyObject && !FullRectIntersect( &HoveredObject, &CopyObject->rect ) && Mouse_command != MOUSE_COMMAND_FASTCOPY ) 
	{
		// The CopyObject Shadow
		//sge_RectAlpha( Screen ,(int)( CopyObject->posx - pCamera->x + 1 ), (int)( CopyObject->posy - pCamera->y + 1 ), (int)( CopyObject->posx - pCamera->x + 1 + CopyObject->width ), 
		//				(int)( CopyObject->posy - pCamera->y + 1 + CopyObject->height ), 0, 0, 0, 64 );

		// Draws an non Filled Rect with the CopyObject rect size
		//sge_RectAlpha( Screen ,(int)( CopyObject->posx - pCamera->x ), (int)( CopyObject->posy - pCamera->y ), (int)( CopyObject->posx - pCamera->x + CopyObject->width ), 
		//				(int)( CopyObject->posy - pCamera->y + CopyObject->height ), 120, 255, 120, 192  );	
	}

	PostDraw();
}



void cLevelEditor :: SetCopyObject( cMVelSprite *nObject )
{	
	if( !nObject ) 
	{
		return;
	}

	// Player can not be copied
	if( nObject->type == SPRITE_TYPE_PLAYER ) 
	{
		return;
	}

	CopyObject = nObject;
}

void cLevelEditor :: SetCopyObject( void )
{
	// Only Map and Enemy Objects can be Copied
	SetCopyObject( GetCollidingObject( pMouse->posx, pMouse->posy ) );
}

void cLevelEditor :: SetMoveObject( cMVelSprite *nObject )
{
	if( !nObject ) 
	{
		return;
	}

	Mouse_w = (int)(nObject->posx -= pMouse->posx + pCamera->x);
	Mouse_h = (int)(nObject->posy -= pMouse->posy + pCamera->y);
	
	Object = nObject;

	Mouse_command = MOUSE_COMMAND_MOVING;
}

void cLevelEditor :: SetMoveObject( void )
{
	// Everything can be moved
	SetMoveObject( GetCollidingObject( pMouse->posx, pMouse->posy )  );
}



void cLevelEditor :: PasteObject( void )
{
	PasteObject( pMouse->posx, pMouse->posy );
}

void cLevelEditor :: PasteObject( double x, double y )
{
	if( !CopyObject ) 
	{
		return;
	}


	// Add the New Object
	if( CopyObject->type == SPRITE_TYPE_MASSIVE || CopyObject->type == SPRITE_TYPE_PASSIVE || CopyObject->type == SPRITE_TYPE_HALFMASSIVE) 
	{
		// Create the new Sprite
		cMVelSprite *new_Object = new cMVelSprite( CopyObject->srcimage, x + pCamera->x, y + pCamera->y );

		new_Object->type = CopyObject->type;

		pLevel->pLevelData->AddSprite( new_Object );

		if( Mouse_command == MOUSE_COMMAND_FASTCOPY ) 
		{
			SetFastCopyObject( new_Object );
		}
		lastCopiedObject = new_Object;
	}
	else if( CopyObject->type == SPRITE_TYPE_ENEMY )
	{
		cEnemy *pEnemy = (cEnemy *)CopyObject;

		AddEnemy(x + pCamera->x, y + pCamera->y, pEnemy->Enemy_type );
	}
}

void cLevelEditor :: DeleteObject( void )
{
	printf("Delete Object Called\n");
	DeleteObject( pMouse->posx, pMouse->posy );
}

void cLevelEditor :: DeleteObject( double x, double y )
{
	// Only Map Objects and Enemies can be deleted
	
	SDL_Rect cRect = SetRect( (int)x, (int)y, 1, 1 );

	int CollisionNum = -1;

	// Map Objects	
	CollisionNum = pLevel->pLevelData->GetCollidingSpriteNum( &cRect );

	if( CollisionNum >= 0 )
	{
		printf("Collision Detected. Deleting Sprite\n");
		pLevel->pLevelData->DeleteSprite( CollisionNum );
		return;
	}
	
	CollisionNum = GetCollidingEnemyNum( &cRect );

	if( CollisionNum >= 0 )
	{
		DeletEnemy( CollisionNum );
		return;
	}

}

void cLevelEditor :: SetFastCopyObject( cMVelSprite *nObject )
{
	SetCopyObject( nObject );

	Mouse_command = MOUSE_COMMAND_FASTCOPY;
}

void cLevelEditor :: SetFastCopyObject( void )
{
	SetCopyObject();
	
	Mouse_command = MOUSE_COMMAND_FASTCOPY;
}

void cLevelEditor :: Release_Command( void )
{
	if( Object ) 
	{
		Object = NULL;
	}

	if( Mouse_command == MOUSE_COMMAND_FASTCOPY ) 
	{
		CopyObject = NULL;
	}

	Mouse_command = MOUSE_COMMAND_NOTHING;
}

cMVelSprite *cLevelEditor :: GetCollidingObject( double x, double y )
{
	SDL_Rect cRect = SetRect( (int)x, (int)y, 1, 1 );

	int CollisionNum = -1;

	// Player
	if( RectIntersect( &(const SDL_Rect&)pPlayer->GetRect( 1 ), &cRect ) ) 
	{
		return (cMVelSprite*)pPlayer;
	}

	// Map Objects	
	CollisionNum = pLevel->pLevelData->GetCollidingSpriteNum( &cRect );

	if( CollisionNum >= 0 )
	{
		return pLevel->pLevelData->BasicSprites[CollisionNum];
	}

	// Enemies
	CollisionNum = GetCollidingEnemyNum( &cRect );

	if( CollisionNum >= 0 )
	{
		return (cMVelSprite *)Enemies[CollisionNum];
	}
	
	return NULL;
}

/** @ingroup LE_Input */
void cLevelEditor::EventHandler()
{
	while ( SDL_PollEvent( &event ) )
	{
		// First Let's poll for Mouse button HELD Down
		// You can hold right click to delete tiles constantly
		Uint8 ms = SDL_GetMouseState(NULL,NULL);
		if (ms & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			pLevelEditor->DeleteObject();
		}
		
		UniversalEventHandler(&event);

		switch ( event.type )
		{
		case SDL_QUIT:
			{
				done = 2;
				break;
			}
		case SDL_KEYDOWN:
			{
				// ESCAPE to ESCAPE mouse command or LEVEL MODE
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					// universal!
					if (pLevelEditor->Mouse_command == MOUSE_COMMAND_NOTHING)
						mode = MODE_GAME;
					else
						pLevelEditor->Mouse_command = MOUSE_COMMAND_NOTHING;
				}
				// ~ to enter CONSOLE
				else if ( event.key.keysym.sym == SDLK_BACKQUOTE )
				{
					oldmode = mode;
					mode = MODE_CONSOLE;
						
				}
				// F8 to Exit LEVLE MODE
				else if( event.key.keysym.sym == SDLK_F8 )
				{
					
					mode = MODE_GAME;

					pCamera->SetPos( pPlayer->posx - pCamera->x - Screen->w, 0 );
					
				}
				// IF LCTRL IS HELD
				else if (event.key.keysym.mod & KMOD_LCTRL)
				{
					// LCTRL S to save
					if( event.key.keysym.sym == SAVE_KEY )
					{
						pLevel->Save();	
					}
					// LCTRL+C to copy

					else if( event.key.keysym.sym == COPY_KEY  )
					{
						pLevelEditor->SetCopyObject();
					}
					//LCTRL+V to paste
					else if( event.key.keysym.sym == PASTE_KEY  )
					{
						pLevelEditor->PasteObject();
					}
				}

				// F Key for Fast Copy
				else if (event.key.keysym.sym == FASTCOPY_KEY)
				{
					if (pLevelEditor->Mouse_command == MOUSE_COMMAND_FASTCOPY)
						pLevelEditor->Mouse_command = MOUSE_COMMAND_NOTHING;
					else
						pLevelEditor->SetFastCopyObject();
				}
				else if( ( event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_a || 
					event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_d ) ) 
				{
					// level editor mode
					if( pLevelEditor->Mouse_command == MOUSE_COMMAND_FASTCOPY && pLevelEditor->CopyObject)
					{
						if( event.key.keysym.sym == SDLK_d) 
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x + pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
							pCamera->Move( pLevelEditor->CopyObject->width, 0 );
						}
						else if( event.key.keysym.sym == SDLK_w )
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x, pLevelEditor->CopyObject->posy - pCamera->y - pLevelEditor->CopyObject->height );
							pCamera->Move( 0, - pLevelEditor->CopyObject->height );
						}
						else if( event.key.keysym.sym == SDLK_a )
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x - pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
							pCamera->Move( - pLevelEditor->CopyObject->width, 0 );
						}
						else if( event.key.keysym.sym == SDLK_s )
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx- pCamera->x , pLevelEditor->CopyObject->posy - pCamera->y + pLevelEditor->CopyObject->height );
							pCamera->Move( 0, pLevelEditor->CopyObject->height );
						}
					}
					else
					{
						// level editor mode
						if( event.key.keysym.sym == SDLK_d) 
						{
							pCamera->Move( 1, 0 );
						}
						else if( event.key.keysym.sym == SDLK_w )
						{
							pCamera->Move( 0, - 1 );
						}
						else if( event.key.keysym.sym == SDLK_a )
						{
							pCamera->Move( - 1, 0 );
						}
						else if( event.key.keysym.sym == SDLK_s )
						{
							pCamera->Move( 0, 1 );
						}						
					}
				}
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					//level editor mode
						pLevelEditor->SetMoveObject();
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					//level editor mode
					pLevelEditor->SetFastCopyObject();
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{
					//level editor mode
					//pLevelEditor->DeleteObject();
				}
				break;					
			}
		case SDL_MOUSEBUTTONUP:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					//level editor mode
					pLevelEditor->Release_Command();
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					//level editor mode
						pLevelEditor->Release_Command();
					//level editor mode
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{

				}
				break;	
			}
		default:
			
			break;
		}
	}
}

void cLevelEditor::HeldKey_fastcopy()
{
	// Lets break this down to HeldKey_fastcopy();
	if( pLevelEditor->Mouse_command == MOUSE_COMMAND_FASTCOPY && pLevelEditor->CopyObject)
	{
		if( keys[SDLK_RCTRL] || keys[SDLK_LCTRL] )
		{
			if( keys[SDLK_d])
			{
				usleep(LEVELEDIT_SLEEPWAIT);
				pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x + pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
				pCamera->Move( pLevelEditor->CopyObject->width, 0 );
			}
			else if( keys[SDLK_w] )
			{
				usleep(LEVELEDIT_SLEEPWAIT);
				pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x, pLevelEditor->CopyObject->posy - pCamera->y - pLevelEditor->CopyObject->height );
				pCamera->Move( 0, - pLevelEditor->CopyObject->height );
			}
			else if( keys[SDLK_a] )
			{
				usleep(LEVELEDIT_SLEEPWAIT);
				pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x - pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
				pCamera->Move( - pLevelEditor->CopyObject->width, 0 );
			}
			else if( keys[SDLK_s] )
			{
				usleep(LEVELEDIT_SLEEPWAIT);
				pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx- pCamera->x , pLevelEditor->CopyObject->posy - pCamera->y + pLevelEditor->CopyObject->height );
				pCamera->Move( 0, pLevelEditor->CopyObject->height );
			}
		}
	}
}

void cLevelEditor::HeldKey_movecamera()
{
	if( keys[SDLK_RIGHT] )
	{
		pCamera->Move( 10 * pFramerate->speedfactor, 0 );
	}
	else if( keys[SDLK_LEFT] )
	{
		pCamera->Move( -10 * pFramerate->speedfactor, 0 );
	}
	else if( keys[SDLK_UP] )
	{
		pCamera->Move( 0, -10 * pFramerate->speedfactor );
	}
	else if( keys[SDLK_DOWN] )
	{
		pCamera->Move( 0, 10 * pFramerate->speedfactor );
	}
}

void  cLevelEditor::HeldKey_Handler()
{
	HeldKey_fastcopy();
	HeldKey_movecamera();
	
	
	
	if ( keys[SDLK_e] )
	{
		pLevelEditor->DeleteObject();
	}
}
