
#include "OptionsMenu.h"
#include "MainMenu.h"
#include "colors.h"

extern cPreferences *pPreferences;

SDL_bool mouse_alreadypressed = SDL_FALSE;

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
	// Sound effect for Sound Volume Adjustment
	SMan->Add( pAudio->LoadSound( DIR_SOUNDS_GUNS "pistol_1.ogg" ), "pistol" );

	
	// Init Font ////
	optionsfont = cFont::CreateFont( DIR_FONT "NIMBU14.TTF", 40, TTF_STYLE_BOLD );

	
	// Font Coords
	int x=20;
	int y=20;

	tMusicVol   = new TextObject(x,y,"Music Volume", optionsfont);
	tSoundVol   = new TextObject(x,y+40,"Sound Volume", optionsfont);
	tControls	= new TextObject(x,y+180, "Controls", optionsfont);
	tExit		= new TextObject(x,window.h-40,"Exit", optionsfont);
	
	// Sky Blue
	tControls->SetFGColor(SkyBlue);

	

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

int OptionsMenu::Do()
{
	mode = OPTIONS;
	while (mode == OPTIONS)
	{
		if (curscreen == OPTIONS_SCREEN)
		{
			mode = EventHandler();
			Update();
			Draw();
		}
		else if (curscreen == CONTROLS_SCREEN)
		{
			// Do specifics for The Controls Screen
			mode = controls_menu.EventHandler();
			PreUpdate();
			controls_menu.Update();
			controls_menu.Draw();
		}
	}
	
	return mode;
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
			pAudio->PlayMusik(DIR_MUSIC "badmofo.mod");
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

	//PostDraw();
	DrawFramerate();
	DrawAllToScreen();
}


int OptionsMenu::LiveInput( void )
{
	//SDL_EnableUNICODE( 1 );				// http://sdl.beuc.net/sdl.wiki/SDL_EnableUNICODE
	Uint8 mode=OPTIONS;
	
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case SDL_QUIT:
			{
				return mode=MODE_QUIT;
				break;
			}
		case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_BACKQUOTE)
				{
					//done = 1;
					mode = MAIN;
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
					/*if( event.key.keysym.unicode )
					{
						livestring += (char)event.key.keysym.unicode;
					}*/ //migration
				}
				
				break;
			}
		default:
			break;
		}
	}
	return mode;
	//SDL_EnableUNICODE( 0 );
}

int OptionsMenu::EventHandler()
{
	Uint8 submode=OPTIONS;
	
	while ( SDL_PollEvent( &event ) )
	{
		submode = UniversalEventHandler(&event);
		if (submode != OPTIONS)
			return submode;
		
		submode = Collisions();
		if (submode != OPTIONS) return submode;
		
		switch ( event.type )
		{
			case SDL_QUIT:
			{
				pPreferences->Apply();
				//done = MODE_QUIT;
				return mode = MODE_QUIT;
				break;
			}
			case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					pPreferences->Apply();
					submode = MAIN;
				}
				else if( event.key.keysym.sym == SDLK_c )
				{
					curscreen = CONTROLS_SCREEN;
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
	
	return submode;
}

int OptionsMenu::Collisions()
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
						return MAIN; // Exit
					}
					else if (MouseCollidesWith(&tControls->rect) )
					{
						//DEBUGLOG("Derp");
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
	
	return OPTIONS;
}


//------------------------------------------------------------------------------------
ControlsMenu::ControlsMenu()
{
	status = BROWSING;
	// Init Font ////
	font = cFont::CreateFont( DIR_FONT "NIMBU14.TTF", 40, TTF_STYLE_BOLD );

	int x=20;
	int y=20;

	tMoveLeft		= new TextObject(x,y,			"Move Left: ",	font);
	tMoveRight		= new TextObject(x,y+40,		"Move Right: ",	font);
	tFirePrimary	= new TextObject(x,y+180,		"Fire: ", font);
	tJump			= new TextObject(x,y+220,		"Jump: ",			font);
	tExit			= new TextObject(x,window.h-40,"Exit",			font);
	tWaitForInput	= new TextObject(window.w/2,window.h/2,"Press a Key...", font);

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
	tWaitForInput->rect.x = (window.w/2 - tWaitForInput->surface->w/2);
	tWaitForInput->rect.y = (window.h/2 - tWaitForInput->surface->h/2);

	
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

	DrawFramerate();
	DrawAllToScreen();
}

int ControlsMenu::EventHandler()
{
	//Uint8 mode;
	while ( SDL_PollEvent( &event ) )
	{
		UniversalEventHandler(&event);
		Collisions();
		
		switch ( event.type )
		{
			case SDL_QUIT:
			{
				pPreferences->Apply();
				return mode=MODE_QUIT;
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
				else if( event.key.keysym.sym == SDLK_l )
				{
					GetInput(KEY_LEFT);
				}
				else if( event.key.keysym.sym == SDLK_r )
				{
					GetInput(KEY_RIGHT);
				}
				else if( event.key.keysym.sym == SDLK_f )
				{
					GetInput(KEY_FIRE);
				}
				else if( event.key.keysym.sym == SDLK_j )
				{
					GetInput(KEY_JUMP);
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
	return 0;
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