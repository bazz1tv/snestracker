#pragma once
struct Experience
{
  virtual void run()=0;
  virtual void draw()=0;
  virtual void receive_event(SDL_Event &ev)=0;

};