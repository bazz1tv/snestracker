#include "app.h"
#include "Colors.h"
#include "Screen.h"

SDL_Window *sdlWindow;

int main(int argc, char **argv)
{ 
	App app(&sdlWindow, argc, argv, SCREEN_WIDTH, SCREEN_HEIGHT);
	app.run();
}

