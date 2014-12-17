#include "MainMenu.h"
#include "Game.h"

#define MAINMENU_MUSIC MUSIC_DIR "fight.ogg"

int cMainMenu::submode = MAIN;

cMainMenu::cMainMenu() : MiniEngine()
{
	Light_reverse =SDL_FALSE;
	
	submode = MAIN;

	myfont = pFont->CreateFont( FONT_DIR "NIMBU14.TTF", 40, TTF_STYLE_BOLD );
	
	//tOptions->rect = SetRect(100,100,tOptions->surface->w, tOptions->surface->h);
	pAudio->PlayMusik( MAINMENU_MUSIC );
	//SMan->Add( pAudio->LoadSound( MUSIC_DIR "menumusic.wav"), MUSIC_DIR "menumusic.wav");
	//pAudio->PlaySound(SMan->GetPointer(MUSIC_DIR "menumusic.wav"));

	// Preload the Button Sound
	SMan->Add( pAudio->LoadSound( SOUNDS_DIR "Button_1.ogg" ), SOUNDS_DIR "Button_1.ogg" );

	/// [Adding a Sprite]
	// Add the image to the Image Manager
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Background3.png", colorkey ), "Menu_Background" );
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Buttons/Start_32bpp.png", colorkey, 140 ), "Start_Button" );
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Buttons/Exit_32bpp.png", colorkey, 140 ), "Exit_Button" );
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Light1.png", colorkey, 0 ), "Light_1" );
	// Register the sprite
	Menu_Background = new cBasicSprite( Renderer, IMan->GetPointer( "Menu_Background" ), 0, 0 );
	Button_Start = new cBasicSprite( Renderer, IMan->GetPointer( "Start_Button" ), window.w - window.w/3, 250 );
	Button_Exit = new cBasicSprite( Renderer, IMan->GetPointer( "Exit_Button" ), window.w - window.w/3, 250 + (int)Button_Start->height + 125);
	Light1 = new cBasicSprite( Renderer, IMan->GetPointer( "Light_1" ), window.w/7-20, window.h/2 );
	/// [Adding a Sprite]

	// Resize The Background image
	Menu_Background->SetSize( (double)window.w, (double)window.h );

	tOptions = new TextObject(window.w - window.w/3, 250 + (int)Button_Start->height + 50, "Options", myfont);
	SDL_Color optionsColor = SetColor(0,0,0);
	tOptions->SetFGColor(optionsColor);
	tOptions->Render();
	
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
}

cMainMenu::~cMainMenu()
{
	delete Button_Exit;
	delete Button_Start;

	delete Light1;
	
	delete Menu_Background;

	delete tOptions;

	//SDL_FreeSurface(tOptions.surface);

	if( myfont ) 
	{
		TTF_CloseFont( myfont );
	}

}

void cMainMenu::UpdateHardware()
{
	pMouse->Update(Renderer);
	pAudio->Update();
}

void cMainMenu::UpdateGraphics()
{
	pFramerate->SetSpeedFactor( );

	Menu_Background->Update(Renderer);
	//Light1->Update();

	Button_Start->Update(Renderer);
	Button_Exit->Update(Renderer);

	tOptions->Update();

	//SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 60, 0, 0 ) );
	
	SDL_RenderClear(Renderer);
	
	Menu_Background->Draw( Renderer ); // Background

	//Light1->Draw( Screen );

	Button_Start->Draw( Renderer );
	Button_Exit->Draw( Renderer );
	tOptions->Draw();
		
	pMouse->Draw( Renderer );
	
	//SDL_RenderPresent(Renderer);
}

/// @todo The Alpha adjustments here could be turned into API calls.
void cMainMenu::Collisions()
{
	
	if( MouseCollidesWith( &Button_Exit->rect ) && Button_Exit->GetAlpha() < 245 ) 
		{
			int nAlpha = Button_Exit->GetAlpha() + (int)( 10.0 * pFramerate->speedfactor );

			if( nAlpha > 255 ) 
			{
				nAlpha = 255;
			}

			Button_Exit->SetAlpha( nAlpha );
		}
		else if( Button_Exit->GetAlpha() > 140 && !MouseCollidesWith( &Button_Exit->rect ) )
		{
			int nAlpha = Button_Exit->GetAlpha() - (int)( 10.0 * pFramerate->speedfactor );

			if( nAlpha < 0 ) 
			{
				nAlpha = 0;
			}

			Button_Exit->SetAlpha( nAlpha );
		}

		if( MouseCollidesWith( &Button_Start->rect ) && Button_Start->GetAlpha() < 245 ) 
		{
			int nAlpha = Button_Start->GetAlpha() + (int)( 10.0 * pFramerate->speedfactor );

			if( nAlpha > 255 ) 
			{
				nAlpha = 255;
			}

			Button_Start->SetAlpha( nAlpha );
		}
		else if( Button_Start->GetAlpha() > 140 && !MouseCollidesWith( &Button_Start->rect ) )
		{
			int nAlpha = Button_Start->GetAlpha() - (int)( 10.0 * pFramerate->speedfactor );

			if( nAlpha < 0 ) 
			{
				nAlpha = 0;
			}

			Button_Start->SetAlpha( nAlpha );
		}
}

void cMainMenu::Update()
{
	UpdateHardware();
	UpdateGraphics();
	Collisions();
	UpdateLogic();
}

void cMainMenu::UpdateLogic()
{
	if( Light_reverse ==SDL_TRUE ) 
		{
			double nAlpha = (double)Light1->GetAlpha();
			if( nAlpha < 200 ) 
			{
				nAlpha += ( 10.0 * pFramerate->speedfactor );
				if( nAlpha > 255 ) 
				{
					nAlpha = 255;
					Light_reverse =SDL_FALSE;
				}
				Light1->SetAlpha( (int)nAlpha  );

			}
			else
			{
				//DEBUGLOG ("setting toSDL_FALSE: %f\n", pFramerate->speedfactor);
				Light_reverse =SDL_FALSE;
			}
		}
		else
		{
			double nAlpha = (double)Light1->GetAlpha();
			if( nAlpha > 35 ) 
			{
				
				if( nAlpha > 100 ) 
				{
					nAlpha -= ( 10.0 * pFramerate->speedfactor );
					
				}
				else
				{
					nAlpha -= ( 2.0 * pFramerate->speedfactor );

				}
				
				if (nAlpha < 0) 
				{
					Light_reverse =SDL_TRUE;
					nAlpha = 0;
				}
				Light1->SetAlpha( (int)nAlpha  );
			}
			else
			{
				Light_reverse =SDL_TRUE;
			}
		}
}



int cMainMenu::Do()
{
	Uint8 mode=0;

	while (mode == 0)
	{
		if (submode == MAIN)
		{
			Update();
			mode = EventHandler();
			//PostDraw();
			DrawAllToScreen();
		}
		else if (submode == OPTIONS)
		{
			submode = optionsmenu.Do();
		}
		
		// if the mode got switched (we pressed start game or quit)
	}
	
	if (mode == MODE_GAME)
	{
		pAudio->PlaySound( SMan->GetPointer( SOUNDS_DIR "Button_1.ogg" ) );
	}
	
	return mode;
}

void cMainMenu::FadeOutBG()
{
	int i; 
	SDL_Surface *surface; 

	//surface = SDL_ConvertSurface(Screen, Screen->format, Screen->flags);
	//SDL_FillRect( surface, NULL, SDL_MapRGB( Screen->format, 0, 0, 0 ) );
	if(surface == NULL) 
	{ 
		DEBUGLOG("Load image error: %s\n", SDL_GetError());
		exit(-1);
	} 
	for (i=0; i < 256; i+=2) 
	{ 
		UpdateGraphics();
		//SDL_SetAlpha(surface,SDL_TRUE,(Uint8)i);
		SDL_SetSurfaceAlphaMod(surface, (Uint8)i);
		//SDL_BlitSurface(surface,NULL,Screen,NULL);
		//PostDraw();
		DrawAllToScreen();
		//Sleep(10);
	}

}

void cMainMenu::PostDraw()
{
	//ScreenTexture = SDL_CreateTextureFromSurface(Renderer, Screen);
	SDL_RenderPresent(Renderer);
	//SDL_Flip( Screen );
}

int cMainMenu::EventHandler()
{
	//int mode=0;
		while ( SDL_PollEvent( &event ) )
		{
			mode = UniversalEventHandler(&event);
			if (mode > 0)
				return mode;
			
			switch ( event.type )
			{
				case SDL_QUIT:
				{
					//done = MODE_QUIT;
					break;
				}
				case SDL_KEYDOWN:
				{
					if( event.key.keysym.sym == SDLK_BACKSLASH && (SDL_GetModState() & KMOD_ALT) )
					{
						SDL_ToggleFS(window.sdlw);
					}
					if( event.key.keysym.sym == SDLK_ESCAPE )
					{
						return mode= MODE_QUIT;
					}
					else if( event.key.keysym.sym == SDLK_RETURN )
					{
						return mode=MODE_GAME;
					}
					else if ( event.key.keysym.sym == SDLK_o )
					{
						submode = OPTIONS;
					}
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					if( event.button.button == 1 )
					{
						/// [Mouse Collision Check]
						if( MouseCollidesWith( &Button_Exit->rect ) ) 
						{
							return mode=MODE_QUIT; // Exit
						}
						else if( MouseCollidesWith( &Button_Start->rect ) ) 
						{
							return mode=MODE_GAME; // Start
						}
						else if( MouseCollidesWith( &tOptions->rect ) )
						{
							submode = OPTIONS;
						}
						/// [Mouse Collision Check]
					}
					break;					
				}
				
				default:
					break;
			}
		}
	
	return 0;
}
