#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "Globals.h"

//struct GraphicsObject;

class Slider 
{
public:
	Slider(int x, int y, int width, int range_min, int range_max);
	void DoRatio();
	void Draw();
	void Slide(int x);	// Slide to this X value, do bounds checking
	void Activate();
	int doCalculation(int);
	int doCalculation();
	int doInverseCalculation(int);
	void SetAdjusterX(int);

	// when you activate the slider, you log it's adjuster's x_coordinate
	// as well as the Mouses' X Coordinate
	int adjuster_x_logged, mouse_x_logged;

	
	void SetAdjusterPos(int x);

	int width;
	int panel_x,panel_y;

	int target_min, target_max;
	int slider_min, slider_max;

	float slider_range;
	float target_range;

	float ratio;

	SDL_Rect adjuster_rect, panel_rect;
	int adjuster_x, adjuster_y;



};


#endif