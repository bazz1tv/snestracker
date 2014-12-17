#include "Slider.h"

void Slider::Draw()
{
	// Get Graphics going
	// void FillRect(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);
	// void FillRectAlpha(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);
	
	FillRectAlpha(Renderer, panel_x, panel_y, width, 10,50,50,50,255); // 50,50,50));
	FillRectAlpha(Renderer, adjuster_x,adjuster_y, 8, 15, 0, 255, 0, 255); // SDL_MapRGB(Screen->format,0,255,0));
}

void Slider::SetAdjusterPos(int x)
{
	adjuster_x = x;
}

void Slider::Slide(int x)
{
	x = adjuster_x_logged+(x-mouse_x_logged);
	// check for MAX X
	if (x > panel_x+width)
		adjuster_x = panel_x+width;
	// check for under Min X
	else if (x < this->panel_x)
		adjuster_x = this->panel_x;
	else
		adjuster_x = x;

	adjuster_rect.x = adjuster_x;

	
	printf("Adjust value: %d. Applied to Ratio: %d\n", adjuster_x - panel_x, doCalculation(adjuster_x - panel_x));
}

int Slider::doCalculation(int adjuster_value)
{
	return (int)floor((float)adjuster_value*ratio);
}

int Slider::doInverseCalculation(int val)
{
	return (int)floor((float)val*(1/ratio));
}
int Slider::doCalculation()
{
	return (int)floor((float)(adjuster_x - panel_x)*ratio);
}

Slider::Slider(int x, int y, int width, int range_min, int range_max)
{
	this->panel_x = x;
	this->panel_y = y;

	this->width = width;

	adjuster_x = x+(width/2);	// Set the adjuster in the middle of the panel
	adjuster_y = y-2;			// guess

	target_range = float(range_max - range_min);
	slider_range = float(width);

	adjuster_rect = SetRect(adjuster_x,adjuster_y, 8, 15);

	// calculate ratio
	DoRatio();
}

void Slider::SetAdjusterX(int x)
{
	adjuster_x = x+panel_x;
	adjuster_rect.x = x+panel_x;
}

void Slider::Activate()
{
	adjuster_x_logged = adjuster_x;
	mouse_x_logged = (int)floor(pMouse->posx);
}

void Slider::DoRatio()
{
	if (target_range > slider_range)
		 ratio = slider_range / target_range;
	else ratio = target_range / slider_range;
}
