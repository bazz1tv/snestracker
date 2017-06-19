#include "SDL.h"
#include "Render_Context.h"
//extern SDL_Window *sdlWindow;
void handle_error( const char* error )
{
  if ( error )
  {
    // put error in window title
    char str [256];
    sprintf( str, "Error: %s", error );
    fprintf( stderr, "Error: %s", error );
    SDL_SetWindowTitle(Render_Context::sdlWindow, str);
    
    // wait for keyboard or mouse activity
    SDL_Event e;
    do
    {
      while ( !SDL_PollEvent( &e ) ) { }
    }
    while ( e.type != SDL_QUIT && e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN );

    exit( EXIT_FAILURE );
  }
}