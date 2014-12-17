#ifndef __OPTIONSMENU_H__
#define __OPTIONSMENU_H__


#include "Globals.h"
#include "MiniEngine.h"
#include "TextObject.h"
#include "Slider.h"
#include "Misc.h"
#include "preferences.h"

/// @ingroup Options_Screen_Statuses
// @{
#define BROWSING 0
#define SLIDING_SVOL	1
#define SLIDING_MVOL 2
#define EDITING	3
// @}

// Controls status
enum Screens {
	OPTIONS_SCREEN,
	CONTROLS_SCREEN,
};



class ControlsMenu : public MiniEngine
{
	public:
	enum { KEY_LEFT, KEY_RIGHT, KEY_FIRE, KEY_JUMP, };
	Uint8 status;
	
	ControlsMenu();
	~ControlsMenu();

	void Update();
	void Draw();
	int EventHandler();
	void Collisions();
	
	void GetInput(Uint8);

	TTF_Font *font;

	// actual Text
	TextObject *tExit, *tMoveLeft, *tMoveRight, *tFirePrimary, *tJump;
	TextObject *tWaitForInput;

	// Text representing the current Controls preferences
	TextObject *tMoveLeft_CurKey, *tMoveRight_CurKey, *tFirePrimary_CurKey, *tJump_CurKey;
};



class OptionsMenu : public MiniEngine
{
public:
	
	Uint8 status;
	static Screens curscreen;
	

	ControlsMenu controls_menu;
	
	OptionsMenu();
	~OptionsMenu();

	int Do();
	void PreUpdate();
	void Update();
	void Draw();
	int EventHandler();
	int Collisions();
	int LiveInput();
	
	TTF_Font *optionsfont;

	TextObject *tExit, *tMusicVol, *tSoundVol;
	TextObject *tControls;
	
	// TODO: implement these
	TextObject *tFullscreen, *tScreenWidth, *tScreenHeight, *tScreenBpp;
	
	Slider *sMusicVol, *sSoundVol;
	// inherited from Console (not literally)
	double ttDrawCur;
	SDL_bool DrawCur;
	string livestring;
	unsigned int CurrentScreen;
};


#endif