/** @addtogroup Level
 
*/
/*@{*/
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Globals.h"
#include "sprite.h"
#include "ObjectManager.h"

class cLevel;
extern cLevel *pLevel;


class cLevelData
{
public:
	cLevelData( void );
	~cLevelData( void );

	/// The Basic Sprites
	//cMVelSprite **BasicSprites;
	/// Basic Sprite Count
	//unsigned int BasicSpriteCount;
	ObjectManager<cMVelSprite> BasicSprites;
	/// Background Color
	Uint8 BG_red,BG_green,BG_blue;
	/// Background Image
	/// not implemented

	/// Adds an Sprite to the Leveldata
	void AddSprite( cMVelSprite *Sprite );

	/// Gets the first overlapping Collision
	int GetCollidingSpriteNum( SDL_Rect *Crect );
	SDL_bool GetAllCollidingSpriteNum( SDL_Rect *Crect, ObjectManager<cMVelSprite> *obj_man );
	cBasicSprite *GetCollidingSprite( SDL_Rect *Crect );

	/// Delets an given Sprite
	void DeleteSprite( unsigned int number );
};


/* New modes of rendering Layers... TILE MODE

BackGround Slow_Scroll Mode

*/


/// Basic Level Class
/// This time maybe with Alpha,glimming and Animation support ?
class cLevel
{
public:
	cLevel( void );
	~cLevel( void );
	
	void PrintSaveHeader();
	
	void SavePlayerPos();
	
	void SaveMusicFile();
	
	void SaveMapObjects();
	
	void SaveMapEnemies();
	
	/// Loads an Level
	void Load( string filename );

	/// Unloads the Level
	void Unload( void );

	/// Saves the Current Level
	void Save( void );
	void SaveToFile(string &filename);

	/// Updates the Level
	void Update( void );
	
	/// Draws the Level
	void Draw( void );

	/// The Level File
	string levelfile;

	/// The Level Data
	cLevelData *pLevelData_Layer1;
	cLevelData *pLevelData_Layer2;
	
	/// The Music
	string Musicfile;


private:
	/// Cuts the parts out and sends it to the ParseLine function
	void Parse( char* command, int line );
	/// Parses the Command and also reports any Error
	int ParseLine( char ** parts, unsigned int count, unsigned int line );
	
	char row[300];
	//unsigned int i;
	std::ofstream *ofs;
};
/*@}*/
#endif
