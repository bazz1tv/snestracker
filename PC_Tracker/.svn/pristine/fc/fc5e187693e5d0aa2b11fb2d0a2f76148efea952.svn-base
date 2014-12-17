#include "Globals.h"
#include "MiniEngine.h"
#include "player.h"
#include "level.h"
#include "Game.h"
#include "playlist.h"
#include "bullet.h"
#include "enemy.h"
#include "effects.h"
#include "Misc.h"
#include "preferences.h"
#include "weapon.h"
#include "Camera.h"


extern cLevel *pLevel;
//extern cCamera *pCamera;



//////////////////////////////////////////////////////
// Game Engine : Derived from MiniEngine
//////////////////////////////////////////////////////


cGame::~cGame()
{
	
}
/// The init function to be called before starting your First Game.
void cGame::Init()
{
	// This function is for the audio playlist, and all it does is fill up some variables
	// with every song filename in the MUSIC_DIR directory
	Playlist::ls_music();
	
	PreloadBulletimages();
	
	
	
	// Messing with Global Variables here XD 
	//done = 0;				// These need to get tighted up. Make their purpose more known
	//mode = MODE_GAME;		// this global universal variable tracks Game modes. (see defines.h)
	
	pPlayer->init();		// add his images to the Image Manager, give him initial weapon
	pPlayer->Reset();		// set direction, position, setACtiveWeapon (pistol)

	/*string levelname = "test2.lvl";
	DEBUGLOG ("loading level: %s\n", levelname.c_str());
	pLevel->Load( levelname);	// load the level
	DEBUGLOG ("Finished loading level\n");*/
	// if we are NOT in demo mode, let's load the level's MusicFile
/*#ifndef DEMO
	string filename = MUSIC_DIR + pLevel->Musicfile;
	pAudio->PlayMusik((char*)filename.c_str(), 1);
	while (!pAudio->MusicPlaying()) {Mix_PlayMusic(pAudio->Music, 1);}
#endif
*/	
}

void cGame::Update()
{
	//PreUpdate();
	pFramerate->SetSpeedFactor(); // Update
	pCamera->Update();
	pMouse->Update(Renderer);
	
	pLevel->Update();
	UpdateEnemies();
	UpdateBullets();
	UpdateParticleEmitter();
	UpdatePlayer();
	//PostUpdate();
}

void cGame::Player_Input()
{
	if( MOVERIGHT_KEYS_HELD )
	{
		pPlayer->direction = RIGHT;
	}
	else if( MOVELEFT_KEYS_HELD )
	{
		pPlayer->direction = LEFT;
	}
	
	if( keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W] )
	{
		pPlayer->updown_pressed = UP;
	}
	else if( keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] )
	{
		pPlayer->updown_pressed = DOWN;
	}
	else
	{
		pPlayer->updown_pressed = NONE;
	}
	
	if( FIRE_KEY_HELD )
	{
		//if( mode == MODE_GAME )
		//{
			pPlayer->Fire( );
		//}
	}
	

}

void cGame::HeldKey_Handler()
{
	Player_Input();
}



void cGame::TurnOnCollisionRects()
{
	// Turns on the Collision Rects for all collide-able images on the screen
	
	//
}

int cGame::EventHandler()
{
	//Uint8 mode=0;
	
	while ( SDL_PollEvent( &event ) )
	{
		mode = UniversalEventHandler(&event);
		if (mode > 0)
			return mode;
		
		switch ( event.type )
		{
			case SDL_KEYDOWN:
			{
				return Keydown_Events(&event);
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				return MouseButtonDown_Events(&event);
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				return MouseButtonUp_Events(&event);
				break;
			}
			default:
			{
				break;
			}
		}
	}
	return 0;
}

int cGame::Keydown_Events(SDL_Event *event)
{
	if( event->key.keysym.sym == SDLK_ESCAPE )
	{
		// universal!
		return mode=MODE_MAINMENU;
	}
	else if( FIRE_KEY_TRIGGERED )
	{
		pPlayer->Fire( );
	}
	else if( JUMP_KEYS_TRIGGERED )
	{
		pPlayer->Jump();
	}
	else if( event->key.keysym.sym == SDLK_1 )
	{
		pPlayer->ChangeActiveWeapon( WEAPON_PISTOL );
	}
	else if( event->key.keysym.sym == SDLK_2 )
	{
		pPlayer->ChangeActiveWeapon( WEAPON_MACHINEGUN );
	}
	else if( event->key.keysym.sym == SDLK_3 )
	{
		pPlayer->ChangeActiveWeapon( WEAPON_LASER_X1 );
	}
	else if ( event->key.keysym.sym == SDLK_BACKQUOTE )
	{
		return mode=MODE_CONSOLE;
	}
	else if( event->key.keysym.sym == SDLK_F8 )
	{
#ifdef INGAME_LEVEL_EDITOR
		return mode= MODE_LEVELEDITOR;
#endif
	}
	
	else if( event->key.keysym.sym == SDLK_F1 ) // Particle Test
	{
		AddParticleEmitter( pPlayer->posx + ( pPlayer->width/2 ), pPlayer->posy + ( pPlayer->height/2 ) - 10, 9, 255, 250, 200, 0.01, 500, 5 );
	}
	else if( event->key.keysym.sym == SDLK_F2 ) // Enemy Test
	{
		AddEnemy( 200, 0 + (double)( window.h/2 ), ENEMY_AF373 );
	}
	
	// DEBUG MODE FEATURES
	//============
#ifdef _DEBUG
	if (event->key.keysym.sym == SDLK_c)
	{
		TurnOnCollisionRects();
	}
#endif
	//------------
	
	return 0;
}

int cGame::MouseButtonDown_Events(SDL_Event *event)
{
	if( event->button.button == 1 ) // Left Mouse Button
	{
		pPlayer->Fire();
	}
	else if( event->button.button == 2 ) // Middle Mouse Button
	{
		
	}
	else if( event->button.button == 3 ) // Right Mouse Button
	{
		pPlayer->Jump();
	}
	return 0;
}

int cGame::MouseButtonUp_Events(SDL_Event *event)
{
	if( event->button.button == 1 ) // Left Mouse Button
	{
		
	}
	else if( event->button.button == 2 ) // Middle Mouse Button
	{
		
	}
	else if( event->button.button == 3 ) // Right Mouse Button
	{
		
	}
	return 0;
}







