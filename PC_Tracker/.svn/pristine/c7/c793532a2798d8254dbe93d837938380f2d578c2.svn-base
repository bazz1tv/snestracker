#include "MiniEngine.h"
#include "level.h"
#include "Camera.h"
#include "player.h"
#include "enemy.h"
#include "effects.h"
#include "bullet.h"

extern cFramerate *pFramerate;
extern cMouseCursor *pMouse;
extern cCamera *pCamera;
extern SDL_Renderer *Renderer;
extern cLevel *pLevel;
extern SDL_bool paused;
extern cPlayer *pPlayer;

MiniEngine::MiniEngine()
{
	mode = 0;
}

int MiniEngine::Do()
{
	// Reset mode to 0
	mode = 0;
	while (mode == 0)
	{
		// Might have to change the whole ONLY checking input while !paused thing...
		if ( !paused )
		{
			Input();	// input will return mode value (however, now we use MiniEngine modevariable, it's kinda pointless)
			Update();
		}
	
		Draw();
	}
	
	return mode;
}

int MiniEngine::Input()
{
	EventHandler();
	HeldKey_Handler();
	
	return (int)mode;
}

/// Does general things
void MiniEngine::PreUpdate( void )
{
	
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
	
}
void MiniEngine::PostDraw()
{
	
}
void MiniEngine::Draw( void )
{
	ClearScreen();
	pLevel->Draw();
    
	DrawBullets(Renderer);
	DrawParticleEmitter(Renderer);
	
	DrawEnemies(Renderer);
	
	pPlayer->Draw( Renderer );
	
	DrawFramerate();	///< only draws framerate if it's set to
	DrawAllToScreen();	// Renders the Renderer' contents to screen
}

void MiniEngine::DrawFramerate()
{
	FramerateDraw( Renderer );
}

void MiniEngine::DrawAllToScreen()
{
	SDL_RenderPresent(Renderer);
}

void MiniEngine::ClearScreen()
{
	ClearScreen(Renderer);
}

void MiniEngine::ClearScreen(SDL_Renderer *renderer)
{
	SDL_RenderClear(renderer);
}

/* Mini Engine Rules
 ----
 1 ) in Do(), while mode == 0, that loop should never leave
*/
int MiniEngineTest()
{
	//MiniEngine test;
	
	
	return 0;
}