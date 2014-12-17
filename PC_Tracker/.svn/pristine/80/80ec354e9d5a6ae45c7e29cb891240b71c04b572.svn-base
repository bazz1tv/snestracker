/** @addtogroup Level
 
*/
/*@{*/
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Globals.h"

class cLevelData
{
public:
	cLevelData( void );
	~cLevelData( void );

	/// The Basic Sprites
	cMVelSprite **BasicSprites;
	/// Basic Sprite Count
	unsigned int BasicSpriteCount;
	/// Background Color
	Uint8 BG_red,BG_green,BG_blue;
	/// Background Image
	/// not implemented

	/// The Music
	string Musicfile;

	/// Adds an Sprite to the Leveldata
	void AddSprite( cMVelSprite *Sprite );

	/// Gets the first overlapping Collision
	int GetCollidingSpriteNum( SDL_Rect *Crect );
	SDL_bool GetAllCollidingSpriteNum( SDL_Rect *Crect, ObjectManager<cMVelSprite> *obj_man );
	cBasicSprite *GetCollidingSprite( SDL_Rect *Crect );

	/// Delets an given Sprite
	void DeleteSprite( unsigned int number );
};

/// Basic Level Class
/// This time maybe with Alpha,glimming and Animation support ?
class cLevel
{
public:
	cLevel( void );
	~cLevel( void );
	
	/// Loads an Level
	void Load( string filename );

	/// Unloads the Level
	void Unload( void );

	/// Saves the Current Level
	void Save( void );

	/// Updates the Level
	void Update( void );
	
	/// Draws the Level
	void Draw( void );

	/// The Level File
	string levelfile;

	/// The Level Data
	cLevelData *pLevelData;


private:
	/// Cuts the parts out and sends it to the ParseLine function
	void Parse( char* command, int line );
	/// Parses the Command and also reports any Error
	int ParseLine( char ** parts, unsigned int count, unsigned int line );
};
/*@}*/
#endif
