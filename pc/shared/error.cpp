#include "SDL.h"
#include "Render.h"
#include "gui/DialogBox.h"

void handle_error( const char* error )
{
  if ( error )
  {
    // put error in window title
    char str [256];
    sprintf( str, "Error: %s", error );
    fprintf( stderr, "Error: %s", error );
    
    DialogBox::SimpleOK("Critical Error", str, DialogBox::Simple::ERR);
    exit( EXIT_FAILURE );
  }
}
