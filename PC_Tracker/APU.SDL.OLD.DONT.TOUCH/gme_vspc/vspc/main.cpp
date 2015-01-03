#include "app.h"
#include "colors.h"

SDL_Window *sdlWindow;

int main(int argc, char **argv)
{ 
	App app(&sdlWindow, argc, argv, 800, 600);
	app.run();
}

