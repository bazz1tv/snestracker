#include "fps.h"
void FPS::set(int fps)
{
    this->fps = fps;
    // calc from framerate. could put this into dynamic function later
    frame_tick_duration = 1000 / fps; // how many ms per frame
}

void FPS::start()
{
    frame_tick_timeout = SDL_GetTicks() + frame_tick_duration;
}

bool FPS::time()
{
    return !SDL_TICKS_PASSED(SDL_GetTicks(), frame_tick_timeout);
}

void FPS::end()
{
    // If we finished the frame early, sleep until the next frame start
    Uint32 curticks = SDL_GetTicks();
    if (!SDL_TICKS_PASSED(curticks, frame_tick_timeout))
    {
        SDL_Delay(frame_tick_timeout - curticks);
        //DEBUGLOG("duration: %d, remaining: %d\n", frame_tick_duration,
        //frame_tick_timeout - curticks);
    }
}