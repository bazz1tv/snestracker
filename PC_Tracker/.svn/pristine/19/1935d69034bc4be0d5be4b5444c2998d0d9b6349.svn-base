/** @addtogroup LevelEditor
 @{
 */
#ifndef __LEVELEDITOR_H__
#define __LEVELEDITOR_H__



#include "Globals.h"
class cMVelSprite;

#include "MiniEngine.h"

class cMultiSelect;

// used for poop pants
#define POSITIVE SDL_TRUE
#define NEGATIVE SDL_FALSE



/// @defgroup Mouse_Cmd Mouse Commands
// @{
// The Current Mouse Command
enum {	COMMAND_NOTHING,
		COMMAND_MOVING_SINGLE_TILE,
		COMMAND_MOVING_MULTISELECT_TILES,
		COMMAND_SELECT_MULTISELECT_TILES,
		COMMAND_FASTCOPY
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
	
	int EventHandler();
	void MouseButton_Held_Events();
	int KeyDownEvents(SDL_Event&);
	void MouseButton_Down_Events(SDL_Event&);
	void MouseButton_Up_Events(SDL_Event&);
	
	SDL_Rect GetHoveredObjectRect();

	void Draw();
	void Draw (SDL_Renderer *renderer);
	/// Updates  all the Leveleditor stuff
	void Update( void );

	/// Draws all the Leveleditor stuff
	//void Draw( SDL_Surface *target );

	/// Sets the to-be-Copied Object
	void SetCopyObject( cMVelSprite *nObject );
	void SetCopyObject( void );
	
	void NewMoveObject(cMVelSprite*);
	
	/// Sets an Object for Mouse Movement
	void SetMoveObject( cMVelSprite *nObject );
	void SetMoveObject( void );
	
	
	
	/// Draw the outline of the hovered Object
	void OutlineHoveredObject( SDL_Renderer *renderer);
	void OutlineHoveredObject( SDL_Renderer *renderer, Uint32 Color);
	static void OutlineObject (SDL_Renderer *renderer, Uint32 Color, SDL_Rect *orect);
	
	

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
	
	void MoveSingleTile();
	
	

	/** Gets the Current Colliding Object
	 * types : the Objects which should be checked
	 * look at the Editor type defines
	 */
	cMVelSprite *GetCollidingObject( double x, double y );
	
	/// The Object Size which should get Hovered
	SDL_Rect HoveredObjectRect;

	/// The currently used object for moving
	cMVelSprite *Object;
	/// The Object used for Copying
	cMVelSprite *CopyObject;
	cMVelSprite *lastCopiedObject;

	/// The Mouse Command ( moving,copying etc.. )
	static Uint8 command;

	/// The additional Mouse position when moving
	int Mouse_w,Mouse_h;
	
	cMultiSelect *MultiSelect;
	SDL_bool coordinates;
	Uint8 mouse_state;
};



#endif

// @}