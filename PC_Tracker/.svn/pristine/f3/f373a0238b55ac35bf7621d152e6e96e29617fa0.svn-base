/** @addtogroup LevelEditor
 @{
 */
#ifndef __LEVELEDITOR_H__
#define __LEVELEDITOR_H__


#include "Globals.h"

#include "ObjectManager.h"

// used for poop pants
#define POSITIVE SDL_TRUE
#define NEGATIVE SDL_FALSE

/// @ingroup Event_Handlers
void leveleditor_ehandler(void);



/// @defgroup Mouse_Cmd Mouse Commands
// @{
// The Current Mouse Command
enum {	MOUSE_COMMAND_NOTHING,
		MOUSE_COMMAND_SINGLE_TILE_MOVING,
		MOUSE_COMMAND_MOVING_MULTISELECT_TILES,
		MOUSE_COMMAND_FASTCOPY,
		MOUSE_COMMAND_SELECT_MULTISELECT_TILES
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
	//void Draw( SDL_Surface *target );

	/// Sets the to-be-Copied Object
	void SetCopyObject( cMVelSprite *nObject );
	void SetCopyObject( void );
	
	/// Sets an Object for Mouse Movement
	void SetMoveObject( cMVelSprite *nObject );
	void SetMoveObject( void );
	
	
	
	/// Draw the outline of the hovered Object
	void OutlineHoveredObject( SDL_Renderer *renderer, Uint32 Color);
	void OutlineObject (SDL_Renderer *renderer, Uint32 Color, SDL_Rect *orect);
	
	

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
	
	

	/// Sets the to-be-FastCopied Object
	void SetFastCopyObject( cMVelSprite *nObject );
	void SetFastCopyObject( void );

	/// Releases every command
	void Release_Command( void );
	
	/// Releases any MultiSelect_ selected object
	/// @returns SDL_TRUE if there were objects to remove, SDL_FALSE if there wasnt
	SDL_bool Release_MultiSelect_Select_Objects();

	/** Gets the Current Colliding Object
	 * types : the Objects which should be checked
	 * look at the Editor type defines
	 */
	cMVelSprite *GetCollidingObject( double x, double y );
	
	/// The Object Size which should get Hovered
	SDL_Rect HoveredObject;

	/// The currently used object for moving
	cMVelSprite *Object;
	/// The Object used for Copying
	cMVelSprite *CopyObject;
	cMVelSprite *lastCopiedObject;

	/// The Mouse Command ( moving,copying etc.. )
	Uint8 Mouse_command;

	/// The additional Mouse position when moving
	int Mouse_w,Mouse_h;
	
	// This next section is for the MultiSelect Tiles
/////////////////////////////////////////////////////////
	void DrawOutlineAroundMultiSelect_Tiles(SDL_Renderer *renderer, Uint32 Color);
	
	/// Add to MultiSelect_Objects array
	void SetMultiSelect_Objects( cMVelSprite *nObject);
	void SetMultiSelect_Objects( void );
	// MultiSelect_ move
	void MultiSelect_Move(void);
	// the actual MultiSelect_-copy process
	//void MultiSelect_Copy( void );
	
	void init_MultiSelect_Tiles();
	void prepareToMove_MultiSelect_Tiles();
	
	SDL_bool GetAllCollidingObjects( SDL_Rect *crect, ObjectManager<cMVelSprite> *obj_man );
	
	// this is true if we have MultiSelect_-selected > 1 tile
	// this is because if we only MultiSelect_-select 1 tile. we might as well just move in 1 tile mode
	SDL_bool multiple_objects_selected;

	/// these very first ones are because I want to show the grid around selected tiles all the way until the point that the tiles actually start moving
	// So I take these first offsets of one of the tiles I multiselect (the first one), I show the grid until there is a change from these coordinates
	double veryfirst_MultiSelect_mouseXOffset, veryfirst_MultiSelect_mouseYOffset;
	
	double MultiSelect_mouseXOffset, MultiSelect_mouseYOffset;
	double MultiSelect_camXOffset, MultiSelect_camYOffset;
	
	SDL_Rect MultiSelect_rect;
	//
	double MultiSelect_rectX_origin, MultiSelect_rectY_origin;
	//
	SDL_bool MultiSelect_Rect_XDirection;
	SDL_bool MultiSelect_Rect_YDirection;

	// Object MAnager for MultiSelect_-object
	ObjectManager<cMVelSprite> MultiSelect_Objects;
	
};

#endif

// @}