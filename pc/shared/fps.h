#pragma once
#include "SDL.h"
/* SDL2 easy (imprecise) frame rate controller
 
 usage:
 1. Init: call set with your desired framerate
 
 2. at game loop beginning, call start()
 
 3. In event loop, ensure there is still frame time to check events,
 by using time()
 
 3. at the game loop end, call end()
 
 Created by Michael Bazzinotti on 5/23/20.
 Copyright © 2020 bazz. All rights reserved.
 
 
 /* TODO: implement high performance counter version */

class FPS
{
public:
    void set(int fps);
    void start();
    bool time();
    void end();
private:
    int fps;
    Uint32 frame_tick_timeout, frame_tick_duration;
};