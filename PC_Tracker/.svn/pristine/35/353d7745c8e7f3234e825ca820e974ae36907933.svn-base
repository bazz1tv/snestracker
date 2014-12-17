#include "MultiSelect.h"
#include "leveleditor.h"
#include "player.h"
#include "level.h"
#include "Camera.h"
#include "enemy.h"

extern cPlayer *pPlayer;
extern cLevel *pLevel;
extern cCamera *pCamera;

void cMultiSelect :: DrawOutlineAroundMultiSelect_Tiles(SDL_Renderer *renderer, Uint32 Color)
{
	//HoveredObject = SetRect( 0, 0, 0, 0 );
	
	// This is code to get a RECT from our sprite Tile
	if (multiple_objects_selected)
	{
		for (unsigned int i=0; i < OM.objcount; i++)
		{
			// Draw a rect around each individual object
			
			// get its rect
			cMVelSprite *ptr = OM.objects[i];
			SDL_Rect orect = ptr->GetRect(SDL_TRUE);
			
			cLevelEditor::OutlineObject(renderer, Color, &orect);
		}
	}
}

void cMultiSelect::NewMoveObjects()
{
	PasteObjects();
	PrepareToMove();
}

void cMultiSelect::CopyObjects()
{
	
}

void cMultiSelect::DeleteObjects()
{
	// Only Map Objects and Enemies can be deleted
	
	
	int index;
	
	for (unsigned int i=0; i < OM.objcount; i++)
	{
		// Map Objects
		index = pLevel->pLevelData_Layer1->BasicSprites.hasa(OM.objects[i]);
		if (index >= 0)
		{
			pLevel->pLevelData_Layer1->DeleteSprite( index );
			continue;
		}

		index = Enemies.hasa((cEnemy*)OM.objects[i]);
		if (index >= 0)
		{
			DeletEnemy( index);
			continue;
		}
		
		
	}
	
	OM.~ObjectManager();
	multiple_objects_selected = SDL_FALSE;

}

void cMultiSelect::PasteObjects()
{
	double leftmostx,leftmosty;
	//ObjectManager<cMVelSprite> newsprites(OM_SAVE_OBJS_AT_DESTROY);
	
	if (OM.objects && OM.objcount > 0)
	{
		leftmostx = OM.objects[0]->posx;
		leftmosty = OM.objects[0]->posy;
	}
	
	if (OM.objcount > 1)
	{
		for (unsigned int i=0; i < OM.objcount; i++)
		{
			int x;
			x = OM.objects[i]->posx;
			if (x <= leftmostx)
			{
				leftmostx = x;
				leftmosty = OM.objects[i]->posy;
			}
		}
	}
	
	for (unsigned int i=0; i < OM.objcount; i++)
	{
		// Add the New Object
		if( OM.objects[i]->type == SPRITE_TYPE_MASSIVE || OM.objects[i]->type == SPRITE_TYPE_PASSIVE || OM.objects[i]->type == SPRITE_TYPE_HALFMASSIVE)
		{
			// Create the new Sprite
			cMVelSprite *new_Object = new cMVelSprite( OM.objects[i]->srcimage, floor((pMouse->posx-leftmostx) + (pCamera->x-camXOffset) + OM.objects[i]->posx),
													  floor( (pMouse->posy - leftmosty) + (pCamera->y-camYOffset) + OM.objects[i]->posy) );
			
			new_Object->type = OM.objects[i]->type;
			
			pLevel->pLevelData_Layer1->AddSprite( new_Object );
			OM.objects[i] = new_Object;
		}
		else if( OM.objects[i]->type == SPRITE_TYPE_ENEMY )
		{
			cEnemy *pEnemy = (cEnemy *)OM.objects[i];
			
			AddEnemy(floor((pMouse->posy - mouseYOffset) + (pCamera->y-camYOffset) + OM.objects[i]->posy), floor((pMouse->posy - mouseYOffset) + (pCamera->y-camYOffset) + OM.objects[i]->posy), pEnemy->Enemy_type );
			
			OM.objects[i] = pEnemy;
		}
	}
	
	//OM.~ObjectManager();
	//OM = newsprites;
}

void cMultiSelect :: SetObjects( void )
{
	// Only Map and Enemy Objects can be Copied (FOR NOW)
	// SetObject( GetCollidingObject( pMouse->posx, pMouse->posy )  );
	GetAllCollidingObjects(&rect, &OM);
	
	multiple_objects_selected = (OM.objcount > 0) ? SDL_TRUE:SDL_FALSE;
}
void cMultiSelect :: SetObject( cMVelSprite *nObject)
{
	if( !nObject )
	{
		return;
	}
	
	int index = OM.hasa(nObject);
	if ( index < 0 )
	{
		OM.add(nObject);
		multiple_objects_selected = SDL_TRUE;
	}
	else
	{
		OM.Remove(index);
	}
	
	
}

void cMultiSelect::Move(void)
{
	for (unsigned int i=0; i < OM.objcount; i++)
	{
		OM.objects[i]->SetPos( floor((pMouse->posx-mouseXOffset) + (pCamera->x-camXOffset) + OM.objects[i]->posx), floor((pMouse->posy - mouseYOffset) + (pCamera->y-camYOffset) + OM.objects[i]->posy) );
		
		OM.objects[i]->Startposx = OM.objects[i]->posx;
		OM.objects[i]->Startposy = OM.objects[i]->posy;
	}
	
	mouseXOffset = pMouse->posx;
	mouseYOffset = pMouse->posy;
	camXOffset = pCamera->x;
	camYOffset = pCamera->y;
}
SDL_bool cMultiSelect :: GetAllCollidingObjects( SDL_Rect *cRect, ObjectManager<cMVelSprite> *obj_man )
{
	// These variables are going to hold the data we need :)
	SDL_bool were_objects_found = SDL_FALSE;
	
	//obj_man->RemoveAllObjects();
	
	// Player
	if( RectIntersect( &(const SDL_Rect&)pPlayer->GetRect( SDL_TRUE ), cRect ) )
	{
		if (obj_man->hasa((cMVelSprite *)pPlayer) < 0)
		{
			obj_man->add((cMVelSprite*)pPlayer);
			were_objects_found = SDL_TRUE;
		}
	}
	
	// Map Objects
	if (pLevel->pLevelData_Layer1->GetAllCollidingSpriteNum( cRect, obj_man ))
		were_objects_found = SDL_TRUE;
	
	
	// Do ENEMIES LATER
	
	// Enemies
	/*CollisionNum = GetCollidingEnemyNum( cRect );
	 
	 if( CollisionNum >= 0 )
	 {
	 return (cMVelSprite *)Enemies[CollisionNum];
	 }*/
	
	//return NULL;
	
	return were_objects_found;
}
void cMultiSelect::InitTiles(SDL_bool release/* = SDL_FALSE */)
{
	cLevelEditor::Mouse_command = MOUSE_COMMAND_SELECT_MULTISELECT_TILES;
	
	mouseXOffset = pMouse->posx;
	mouseYOffset = pMouse->posy;
	
	rectX_origin = pMouse->posx;
	rectY_origin = pMouse->posy;
	
	rect.x = (int)pMouse->posx;
	rect.y = (int)pMouse->posy;
	
	rect.w = 0;
	rect.h = 0;
	
	// Release last time's old tiles
	if (release)
		Release();
}

void cMultiSelect :: Prepare()
{
	//mouseXOffset = veryfirst_mouseXOffset =  pMouse->posx;
	//mouseYOffset = veryfirst_mouseYOffset =  pMouse->posy;
	//camXOffset = pCamera->x;
	//camYOffset = pCamera->y;
}

void cMultiSelect::PrepareToMove()
{
	//Object = GetCollidingObject( pMouse->posx, pMouse->posy );
	cLevelEditor::Mouse_command = MOUSE_COMMAND_MOVING_MULTISELECT_TILES;
	
	mouseXOffset = veryfirst_mouseXOffset =  pMouse->posx;
	mouseYOffset = veryfirst_mouseYOffset =  pMouse->posy;
	camXOffset = pCamera->x;
	camYOffset = pCamera->y;
}

SDL_bool cMultiSelect::Release()
{
	OM.RemoveAllObjects();
	
	if (multiple_objects_selected)
	{
		//OM.RemoveAllObjects();
		multiple_objects_selected = SDL_FALSE;
		return SDL_TRUE;
	}
	return SDL_FALSE;
}