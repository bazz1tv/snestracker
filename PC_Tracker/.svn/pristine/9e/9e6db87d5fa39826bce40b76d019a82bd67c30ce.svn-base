/** @addtogroup LevelEditor
 @{
 */
#ifndef __LEVELEDITOR_H__
#define __LEVELEDITOR_H__

#include "Globals.h"

/// @ingroup Event_Handlers
void leveleditor_ehandler(void);



/// @defgroup Mouse_Cmd Mouse Commands
// @{
// The Current Mouse Command
#define MOUSE_COMMAND_NOTHING 0
#define MOUSE_COMMAND_MOVING 1
#define MOUSE_COMMAND_FASTCOPY 2
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
};

#endif

// @}