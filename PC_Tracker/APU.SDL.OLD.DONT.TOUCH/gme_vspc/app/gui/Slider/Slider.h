#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "globals.h"
#include "Render_Context.h"
#include "gui/Fill_Rect.h"

// The Slider is a primitive graphics object that allows linear adjustments of values
/* When initializing a Slider, you specify it's position, it's graphical width (visual), and the lowest and highest values it can be set to.
 
 The magic lies in the ratio calculations.
 
 
 
 ToDo: The adjuster width and height are Constant values.. There should be more dynamic ways to make a slider that is adjustable in size.
			this means adjusting its panel graphics, and adjuster width and height
 
 So there is a relationship between the panel height and the adjuster height.
 
		Vertical Sliders...
 
 */

//#include "OS.h"
#include <SDL.h>
#include "Range.h"
#include "utility.h"


template <class T>
struct Slider : public Render_Context
{
public:
	Slider(int x, int y, 
		int panel_width, int panel_height, 
		int adjuster_width, int adjuster_height, 
		T range_min, T range_max,
		T default_value=0,
		int (*action)(void *data)=NULL);

	bool receive_event(SDL_Event &ev);

	int (*action)(void* data);
	
	void DoRatio();
	void draw();
	void Slide(int mouse_x);	// Slide to this X value, do bounds checking
	void Activate(int mouse_x);
	void Deactivate();
	T getTargetValue(int adjuster_pixel_offset);
	T getTargetValue();
	void setTargetValue(T v);
	int getPixelValueFromTargetValue(T);
	void SetAdjusterXOffset(int);

	// when you activate the slider, you log it's adjuster's x_coordinate
	// as well as the Mouses' X Coordinate
	int adjuster_x_logged, mouse_x_logged;

	
	void SetAdjusterPos(int x);

	/*int width, height;
	int panel_x,panel_y;*/
	//SDL_Rect panel_rect

	
	
	Range<T> target_valueRange;

	float target_numValuesInRange;
	float slider_pixelRange;

	float ratio;

	SDL_Rect adjuster_rect, panel_rect;
	int adjuster_x, adjuster_y;
	bool is_sliding=false;
};

#include "Slider.tpp"


#endif