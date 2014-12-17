#ifndef __OPTIONSMENU_H__
#define __OPTIONSMENU_H__


class TextObject;
#include "Globals.h"
class Slider;

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



class OptionsMenu;

class ControlsMenu : public MiniEngine
{
	public:
	enum { KEY_LEFT, KEY_RIGHT, KEY_FIRE, KEY_JUMP, };
	Uint8 status;
	
	ControlsMenu();
	~ControlsMenu();

	//void Do();
	void Update();
	void Draw();
	void EventHandler();
	void Collisions();
	
	void GetInput(Uint8);		// NEED TO WRITE THIS

	TTF_Font *font;

	// actual Text
	TextObject *tExit, *tMoveLeft, *tMoveRight, *tFirePrimary, *tJump;
	TextObject *tWaitForInput;

	// TO-DO implement these
	TextObject *tFullscreen, *tScreenWidth, *tScreenHeight, *tScreenBpp;
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

	void Do();
	void PreUpdate();
	void Update();
	void Draw();
	void EventHandler();
	void Collisions();
	//BlinkCursor();
	void LiveInput();
	//Slide(Slider *slider);
	//Keyboard_Joypad_Oneshot();

	//cBasicSprite *Musicvol, *Soundvol; // This is how you store sprites in the class
	//SDL_Surface *cursor;
	//SDL_Event event;
	TTF_Font *optionsfont;

	TextObject *tExit, *tMusicVol, *tSoundVol;
	TextObject *tControls;
	
	Slider *sMusicVol, *sSoundVol;
	// inherited from Console (not literally)
	double ttDrawCur;
	SDL_bool DrawCur;
	string livestring;
	unsigned int CurrentScreen;
};


#endif