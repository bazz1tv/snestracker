#include "Slider.h"

// This function draws the panel (background) and actual slider adjuster
void Slider::Draw()
{
	// Get Graphics going
	// void FillRect(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);
	// void FillRectAlpha(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);
	
	FillRectAlpha(Renderer, panel_x, panel_y, width, 10,50,50,50,255); // 50,50,50));
	FillRectAlpha(Renderer, &adjuster_rect, 0, 255, 0, 255); // SDL_MapRGB(Screen->format,0,255,0));
}

// This is obvious
void Slider::SetAdjusterPos(int x)
{
	adjuster_rect.x = x;
}


// Slide() gets called with a Mouse coordinate
// There is a reason why we don't just set x to mouse_x.
/* 1) there is a rect area that is clickable to take focus on the slider... If we only used mouse_x.. the slider will immediately snap to mouse_x position...
 An example shows the weakness of using mouse_x directly
 -------
 |     |
 |	   |
 -------
 
 if you click teh slider at this point:
 
 -------
 |    .|
 |     |
 -------
 
 the slider will immediately adjust its position to mouse_x
 
	 -------
	 |.    |
     |     |
	 -------
 
 This is undesired behavior. We DONT want the slider to change its value just by focusing on it,
 
 so we take an alternate method.
 
 ----------------------------------------------------------------
 
 How the Alternate Method Works
 
 ----------------------------------------------------------------
 When the Slider is Activated (focused), we log both the current adjuster coordinate, and the current mouse_coordinate. We use this historical information to
 base all subsuquent calculations.
 
 We can get a mouse offset by using "mouse_x - mouse_x_logged" then we simply add that to adjuster_x_logged.
 
 */
void Slider::Slide(int mouse_x)
{
	int new_adjuster_x = adjuster_x_logged+(mouse_x-mouse_x_logged);
	// check for MAX X
	if (new_adjuster_x >= panel_x+width)
	{
		adjuster_rect.x = panel_x+width;
		//SDL_WarpMouseInWindow(Window,adjuster_rect.x, pMouse->posy);
	}
	// check for under Min X
	else if (new_adjuster_x <= this->panel_x)
	{
		adjuster_rect.x = this->panel_x;
		//SDL_WarpMouseInWindow(Window,adjuster_rect.x, pMouse->posy);
	}
	else
		adjuster_rect.x = new_adjuster_x;

	printf("Adjuster\n\t");
	printf("[Mouse_X: %d] - [Mouse_X_logged: %d] = %d\n\t", mouse_x, mouse_x_logged, mouse_x-mouse_x_logged);
	printf("adjuster_x_logged: %d | new_adjuster_x: %d\n\t", adjuster_x_logged,adjuster_rect.x);
	printf("Pixel offset: %d. Target value: %d\n", adjuster_rect.x - panel_x, getTargetValue(adjuster_rect.x - panel_x));
}

// This is only here incase for some reason we need to calculation the ratio for a specific value
// otherwise we always base our calculation on the adjuster_X - panel_X offset.
int Slider::getTargetValue(int adjuster_pixel_offset)
{
	return (int)floor((float)adjuster_pixel_offset*ratio);
}

int Slider::getPixelValueFromTargetValue(int target_val)
{
	return (int)floor((float)target_val*(1/ratio));
}

// Applies Ratio to the adjuster offset
// to get the pixel offset of the adjuster, we subtract the panel_x value from the adjuster_X value. this gives us a pure offset.
/* ie
 here is our slider
 ...@Pixel location (100,100)	[]------------------
 this means panel_x = 100.
 the slider_adjuster_x = 100 too. it's value then changes:
 ------[]------------
 [---------- panel_x ----------]
 [====== slider_adjuster_x ========== ]
 [offset]
 
 offset = slider_adjuster_x - panel_x
 
 this offset is really a pixel offset.
 
 then with getTargetValue(), we convert the pixel offset into a value inside the target Range.
 
 */
int Slider::getTargetValue()
{
	return (int)floor((float)(adjuster_rect.x - panel_x)*ratio);
}

Slider::Slider(int x, int y, int width, int range_min, int range_max) :
target_valueRange(range_min, range_max)
{
	this->panel_x = x;
	this->panel_y = y;

	this->width = width;

	adjuster_rect.x = x+(width/2);	// Set the adjuster in the middle of the panel
	adjuster_rect.y = y-3;			// guess

	target_numValuesInRange = float(target_valueRange.max - target_valueRange.min);
	slider_pixelRange = float(width);

	adjuster_rect = SetRect(adjuster_rect.x,adjuster_rect.y, 11, 16);

	// calculate ratio
	DoRatio();
}

// x_offset added to the panel_x
void Slider::SetAdjusterXOffset(int x_offset)
{
	adjuster_rect.x = x_offset+panel_x;
}

// Put focus on the Slider
// this just logs the current Adjuster_X and Mouse_X coordinate for proper subsequent adjustments to the slider
void Slider::Activate()
{
	adjuster_x_logged = adjuster_rect.x;
	mouse_x_logged = (int)floor(pMouse->posx);
}

// Adjuster pixel offset is multiplied by the result of this ratio calculation
/* The following code basically does the following:
 
 say we have a slider
 --[]--------- , it's 100 pixels long. slider_pixelRange
 it adjusts values in range from 0-200. target_numValuesInRange
 
 ratio = target_numValuesInRange / slider_pixelRange;
 200/100 = 2
 
 now we can use the values in the graphics range * ratio. to relate to the range of our target Values
 
 It works the same way in the opposite:
 --[]---------------------- , it's 200 pixels long.
 it adjusts values in range from 0-100.
 
 ratio = target_numValuesInRange / slider_pixelRange;
 ratio = 100/200 = 1/2
 */
void Slider::DoRatio()
{	
	ratio = target_numValuesInRange / slider_pixelRange;
}
