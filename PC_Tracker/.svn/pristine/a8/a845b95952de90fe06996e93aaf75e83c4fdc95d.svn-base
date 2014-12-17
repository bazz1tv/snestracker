
#include "OptionsMenu.h"
#include "MainMenu.h"

extern cPreferences *pPreferences;

SDL_bool mouse_alreadypressed =SDL_FALSE;

int channel=0;
void playSoundEffect()
{
	if (!Mix_Playing(channel))
		channel = pAudio->PlaySound( SMan->GetPointer( "pistol" ) );
}

Screens OptionsMenu::curscreen=OPTIONS_SCREEN;

OptionsMenu::OptionsMenu()
{
	//curscreen = OPTIONS_SCREEN;
	status = BROWSING;
	SMan->Add( pAudio->LoadSound( SOUNDS_DIR "pistol_1.ogg" ), "pistol" );

	
	// Init Font ////
	optionsfont = pFont->CreateFont( FONT_DIR "NIMBU14.TTF", 40, TTF_STYLE_BOLD );

	int x=20;
	int y=20;

	tMusicVol   = new TextObject(x,y,"Music Volume", optionsfont);
	tSoundVol   = new TextObject(x,y+40,"Sound Volume", optionsfont);
	tControls	= new TextObject(x,y+180, "Controls", optionsfont);
	tExit		= new TextObject(x,window_height-40,"Exit", optionsfont);


	tMusicVol->SetFGColor(SetColor(255,0,0));

	//Render
	tMusicVol->Render();
	tSoundVol->Render();
	tExit->Render();
	tControls->Render();

	sMusicVol = new Slider(x+tMusicVol->surface->w+25, tMusicVol->rect.y+16, 382, 0, 128);
	sSoundVol = new Slider(sMusicVol->panel_x, tSoundVol->rect.y+16, 382, 0, 128);
	// Set the Adjusters to lie on the Values of our previous User Settings
	sMusicVol->SetAdjusterXOffset(sMusicVol->getPixelValueFromTargetValue(pPreferences->pSettings->mvol));
	sSoundVol->SetAdjusterXOffset(sSoundVol->getPixelValueFromTargetValue(pPreferences->pSettings->svol));
}

OptionsMenu::~OptionsMenu()
{
	delete sSoundVol;
	delete sMusicVol;

	delete tMusicVol;
	delete tSoundVol;
	delete tExit;
	delete tControls;

	if( optionsfont ) 
	{
		TTF_CloseFont( optionsfont );
	}

}

void OptionsMenu::Do()
{
	if (curscreen == OPTIONS_SCREEN)
	{
		EventHandler();
		Update();
		Draw();
	}
	else if (curscreen == CONTROLS_SCREEN)
	{
		// Do specifics for The Controls Screen
		controls_menu.EventHandler();
		PreUpdate();
		controls_menu.Update();
		controls_menu.Draw();
	}
}

void OptionsMenu::PreUpdate()
{
	pMouse->Update(Renderer);
	pAudio->Update();
	pFramerate->SetSpeedFactor( );
}
void OptionsMenu::Update()
{
	PreUpdate();

	tMusicVol->Update();
	tExit->Update();
	tSoundVol->Update();
	tControls->Update();

	if (status == SLIDING_SVOL)
	{
		sSoundVol->Slide((int)floor(pMouse->posx));
		int svol = sSoundVol->getTargetValue();
		pPreferences->pSettings->svol = svol;
		pAudio->SetAllSoundsVolume(svol);
		playSoundEffect();
		// that function will play a sound effect only if one isn't playing already
		// look into SDL_Mixer doc
	}

	if (status == SLIDING_MVOL)
	{
		sMusicVol->Slide((int)floor(pMouse->posx));
		int mvol = sMusicVol->getTargetValue();
		pPreferences->pSettings->mvol = mvol;
		pAudio->SetMusicVolume(mvol);
		// If music is not playing, this section here should throw on a tune
		if (pAudio->MusicPlaying() !=SDL_TRUE)
			pAudio->PlayMusik(MUSIC_DIR "badmofo.mod");
	}
	
}


void OptionsMenu::Draw()
{
	//SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 00, 0, 0 ) );
	
	SDL_RenderClear(Renderer);
	
	
	tMusicVol->Draw();
	tExit->Draw();
	tSoundVol->Draw();
	tControls->Draw();

	sMusicVol->Draw();
	sSoundVol->Draw();
	

	pMouse->Draw( Renderer );

	PostDraw();
}


void OptionsMenu::LiveInput( void )
{
	//SDL_EnableUNICODE( 1 );				// http://sdl.beuc.net/sdl.wiki/SDL_EnableUNICODE
	
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case SDL_QUIT:
			{
				done = 2;
				break;
			}
		case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_BACKQUOTE)
				{
					//done = 1;
					mode = oldmode;
				}

				else if ( event.key.keysym.sym == SDLK_BACKSPACE )
				{
					if ( !livestring.empty() )
					{
						livestring.erase( livestring.end()-1 , livestring.end() );
					}
				}	
				else if ( event.key.keysym.sym == SDLK_RETURN )
				{	
					// ACCEPT THE INPUT
				}
				else if ( event.key.keysym.sym == SDLK_UP )
				{	
				}
				else if ( event.key.keysym.sym == SDLK_DOWN )
				{	
				}
				else
				{
					if( event.key.keysym.unicode )
					{
						livestring += (char)event.key.keysym.unicode;
					}
				}
				
				break;
			}
		default:
			break;
		}
	}
	
	//SDL_EnableUNICODE( 0 );
}

void OptionsMenu::EventHandler()
{		
	while ( SDL_PollEvent( &event ) )
	{
		UniversalEventHandler(&event);
		Collisions();
		switch ( event.type )
		{
			case SDL_QUIT:
			{
				pPreferences->Apply();
				done = MODE_QUIT;
				break;
			}
			case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					pPreferences->Apply();
					MainMenu::submode = MAIN;
				}
				else if( event.key.keysym.sym == SDLK_RETURN )
				{
					
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				break;					
			}
			default:
				break;
		}
	}
}

void OptionsMenu::Collisions()
{
	Uint8 ms = SDL_GetMouseState(NULL,NULL);
	if ((ms & SDL_BUTTON(SDL_BUTTON_LEFT)) && (status == SLIDING_SVOL || status == SLIDING_MVOL))
	{
		
	}
	switch ( event.type )
	{
		case SDL_KEYDOWN:
		{
			
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			{
				if (event.button.button == 1)
				{
					if (MouseCollidesWith(&sSoundVol->adjuster_rect) && status == BROWSING)
					{
						pMouse->hide();
						sSoundVol->Activate();
						status = SLIDING_SVOL;
					}
					else if (MouseCollidesWith(&sMusicVol->adjuster_rect) && status == BROWSING)
					{
						pMouse->hide();
						sMusicVol->Activate();
						status = SLIDING_MVOL;
					}
				}
				break;
			}
		case SDL_MOUSEBUTTONUP:
		{
			if( event.button.button == 1 )
			{
				if (status == BROWSING)
				{
					if( MouseCollidesWith( &tExit->rect ) ) 
					{
						pPreferences->Apply();
						MainMenu::submode = MAIN; // Exit
					}
					else if (MouseCollidesWith(&tControls->rect) )
					{
						DEBUGLOG("Derp");
						curscreen = CONTROLS_SCREEN;
					}
				}

				// These will Release the SLIDING status after the user releases the mouse
				// (the user is only sliding when the mouse is down)
				else if (status == SLIDING_SVOL)
				{
					status = BROWSING;
					pMouse->show();
				}
				else if (status == SLIDING_MVOL)
				{
					status = BROWSING;
					pMouse->show();
				}
		
			}
			break;					
		}
		default:
			break;
	}
}


//------------------------------------------------------------------------------------
ControlsMenu::ControlsMenu()
{
	status = BROWSING;
	// Init Font ////
	font = pFont->CreateFont( FONT_DIR "NIMBU14.TTF", 40, TTF_STYLE_BOLD );

	int x=20;
	int y=20;

	tMoveLeft		= new TextObject(x,y,			"Move Left: ",	font);
	tMoveRight		= new TextObject(x,y+40,		"Move Right: ",	font);
	tFirePrimary	= new TextObject(x,y+180,		"Fire: ", font);
	tJump			= new TextObject(x,y+220,		"Jump: ",			font);
	tExit			= new TextObject(x,window_height-40,"Exit",			font);
	tWaitForInput	= new TextObject(window_width/2,window_height/2,"Press a Key...", font);

	//Render
	tMoveLeft->Render();
	tMoveRight->Render();
	tExit->Render();
	tJump->Render();
	tFirePrimary->Render();

//Key_walk_left,Key_walk_right,Key_shoot_primary;
//	SDLKey Key_jump;

	tMoveLeft_CurKey	= new TextObject(x+tMoveLeft->surface->w+20, tMoveLeft->rect.y, 
		SDL_GetKeyName(pPreferences->pSettings->Key_walk_left), font);
	tMoveRight_CurKey	= new TextObject(x+tMoveRight->surface->w+20, tMoveRight->rect.y, 
		SDL_GetKeyName(pPreferences->pSettings->Key_walk_right), font);
	tFirePrimary_CurKey	= new TextObject(x+tFirePrimary->surface->w+20, tFirePrimary->rect.y, 
		SDL_GetKeyName(pPreferences->pSettings->Key_shoot_primary), font);
	tJump_CurKey		= new TextObject(x+tJump->surface->w+20, tJump->rect.y, 
		SDL_GetKeyName(pPreferences->pSettings->Key_jump), font);

	SDL_Color col = SetColor(255,0,0);
	tMoveLeft_CurKey->SetFGColor(col);
	tMoveRight_CurKey->SetFGColor(col);
	tFirePrimary_CurKey->SetFGColor(col);
	tJump_CurKey->SetFGColor(col);

	SDL_Color bgcol = SetColor(20,20,20);
	tWaitForInput->SetBGColor(bgcol);

	tMoveLeft_CurKey->Render();
	tMoveRight_CurKey->Render();
	tFirePrimary_CurKey->Render();
	tJump_CurKey->Render();
	tWaitForInput->Render_shaded();
	tWaitForInput->rect.x = (window_width/2 - tWaitForInput->surface->w/2);
	tWaitForInput->rect.y = (window_height/2 - tWaitForInput->surface->h/2);

	
}

ControlsMenu::~ControlsMenu()
{
	delete tMoveLeft;
	delete tMoveRight;	
	delete tFirePrimary;
	delete tJump;
	delete tExit;		
	
	delete tMoveLeft_CurKey;
	delete tMoveRight_CurKey;
	delete tFirePrimary_CurKey;
	delete tJump_CurKey;

	delete tWaitForInput;

	if( font ) 
	{
		TTF_CloseFont( font );
	}

	
}

void ControlsMenu::Update()
{

	tMoveLeft->Update();	
	tMoveRight->Update();	
	tFirePrimary->Update();
	tJump->Update();
	tExit->Update();

	tMoveLeft_CurKey->Update();	
	tMoveRight_CurKey->Update();
	tFirePrimary_CurKey->Update();
	tJump_CurKey->Update();

	if (tMoveLeft_CurKey->text	!= SDL_GetKeyName(pPreferences->pSettings->Key_walk_left))
	{
		tMoveLeft_CurKey->text = SDL_GetKeyName(pPreferences->pSettings->Key_walk_left);
		tMoveLeft_CurKey->Render();
	}
	if (tMoveRight_CurKey->text	!= SDL_GetKeyName(pPreferences->pSettings->Key_walk_right))
	{
		tMoveRight_CurKey->text	= SDL_GetKeyName(pPreferences->pSettings->Key_walk_right);
		tMoveRight_CurKey->Render();
	}
	if (tFirePrimary_CurKey->text != SDL_GetKeyName(pPreferences->pSettings->Key_shoot_primary))
	{
		tFirePrimary_CurKey->text = SDL_GetKeyName(pPreferences->pSettings->Key_shoot_primary);
		tFirePrimary_CurKey->Render();
	}
	
	if (tJump_CurKey->text	!= SDL_GetKeyName(pPreferences->pSettings->Key_jump))
	{
		tJump_CurKey->text	= SDL_GetKeyName(pPreferences->pSettings->Key_jump);
		tJump_CurKey->Render();
	}

	
	
	
	

}

void ControlsMenu::Draw()
{
	//SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 00, 0, 0 ) );
	SDL_RenderClear(Renderer);
	
	tMoveLeft->Draw();	
	tMoveRight->Draw();	
	tFirePrimary->Draw();
	tJump->Draw();
	tExit->Draw();

	tMoveLeft_CurKey->Draw();	
	tMoveRight_CurKey->Draw();
	tFirePrimary_CurKey->Draw();
	tJump_CurKey->Draw();

	pMouse->Draw( Renderer );

	if (status == EDITING)
	{
		tWaitForInput->Draw();
	}

	PostDraw();
	
}

void ControlsMenu::EventHandler()
{
	while ( SDL_PollEvent( &event ) )
	{
		UniversalEventHandler(&event);
		Collisions();
		switch ( event.type )
		{
			case SDL_QUIT:
			{
				pPreferences->Apply();
				done = MODE_QUIT;
				break;
			}
			case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					pPreferences->Apply();
					OptionsMenu::curscreen = OPTIONS_SCREEN;
				}
				else if( event.key.keysym.sym == SDLK_RETURN )
				{
					
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				DEBUGLOG("crap");
				break;					
			}
			default:
				break;
		}
	}
}



void ControlsMenu::Collisions()
{
	switch ( event.type )
	{
		case SDL_KEYDOWN:
		{
			
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			{
				if (event.button.button == 1)
				{
					
				}
				break;
			}
		case SDL_MOUSEBUTTONUP:
		{
			if( event.button.button == 1 )
			{
				DEBUGLOG("CRAP");
				if (status == BROWSING)
				{
					if (MouseCollidesWith(&tMoveLeft_CurKey->rect) )
					{
						//status = EDITING;
						GetInput(KEY_LEFT);
					}
					else if (MouseCollidesWith(&tMoveRight_CurKey->rect))
					{
						//status = EDITING;
						GetInput(KEY_RIGHT);
					}
					else if (MouseCollidesWith(&tFirePrimary_CurKey->rect) )
					{
						//status = EDITING;
						GetInput(KEY_FIRE);
					}
					else if (MouseCollidesWith(&tJump_CurKey->rect))
					{
						//status = EDITING;
						GetInput(KEY_JUMP);
					}
					else if( MouseCollidesWith( &tExit->rect ) ) 
					{
						pPreferences->Apply();
						OptionsMenu::curscreen = OPTIONS_SCREEN; // Exit
					}
				}

				// These will Release the SLIDING status after the user releases the mouse
				// (the user is only sliding when the mouse is down)
		
			}
			break;					
		}
		default:
			break;
	}
}

void ControlsMenu::GetInput(Uint8 type)
{
	status = EDITING;
	Draw();
	while (SDL_WaitEvent(&event))
	{
		switch ( event.type )
		{
			//DEBUGLOG("BEEP");
			case SDL_KEYDOWN:
			{
				SDL_Keycode key = event.key.keysym.sym;

				if (type == KEY_LEFT)
				{
					pPreferences->pSettings->Key_walk_left = key;
					goto end;
				}
				else if (type == KEY_RIGHT)
				{
					pPreferences->pSettings->Key_walk_right = key;
					goto end;
				}
				else if (type == KEY_FIRE)
				{
					pPreferences->pSettings->Key_shoot_primary = key;
					goto end;
				}
				else if (type == KEY_JUMP)
				{
					pPreferences->pSettings->Key_jump = key;
					goto end;
				}
				break;
			}
			
			default:
				break;
		}
	}
	
end:
	status = BROWSING;

	
}