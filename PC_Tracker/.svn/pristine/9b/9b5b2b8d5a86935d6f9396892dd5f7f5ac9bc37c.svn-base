

//#include "leveleditor.h"
#include "MultiSelect.h"
#include "leveleditor.h"
#include "enemy.h"
#include "Misc.h"
#include "level.h"
#include "player.h"
#include "Camera.h"
#include "TextObject.h"
#include <sstream>

extern cPlayer *pPlayer;
extern cCamera *pCamera;
extern cLevel *pLevel;

enum LevelEditor_Modes {
	MAIN=0,
	TABMODE
};
int cLevelEditor::submode = MAIN;

// This is the amount of time to SLEEP when doing HELD_KEY events (so we don't work at the speed of light)
#define LEVELEDIT_SLEEPWAIT 100000

// Definitions for easy portability
#define SAVE_KEY SDLK_s
#define COPY_KEY SDLK_c
#define PASTE_KEY SDLK_v
#define FASTCOPY_KEY SDLK_f


#define COLOR_WHITE 0xffffffff
#define COLOR_BLUE 0xff7878ff

Uint8 cLevelEditor::command =COMMAND_NOTHING;

cLevelEditor :: cLevelEditor( void ) : MiniEngine()
{
	command =COMMAND_NOTHING;
	
	MultiSelect = new cMultiSelect;

	Mouse_w = 0;
	Mouse_h = 0;

	Object = NULL;
	CopyObject = NULL;
	lastCopiedObject = NULL;
	
	coordinates = SDL_FALSE;
}

cLevelEditor :: ~cLevelEditor( void )
{
	delete MultiSelect;
}

int cLevelEditor :: Do()
{
	// Reset mode to 0
	mode = 0;
	
	while (mode == 0)
	{
		if (submode == MAIN)
		{
			Update();
			mode = EventHandler();
			this->Draw(Renderer);
			//PostDraw();
			//DrawAllToScreen();
		}
		/*else if (submode == TABMODE)
		{
			//submode = optionsmenu.Do();
		}*/
		else
		{
			// can't remember why I have this .-.
			mode = submode;
		}
		
		// if the mode got switched (we pressed start game or quit)
	}
	
	return mode;
	
	
}

SDL_Rect cLevelEditor::GetHoveredObjectRect()
{
	cMVelSprite *Collision_Object = GetCollidingObject( pMouse->posx, pMouse->posy );
	
	if( Collision_Object )
	{
		return  HoveredObjectRect=Collision_Object->GetRect( SDL_TRUE );
	}
	
	return SetRect(0,0,0,0);
}

void cLevelEditor :: Update( void )
{
	//PreUpdate();
	pFramerate->SetSpeedFactor(); // Update
	pCamera->Update();
	pMouse->Update(Renderer);

	if( command == COMMAND_NOTHING || command == COMMAND_FASTCOPY ) 
	{
		HoveredObjectRect = GetHoveredObjectRect();
	}
	else if( command == COMMAND_MOVING_SINGLE_TILE )
	{
		MoveSingleTile();
	}
	else if (command == COMMAND_SELECT_MULTISELECT_TILES)
	{
		MultiSelect->DoRect();
	}
	else if (command == COMMAND_MOVING_MULTISELECT_TILES )
	{
		MultiSelect->Move();
	}

	PostUpdate();
}

void cLevelEditor::MoveSingleTile()
{
	Object->SetPos( floor(pMouse->posx + Mouse_w + pCamera->x), floor(pMouse->posy + Mouse_h + pCamera->y) );
	
	Object->Startposx = floor(pMouse->posx + Mouse_w + pCamera->x);
	Object->Startposy = floor(pMouse->posy + Mouse_h + pCamera->y);
}

void cLevelEditor :: Draw()
{
	Draw(Renderer);
}
void cLevelEditor :: Draw (SDL_Renderer *renderer)
{
	

	//PreDraw();
	SDL_RenderClear(Renderer);
	pLevel->Draw();
	
	pPlayer->Draw( renderer );
	
	DrawEnemies(renderer);
	pMouse->Draw( renderer );


	
	
	// Draw the outline of the hovered Object
	if (command == COMMAND_NOTHING)
		OutlineHoveredObject(renderer);
	else if (command == COMMAND_FASTCOPY)
	{
		OutlineHoveredObject(renderer, COLOR_BLUE);
	}
	
	if (command == COMMAND_SELECT_MULTISELECT_TILES)
	{
		MultiSelect->DrawRect(renderer);
	}
	
	// Only the draw the MultiSelect_-tile outlines if we are not moving them (for accuracy when moving)
	if (command != COMMAND_MOVING_MULTISELECT_TILES || (MultiSelect->veryfirst_mouseXOffset == pMouse->posx && MultiSelect->veryfirst_mouseYOffset == pMouse->posy))
	{
		// Draw the MultiSelect_-outline (it will check automatically if we have MultiSelect_ tiles to draw)
		MultiSelect->DrawTileOutlines(renderer);
	}
	
	// draw
	if (coordinates)
	{
		// draw coordinates
		std::stringstream var;
		var << "("<<pMouse->posx+pCamera->x<<","<<pMouse->posy+pCamera->y<<")";
		TextObject xy_coordinate(pMouse->posx+pMouse->width/2, pMouse->posy+pMouse->height+5, var.str(), bold_16); //(int x, int y, string text, TTF_Font *font); // (x,y)
		
		xy_coordinate.Render();
		xy_coordinate.Draw();
	}
	
	
	///PostDraw();
	DrawFramerate();
	DrawAllToScreen();
}



void cLevelEditor :: OutlineObject(SDL_Renderer *renderer, Uint32 Color, SDL_Rect *object_rect)
{
	// The object_rect Shadow
	// top line
	FillRect(renderer, object_rect->x+1, object_rect->y+1, object_rect->w, 1, 0,0,0);
	// Left Line
	FillRect(renderer, object_rect->x+1, object_rect->y+1, 1, object_rect->h, 0,0,0);
	// Bottom Line
	FillRect(renderer, object_rect->x+1, object_rect->y+1+object_rect->h, object_rect->w, 1, 0,0,0);
	// Right Line
	FillRect(renderer, object_rect->x+1+object_rect->w, object_rect->y+1, 1, object_rect->h, 0,0,0);
	
	// The object_rect Outline (white or blue depending on Fastcopy or just a selection)
	// top line
	FillRectAlpha(renderer, object_rect->x, object_rect->y, object_rect->w, 1, Color);
	// Left Line
	FillRectAlpha(renderer, object_rect->x, object_rect->y, 1, object_rect->h, Color);
	// Bottom Line
	FillRectAlpha(renderer, object_rect->x, object_rect->y+object_rect->h, object_rect->w, 1, Color);
	// Right Line
	FillRectAlpha(renderer, object_rect->x+object_rect->w, object_rect->y, 1, object_rect->h, Color);
}

void cLevelEditor::OutlineHoveredObject(SDL_Renderer *renderer, Uint32 Color)
{
	OutlineObject(renderer, Color, &HoveredObjectRect);
}

void cLevelEditor:: OutlineHoveredObject( SDL_Renderer *renderer)
{
	Uint32 Color = COLOR_WHITE;
	if( command != COMMAND_FASTCOPY )
	{
		Color = COLOR_WHITE; // White
	}
	
	OutlineObject(renderer, Color, &HoveredObjectRect);
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

	command =COMMAND_MOVING_SINGLE_TILE;
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

void cLevelEditor::NewMoveObject(cMVelSprite *nObject)
{
	if( !nObject )
	{
		return;
	}
	
	CopyObject = nObject;

	PasteObject(pMouse->posx, pMouse->posy);
	Mouse_w = (int)(lastCopiedObject->posx -= pMouse->posx + pCamera->x);
	Mouse_h = (int)(lastCopiedObject->posy -= pMouse->posy + pCamera->y);
	
	Object = lastCopiedObject;
	
	command =COMMAND_MOVING_SINGLE_TILE;
}

void cLevelEditor :: PasteObject( double x, double y )
{
	if( !CopyObject ) 
	{
		return;
	}

	// Add the New Object
	if( CopyObject->type == SPRITE_TYPE_SOLID || CopyObject->type == SPRITE_TYPE_PASSIVE || CopyObject->type == SPRITE_TYPE_TOPSOLID) 
	{
		// Create the new Sprite
		cMVelSprite *new_Object = new cMVelSprite( CopyObject->srcimage, x + pCamera->x, y + pCamera->y );

		new_Object->type = CopyObject->type;

		pLevel->pLevelData_Layer1->AddSprite( new_Object );

		if( command == COMMAND_FASTCOPY ) 
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
	DEBUGLOG("Delete Object Called\n");
	DeleteObject( pMouse->posx, pMouse->posy );
}

void cLevelEditor :: DeleteObject( double x, double y )
{
	// Only Map Objects and Enemies can be deleted
	
	SDL_Rect cRect = SetRect( (int)x, (int)y, 1, 1 );

	int CollisionNum = -1;

	// Map Objects	
	CollisionNum = pLevel->pLevelData_Layer1->GetCollidingSpriteNum( &cRect );

	if( CollisionNum >= 0 )
	{
		DEBUGLOG("Collision Detected. Deleting Sprite\n");
		pLevel->pLevelData_Layer1->DeleteSprite( CollisionNum );
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

	command =COMMAND_FASTCOPY;
}

void cLevelEditor :: SetFastCopyObject( void )
{
	SetCopyObject();
	
	command =COMMAND_FASTCOPY;
}

void cLevelEditor :: Release_Command( void )
{
	if( Object ) 
	{
		Object = NULL;
	}

	if( command == COMMAND_FASTCOPY ) 
	{
		CopyObject = NULL;
	}

	command =COMMAND_NOTHING;
}

cMVelSprite *cLevelEditor :: GetCollidingObject( double x, double y )
{
	SDL_Rect cRect = SetRect( (int)x, (int)y, 1, 1 );

	int CollisionNum = -1;

	// Player
	if( RectIntersect( &(const SDL_Rect&)pPlayer->GetRect( SDL_TRUE ), &cRect ) )
	{
		return (cMVelSprite*)pPlayer;
	}

	// Map Objects	
	CollisionNum = pLevel->pLevelData_Layer1->GetCollidingSpriteNum( &cRect );

	if( CollisionNum >= 0 )
	{
		return pLevel->pLevelData_Layer1->BasicSprites.objects[CollisionNum];
	}

	// Enemies
	CollisionNum = GetCollidingEnemyNum( &cRect );

	if( CollisionNum >= 0 )
	{
		return (cMVelSprite *)Enemies.objects[CollisionNum];
	}
	
	return NULL;
}



/** @ingroup LE_Input */
int cLevelEditor::EventHandler()
{
	//int mode=0;
	while ( SDL_PollEvent( &event ) )
	{
		mode = UniversalEventHandler(&event);
		if (mode > 0)
			return mode;
		
		MouseButton_Held_Events();
		
		

		switch ( event.type )
		{
			case SDL_QUIT:
			{
				//done = 2;
				break;
			}
			case SDL_DROPFILE:
			{
				printf("%s\n", event.drop.file);
				SDL_free(event.drop.file);
				break;
				
			}
			case SDL_KEYDOWN:
			{
				mode = KeyDownEvents(event);
				if (mode > 0) return mode;
				break;
			}
				
			case SDL_MOUSEBUTTONDOWN:
			{
				MouseButton_Down_Events(event);
				break;					
			}
				
			case SDL_MOUSEBUTTONUP:
			{
				MouseButton_Up_Events(event);
				break;	
			}
				
			default: break;
		}
	}
	
	return (int)mode;
}

int cLevelEditor::KeyDownEvents(SDL_Event &event)
{
	//int mode=0;
	// ESCAPE to ESCAPE mouse command or LEVEL MODE
	if( event.key.keysym.sym == SDLK_ESCAPE )
	{
		
		if (command == COMMAND_NOTHING && !MultiSelect->multiple_objects_selected)
			mode = MODE_GAME;
		else if (MultiSelect->multiple_objects_selected)
		{
			MultiSelect->OM.RemoveAllObjects();
			MultiSelect->multiple_objects_selected = SDL_FALSE;
		}
		
		
		command = COMMAND_NOTHING;
		
	}
	/*else if ( event.key.keysym.sym == SDLK_TAB )
	{
		submode = TABMODE;
	}*/
	else if ( event.key.keysym.sym == SDLK_c && !(event.key.keysym.mod & KMOD_LCTRL) )
	{
		// c for coordinates
		if (coordinates)
			coordinates= SDL_FALSE;
		else coordinates = SDL_TRUE;
	}
	// ~ to enter CONSOLE
	else if ( event.key.keysym.sym == SDLK_BACKQUOTE )
	{
		return mode=MODE_CONSOLE;
		
	}
	// F8 to Exit LEVEL MODE
	else if( event.key.keysym.sym == SDLK_F8 )
	{
		mode = MODE_GAME;
		pCamera->SetPos( pPlayer->posx - pCamera->x - window.w, 0 );
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
			if (!MultiSelect->multiple_objects_selected)
				SetCopyObject();
			else
			{
				MultiSelect->CopyObjects();
			}
			
		}
		//LCTRL+V to paste
		else if( event.key.keysym.sym == PASTE_KEY  )
		{
			if (!MultiSelect->multiple_objects_selected)
				PasteObject();
			else
			{
				MultiSelect->Prepare();
				MultiSelect->PasteObjects();
			}
		}
	}
	
	
	// F Key for Fast Copy
	else if (event.key.keysym.sym == FASTCOPY_KEY)
	{
		if (command == COMMAND_FASTCOPY)
			command =COMMAND_NOTHING;
		else
			SetFastCopyObject();
	}
	
	else if( command == COMMAND_FASTCOPY && CopyObject)
	{
		if( event.key.keysym.sym == SDLK_d)
		{
			PasteObject( CopyObject->posx - pCamera->x + CopyObject->width, CopyObject->posy - pCamera->y );
			pCamera->Move( CopyObject->width, 0 );
		}
		else if( event.key.keysym.sym == SDLK_w )
		{
			PasteObject( CopyObject->posx - pCamera->x, CopyObject->posy - pCamera->y - CopyObject->height );
			pCamera->Move( 0, - CopyObject->height );
		}
		else if( event.key.keysym.sym == SDLK_a )
		{
			PasteObject( CopyObject->posx - pCamera->x - CopyObject->width, CopyObject->posy - pCamera->y );
			pCamera->Move( - CopyObject->width, 0 );
		}
		else if( event.key.keysym.sym == SDLK_s )
		{
			PasteObject( CopyObject->posx- pCamera->x , CopyObject->posy - pCamera->y + CopyObject->height );
			pCamera->Move( 0, CopyObject->height );
		}
	}
	else
	{
		if (event.key.keysym.mod & KMOD_SHIFT)
		{
			
		}
		else
		{
			if( event.key.keysym.sym == SDLK_d)
			{
				pCamera->Move( 1, 0 );
			}
			else if( event.key.keysym.sym == SDLK_a )
			{
				pCamera->Move( -1, 0 );
			}
			
			
			if( event.key.keysym.sym == SDLK_w )
			{
				pCamera->Move( 0, -1 );
			}
			
			else if( event.key.keysym.sym == SDLK_s )
			{
				pCamera->Move( 0, 1 );
			}
		}
		
	}
	
	return mode;
}

void cLevelEditor::MouseButton_Down_Events(SDL_Event &event)
{
	if( event.button.button == 1 ) // Left Mouse Button
	{
		if (keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL])
		{
			if (MultiSelect->multiple_objects_selected)
			{
				if (GetCollidingObject(pMouse->posx, pMouse->posy))
				{
					MultiSelect->AddObject(GetCollidingObject( pMouse->posx, pMouse->posy ));
					MultiSelect->PrepareToMove();
				}
				else
				{
					MultiSelect->InitTiles(SDL_FALSE);
				}
			}
			else
			{
				MultiSelect->InitTiles(SDL_TRUE);
			}
		}
		else if (keys[SDL_SCANCODE_LALT])
		{
			if (MultiSelect->multiple_objects_selected)
			{
				MultiSelect->NewMoveObjects();
			}
			else
			{
				cMVelSprite *ptr;
				if ((ptr = GetCollidingObject(pMouse->posx, pMouse->posy)))
				{
					NewMoveObject(ptr);
				}
			}
		}
		else
		{
			if (MultiSelect->multiple_objects_selected == SDL_TRUE && (MultiSelect->OM.hasa(GetCollidingObject( pMouse->posx, pMouse->posy )) >= 0) )
			{
				// There is an explanation for the explicit checks for SDL_TRUE/SDL_FALSE
				// I was not sure if these values could use the if (varname) since i was unsure of the values
				// it seems safe because SDL_TRUE / SDL_FALSE is 1,0 but i do it explicit to be safer.
				//if ()
				//Move();
				
				/* the current process is that */
				MultiSelect->PrepareToMove();
			}
			else if (!GetCollidingObject(pMouse->posx, pMouse->posy))
			{
				MultiSelect->InitTiles(SDL_TRUE);
			}
			else
			{
				// Release any MultiSelect_-shit
				MultiSelect->Release();
				SetMoveObject();
				
			}
		}
	}
	else if( event.button.button == 2 ) // Middle Mouse Button
	{
		//level editor mode
		SetFastCopyObject();
	}
	else if( event.button.button == 3 ) // Right Mouse Button
	{
		
	}
}

void cLevelEditor::MouseButton_Up_Events(SDL_Event &event)
{
	if( event.button.button == 1 ) // Left Mouse Button
	{
		if (command != COMMAND_MOVING_MULTISELECT_TILES && command != COMMAND_SELECT_MULTISELECT_TILES)
			MultiSelect->Release();
		
		Release_Command();
	}
	else if( event.button.button == 2 ) // Middle Mouse Button
	{
		Release_Command();
	}
	else if( event.button.button == 3 ) // Right Mouse Button
	{
		
	}
}


void  cLevelEditor::HeldKey_Handler()
{
	// Fast copy
	HeldKey_fastcopy();
	
	/// Move camera functions
	HeldKey_movecamera();
	
	
	// if at anytime 'e' is pressed. Delete the object the mouse is hovering over.
	if ( keys[SDL_SCANCODE_E] )
	{
		if (!MultiSelect->multiple_objects_selected)
			DeleteObject();
		else
			MultiSelect->DeleteObjects();
	}
}


void cLevelEditor::HeldKey_fastcopy()
{
	// Lets break this down to HeldKey_fastcopy();
	if( command == COMMAND_FASTCOPY && CopyObject)
	{
		if( keys[SDL_SCANCODE_RCTRL] || keys[SDL_SCANCODE_LCTRL] )
		{
			if( keys[SDL_SCANCODE_D])
			{
				//usleep(LEVELEDIT_SLEEPWAIT);
				PasteObject( CopyObject->posx - pCamera->x + CopyObject->width, CopyObject->posy - pCamera->y );
				pCamera->Move( CopyObject->width, 0 );
			}
			else if( keys[SDL_SCANCODE_W] )
			{
				//usleep(LEVELEDIT_SLEEPWAIT);
				PasteObject( CopyObject->posx - pCamera->x, CopyObject->posy - pCamera->y - CopyObject->height );
				pCamera->Move( 0, - CopyObject->height );
			}
			else if( keys[SDL_SCANCODE_A] )
			{
				//usleep(LEVELEDIT_SLEEPWAIT);
				PasteObject( CopyObject->posx - pCamera->x - CopyObject->width, CopyObject->posy - pCamera->y );
				pCamera->Move( - CopyObject->width, 0 );
			}
			else if( keys[SDL_SCANCODE_S] )
			{
				//usleep(LEVELEDIT_SLEEPWAIT);
				PasteObject( CopyObject->posx- pCamera->x , CopyObject->posy - pCamera->y + CopyObject->height );
				pCamera->Move( 0, CopyObject->height );
			}
		}
	}
}

void cLevelEditor::HeldKey_movecamera()
{
	
		if (command == COMMAND_FASTCOPY || keys[SDL_SCANCODE_LCTRL] || (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT) && !keys[SDL_SCANCODE_LSHIFT]) )
		{
			if( keys[SDL_SCANCODE_RIGHT] )
			{
				pCamera->Move( 10 * pFramerate->speedfactor, 0 );
			}
			else if( keys[SDL_SCANCODE_LEFT])
			{
				pCamera->Move( -10 * pFramerate->speedfactor, 0 );
			}
			
			if( keys[SDL_SCANCODE_UP])
			{
				pCamera->Move( 0, -10 * pFramerate->speedfactor );
			}
			else if( keys[SDL_SCANCODE_DOWN])
			{
				pCamera->Move( 0, 10 * pFramerate->speedfactor );
			}
		}
		else
		{
			if( keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D] )
			{
				pCamera->Move( 10 * pFramerate->speedfactor, 0 );
			}
			else if( keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A] )
			{
				pCamera->Move( -10 * pFramerate->speedfactor, 0 );
			}
			
			if( keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W] )
			{
				pCamera->Move( 0, -10 * pFramerate->speedfactor );
			}
			else if( keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] )
			{
				pCamera->Move( 0, 10 * pFramerate->speedfactor );
			}
		}
		// I floor it because otherwise weird results in using decimal values
		pCamera->x = floor(pCamera->x);
		pCamera->y = floor(pCamera->y);
	
}

void cLevelEditor::MouseButton_Held_Events()
{
	// First Let's poll for Mouse button HELD Down
	// You can hold right click to delete tiles constantly
	mouse_state = SDL_GetMouseState(NULL,NULL);
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		DeleteObject();
	}
	
	if (command == COMMAND_SELECT_MULTISELECT_TILES)
	{
		if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			if (keys[SDL_SCANCODE_LCTRL])
				MultiSelect->AddObjects(SDL_FALSE);
			else
				MultiSelect->AddObjects(SDL_TRUE);
		}
	}
}
	

