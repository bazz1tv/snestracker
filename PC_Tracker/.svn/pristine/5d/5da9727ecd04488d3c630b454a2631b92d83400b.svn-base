/** @addtogroup LevelEditor
 @{
 */
#ifndef __LEVELEDITOR_H__
#define __LEVELEDITOR_H__

#include "Globals.h"
#include "ObjectManager.h"

/// @ingroup Event_Handlers
void leveleditor_ehandler(void);



/// @defgroup Mouse_Cmd Mouse Commands
// @{
// The Current Mouse Command
enum {	MOUSE_COMMAND_NOTHING,
		MOUSE_COMMAND_SINGLE_TILE_MOVING,
		MOUSE_COMMAND_MOVING_MULTI_TILES,
		MOUSE_COMMAND_FASTCOPY,
		MOUSE_COMMAND_SELECT_MULTI_TILES
};
// @}

/// The Leveleditor as an ingame implementation.
class cLevelEditor : public MiniEngine
{
public:
	cLevelEditor( void );
	~cLevelEditor( void );

	void HeldKey_Handler();
	void HeldKey_fastcopy();
	void HeldKey_movecamera();
	
	void EventHandler();



	void Draw();
	void Draw (SDL_Renderer *renderer);
	/// Updates  all the Leveleditor stuff
	void Update( void );

	/// Draws all the Leveleditor stuff
	void Draw( SDL_Surface *target );

	/// Sets the to-be-Copied Object
	void SetCopyObject( cMVelSprite *nObject );
	void SetCopyObject( void );
	
	/// Sets an Object for Mouse Movement
	void SetMoveObject( cMVelSprite *nObject );
	void SetMoveObject( void );
	
	/// Add to MultiObjects array
	void SetMultiObjects( cMVelSprite *nObject);
	void SetMultiObjects( void );
	// multi move
	void MultiMove(void);
	// the actual multi-copy process
	void MultiCopy( void );
	
	/// Draw the outline of the hovered Object
	void OutlineHoveredObject( SDL_Renderer *renderer, Uint32 Color);
	void OutlineObject (SDL_Renderer *renderer, Uint32 Color, SDL_Rect *orect);
	void DrawOutlineAroundMultiSelectedTiles(SDL_Renderer *renderer, Uint32 Color);
	

	//void SpecialPasteObject(void);
	//void SpecialPasteObject( double x, double y );
	/// Paste's an Object if SetCopyObject was set
	void PasteObject( void );
	/// Paste's an Object on the given position if SetCopyObject was set
	void PasteObject( double x, double y );

	/// Delete's an Object
	void DeleteObject( void );
	/// Delete's an Object on the given position
	void DeleteObject( double x, double y );
	
	void prepareToMove_MultiSelect_Tiles();

	/// Sets the to-be-FastCopied Object
	void SetFastCopyObject( cMVelSprite *nObject );
	void SetFastCopyObject( void );

	/// Releases every command
	void Release_Command( void );
	
	/// Releases any multi selected object
	/// @returns SDL_TRUE if there were objects to remove, SDL_FALSE if there wasnt
	SDL_bool Release_MultiSelect_Objects();

	/** Gets the Current Colliding Object
	 * types : the Objects which should be checked
	 * look at the Editor type defines
	 */
	cMVelSprite *GetCollidingObject( double x, double y );
	
	/// The Object Size which should get Hovered
	SDL_Rect HoveredObject;
	
	// Object MAnager for Multi-object
	ObjectManager<cMVelSprite> MultiObjects;

	/// The currently used object for moving
	cMVelSprite *Object;
	/// The Object used for Copying
	cMVelSprite *CopyObject;
	cMVelSprite *lastCopiedObject;

	/// The Mouse Command ( moving,copying etc.. )
	Uint8 Mouse_command;

	/// The additional Mouse position when moving
	int Mouse_w,Mouse_h;
	
	// this is true if we have multi-selected > 1 tile
	// this is because if we only multi-select 1 tile. we might as well just move in 1 tile mode
	SDL_bool multiple_objects_selected;

	/// these very first ones are because I want to show the grid around selected tiles all the way until the point that the tiles actually start moving
	double veryfirst_multi_mouseXOffset, veryfirst_multi_mouseYOffset;
	
	double multi_mouseXOffset, multi_mouseYOffset;
	double multi_camXOffset, multi_camYOffset;
	
};

#endif

// @}