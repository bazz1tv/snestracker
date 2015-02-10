//#include "Slider<T>.h"


// This function draws the panel (background) and actual slider adjuster
template <class T>
void Slider<T>::draw()
{
	static SDL_Rect txt_rect = adjuster_rect;
	// Get Graphics going
	// void FillRect(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);
	// void FillRectAlpha(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);
	SDL_FillRect(screen, &txt_rect, 0);
	SDL_Rect value_rect = {panel_rect.x, panel_rect.y, adjuster_rect.x - panel_rect.x, panel_rect.h };
	FillRectAlpha(sdlRenderer, &panel_rect,&colors.panel); // 50,50,50));
	FillRectAlpha(sdlRenderer, &value_rect,&colors.value);
	FillRectAlpha(sdlRenderer, &adjuster_rect, &colors.adjuster); // SDL_MapRGB(Screen->format,0,255,0));

	if (is_sliding)
	{
		char tmpbuf[20];
		snprintf(tmpbuf, 20, "%1.03f", target_value );
		txt_rect.x = adjuster_rect.x - ((strlen(tmpbuf)*CHAR_WIDTH)/2);
		txt_rect.y = adjuster_rect.y-CHAR_HEIGHT;
		txt_rect.h = CHAR_HEIGHT;
		txt_rect.w = strlen(tmpbuf) * CHAR_WIDTH;
		sdlfont_drawString(screen, txt_rect.x, txt_rect.y, tmpbuf, Colors::white);
	}
}

// This is obvious
template <class T>
void Slider<T>::SetAdjusterPos(int x)
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
 When the Slider<T> is Activated (focused), we log both the current adjuster coordinate, and the current mouse_coordinate. We use this historical information to
 base all subsuquent calculations.
 
 We can get a mouse offset by using "mouse_x - mouse_x_logged" then we simply add that to adjuster_x_logged.
 
 */
template <class T>
void Slider<T>::Slide(int mouse_x)
{
	int new_adjuster_x = adjuster_x_logged+(mouse_x-mouse_x_logged);
	// check for MAX X
	if (new_adjuster_x >= panel_rect.x+panel_rect.w)
	{
		adjuster_rect.x = panel_rect.x+panel_rect.w;
		//SDL_WarpMouseInWindow(Window,adjuster_rect.x, pMouse->posy);
	}
	// check for under Min X
	else if (new_adjuster_x <= this->panel_rect.x)
	{
		adjuster_rect.x = this->panel_rect.x;
		//SDL_WarpMouseInWindow(Window,adjuster_rect.x, pMouse->posy);
	}
	else
		adjuster_rect.x = new_adjuster_x;

	target_value = getTargetValue(adjuster_rect.x - panel_rect.x);
	DEBUGLOG("Adjuster\n\t");
	DEBUGLOG("[Mouse_X: %d] - [Mouse_X_logged: %d] = %d\n\t", mouse_x, mouse_x_logged, mouse_x-mouse_x_logged);
	DEBUGLOG("adjuster_x_logged: %d | new_adjuster_x: %d\n\t", adjuster_x_logged,adjuster_rect.x);
	DEBUGLOG("Pixel offset: %d. Target value: %f\n", adjuster_rect.x - panel_rect.x, target_value);

	if (action)
		action(&target_value);
}

// This is only here incase for some reason we need to calculation the ratio for a specific value
// otherwise we always base our calculation on the adjuster_X - panel_X offset.
template <class T>
T Slider<T>::getTargetValue(int adjuster_pixel_offset)
{
	return (T)(float)(adjuster_pixel_offset)*ratio;
}

template <class T>
int Slider<T>::getPixelValueFromTargetValue(T target_val)
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
template <class T>
T Slider<T>::getTargetValue()
{
	return (T)(float)((adjuster_rect.x - panel_rect.x)*ratio);
}

template <class T>
void Slider<T>::setTargetValue(T v)
{
	// V / ratio = Y; Y += panel_rect.x;
	adjuster_rect.x = (v / ratio) + panel_rect.x;
	target_value = v;
}

template <class T>
Slider<T>::Slider(int x, int y, 
	int panel_width, int panel_height, 
	int adjuster_width, int adjuster_height, 
	T range_min, T range_max,
	T default_value/*=0*/,
	int (*action)(void *data)/*=NULL*/,
	SDL_Color panel_color,
	SDL_Color value_color,
	SDL_Color adjuster_color) :
target_valueRange(range_min, range_max),
action(action),
colors({panel_color, value_color, adjuster_color})
//panel_x(x),panel_y(y),
//width(width), height(height)
{
	/*this->panel_x = x;
	this->panel_y = y;

	this->width = width;
	this->height = height;*/
	// maybe usefule (range_min+ ((range_max-range_min)/2) )
	panel_rect.x = x;
	panel_rect.y = y;
	panel_rect.w = panel_width;
	panel_rect.h = panel_height;

	// move to initializer list later
	adjuster_rect.x = x+(panel_width/2) - adjuster_width/2;	// Set the adjuster in the middle of the panel
	adjuster_rect.y = y - ((adjuster_height - panel_height)/2);			// guess
	adjuster_rect.w = adjuster_width;
	adjuster_rect.h = adjuster_height;

	adjuster_collision_rect.x = adjuster_rect.x;
	adjuster_collision_rect.y = adjuster_rect.y;
	adjuster_collision_rect.w = adjuster_rect.w;
	adjuster_collision_rect.h = adjuster_rect.h;
	adjuster_collision_rect.x -= 2;
	adjuster_collision_rect.w += 4;
	adjuster_collision_rect.y -= 2;
	adjuster_collision_rect.h += 6;

	target_numValuesInRange = float(target_valueRange.max - target_valueRange.min);
	slider_pixelRange = float(panel_width);

	//adjuster_rect = SetRect(adjuster_rect.x,adjuster_rect.y, 11, 16);

	// calculate ratio
	DoRatio();

	// 
	setTargetValue(default_value);
}

// x_offset added to the panel_x
template <class T>
void Slider<T>::SetAdjusterXOffset(int x_offset)
{
	adjuster_rect.x = x_offset+panel_rect.x;
}

// Put focus on the Slider<T>
// this just logs the current Adjuster_X and Mouse_X coordinate for proper subsequent adjustments to the slider
template <class T>
void Slider<T>::Activate(int mouse_x)
{
	adjuster_x_logged = adjuster_rect.x;
	mouse_x_logged = (int)floor(mouse_x);
	is_sliding=true;
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
template <class T>
void Slider<T>::DoRatio()
{	
	ratio = target_numValuesInRange / slider_pixelRange;
}

template <class T>
bool Slider<T>::receive_event(SDL_Event &ev)
{
	switch (ev.type)
	{
		case SDL_MOUSEMOTION:
			if (is_active())
			{
				Slide(mouse::x);
			}
		break;
		case SDL_MOUSEBUTTONDOWN:
			adjuster_collision_rect.x = adjuster_rect.x - 2;
			if (Utility::coord_is_in_rect(mouse::x, mouse::y, &adjuster_collision_rect))
			{
				if (!is_sliding)
					Activate(mouse::x);
				return true;
			}
		break;
		case SDL_MOUSEBUTTONUP:
			Deactivate();
			return true;
		break;

		default:break;
	}
	return false;
}

template <class T>
bool Slider<T>::is_active()
{
	return is_sliding;
}
template <class T>
void Slider<T>::Deactivate()
{
	is_sliding = false;
}
