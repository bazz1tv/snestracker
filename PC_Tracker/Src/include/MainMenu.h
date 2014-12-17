#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "sprite.h"
#include "TextObject.h"
#include "Misc.h"
#include "MiniEngine.h"
#include "OptionsMenu.h"



extern cImageManager *IMan;
extern cSoundManager *SMan;
//extern cFramerate *pFramerate;
extern cAudio *pAudio;
//extern cFont *pFont;

// submode definitions
#define MAIN	1
#define OPTIONS 0

#define MAINMENU_MUSIC DIR_MUSIC "fight.ogg"

class OptionsMenu;

class cMainMenu : public MiniEngine
{
public:
	cMainMenu();
	~cMainMenu();


	cBasicSprite *Button_Start,*Button_Exit;
	cBasicSprite *Menu_Background;
	
	cBasicSprite *Light1;
	
	OptionsMenu optionsmenu;
	TTF_Font *myfont;
	TextObject *tOptions;

	static int submode;
	SDL_bool Light_reverse;

	void FadeOutBG();
	void PostDraw();
	//void Loop();
	void Update();
	void Collisions();
	void UpdateGraphics();
	void UpdateHardware();
	void UpdateLogic();
	int Do();
	int EventHandler();
};


#endif