#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "Globals.h"

// The Slider is a primitive graphics object that allows linear adjustments of values
/* When initializing a Slider, you specify it's position, it's graphical width (visual), and the lowest and highest values it can be set to.
 
 The magic lies in the ratio calculations.
 
 
 
 To-Do: The adjuster width and height are Constant values.. There should be more dynamic ways to make a slider that is adjustable in size.
			this means adjusting its panel graphics, and adjuster width and height
 
		Vertical Sliders...
 
 */



class Slider 
{
public:
	Slider(int x, int y, int width, int range_min, int range_max);
	
	
	void DoRatio();
	void Draw();
	void Slide(int mouse_x);	// Slide to this X value, do bounds checking
	void Activate();
	int getTargetValue(int adjuster_pixel_offset);
	int getTargetValue();
	int getPixelValueFromTargetValue(int);
	void SetAdjusterXOffset(int);

	// when you activate the slider, you log it's adjuster's x_coordinate
	// as well as the Mouses' X Coordinate
	int adjuster_x_logged, mouse_x_logged;

	
	void SetAdjusterPos(int x);

	int width;
	int panel_x,panel_y;

	
	
	Range<int> target_valueRange;

	float target_numValuesInRange;
	float slider_pixelRange;

	float ratio;

	SDL_Rect adjuster_rect, panel_rect;
	int adjuster_x, adjuster_y;



};


#endif