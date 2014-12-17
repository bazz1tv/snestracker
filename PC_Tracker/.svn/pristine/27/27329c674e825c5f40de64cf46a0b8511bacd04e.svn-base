#include "Globals.h"
#include "Game.h"
#include "playlist.h"




void MiniEngine::Do()
{
	// Might have to change the whole ONLY checking input while !paused thing...
	if ( !paused )
	{
		Input();
		Update();
	}
	Draw();
}

/// Does general things
void MiniEngine::PreUpdate( void )
{	
	pFramerate->SetSpeedFactor(); // Update
	
	pCamera->Update();
	
	pMouse->Update();
}

void MiniEngine::Update()
{
	PreUpdate();
	PostUpdate();
}

void MiniEngine::PostUpdate()
{
	
	
}

void MiniEngine::PreDraw()
{
	SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 0, 0, 0 ) );

	pLevel->Draw();
}
void MiniEngine::PostDraw()
{
	FramerateDraw( Screen );
	
	SDL_Flip( Screen );
}
void MiniEngine::Draw( void )
{
	
	PreDraw();

    pPlayer->Draw( Screen );
    
	DrawBullets();
	DrawParticleEmitter();

	
	
	DrawEnemies();
	

	PostDraw();

	
}


void Game::Init()
{
	//Playlist::ls_music();
	// Messing with Global Variables here XD 
	done = 0;
	mode = MODE_GAME;

	pPlayer->init();
	pPlayer->Reset();

	
	pLevel->Load( "test.lvl" );
#ifndef DEMO
	string filename = MUSIC_DIR + pLevel->pLevelData->Musicfile;
	pAudio->PlayMusik((char*)filename.c_str(), 1);
	while (!pAudio->MusicPlaying()) {Mix_PlayMusic(pAudio->Music, 1);}
#endif
	
}
void Game::Update()
{
	PreUpdate();
	pLevel->Update();
	UpdateEnemies();
	UpdateBullets();
	UpdateParticleEmitter();
	UpdatePlayer();
	PostUpdate();
}

void Game::HeldKey_Handler()
{
	if( MOVERIGHT_KEYS_HELD ) 
	{
		pPlayer->direction = RIGHT;
	}
	else if( MOVELEFT_KEYS_HELD ) 
	{
		pPlayer->direction = LEFT;
	}

	if( keys[SDLK_UP] || keys[SDLK_w] ) 
	{
		pPlayer->updown_pressed = UP;
	}
	else if( keys[SDLK_DOWN] || keys[SDLK_s] ) 
	{
		pPlayer->updown_pressed = DOWN;
	}
	else
	{
		pPlayer->updown_pressed = NONE;
	}
	
	if( FIRE_KEY_HELD )
	{
		if( mode == MODE_GAME ) 
		{
			pPlayer->Fire( );
		}
	}
}

void Game::EventHandler()
{
	while ( SDL_PollEvent( &event ) )
	{
		UniversalEventHandler(&event);

		switch ( event.type )
		{
			case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					// universal!
					mode = MODE_MAINMENU;
				}
				else if( FIRE_KEY_TRIGGERED )
				{
					pPlayer->Fire( );
				}
				else if( JUMP_KEYS_TRIGGERED )
				{
					pPlayer->Jump();
				}
				else if( event.key.keysym.sym == SDLK_1 )
				{
					pPlayer->ChangeActiveWeapon( WEAPON_PISTOL );	
				}
				else if( event.key.keysym.sym == SDLK_2 ) 
				{
					pPlayer->ChangeActiveWeapon( WEAPON_MACHINEGUN );	
				}
				else if( event.key.keysym.sym == SDLK_3 ) 
				{
					pPlayer->ChangeActiveWeapon( WEAPON_LASER_X1 );
				}
				else if ( event.key.keysym.sym == SDLK_BACKQUOTE )
				{
					oldmode = mode;
					mode = MODE_CONSOLE;	
				}
				else if( event.key.keysym.sym == SDLK_F8 )
				{
					mode = MODE_LEVELEDITOR;
				}		
				
				else if( event.key.keysym.sym == SDLK_F1 ) // Particle Test
				{
					AddParticleEmitter( pPlayer->posx + ( pPlayer->width/2 ), pPlayer->posy + ( pPlayer->height/2 ) - 10, 9, 255, 250, 200, 0.01, 500, 5 );
				}
				else if( event.key.keysym.sym == SDLK_F2 ) // Enemy Test
				{
					AddEnemy( 200, 0 + (double)( Screen->h/2 ), ENEMY_AF373 );
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					pPlayer->Fire();
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{
					pPlayer->Jump();
				}
				break;					
			}
			case SDL_MOUSEBUTTONUP:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{

				}
				break;	
			}
			default:
				{
					
					break;
				}
		}
	}
}





