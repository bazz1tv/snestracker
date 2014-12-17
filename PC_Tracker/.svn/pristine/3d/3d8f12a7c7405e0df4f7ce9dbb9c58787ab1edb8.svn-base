#include "MiniEngine.h"
#include "Globals.h"

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
	
	pMouse->Update(Renderer);
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
	SDL_RenderClear(Renderer);
	
	pLevel->Draw();
}
void MiniEngine::PostDraw()
{
	FramerateDraw( Screen );
	
	SDL_RenderPresent(Renderer);
}
void MiniEngine::Draw( void )
{
	PreDraw();
    
	DrawBullets(Renderer);
	DrawParticleEmitter(Renderer);
	
	DrawEnemies(Renderer);
	
	pPlayer->Draw( Renderer );
	
	
	PostDraw();
}