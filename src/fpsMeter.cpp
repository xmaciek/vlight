#include "fpsMeter.h"

fpsMeter::fpsMeter() {
  timeTotal = framesDone = count = 0;
  lastUpdate = 0;
  highest = 0;
  top = 0;
}

fpsMeter& fpsMeter::beginFrame() {
  timeStart = SDL_GetTicks();
  return *this;
}


fpsMeter& fpsMeter::endFrame() {
  framesDone++;
  uint tmp = SDL_GetTicks() - timeStart;
  timeTotal += tmp;
  if (tmp>highest) { highest = tmp; }
  return *this;
}



fpsMeter& fpsMeter::update() {
  uint x = SDL_GetTicks();
  if (x-lastUpdate<1000) { return *this; }
  lastUpdate = x;
  count = framesDone;
  framesDone = 0;
  top = highest;
  highest = 0;
  average = (double)timeTotal/count;
  FPS = 1000.0 / average;
  timeTotal = 0;
  return *this;
}

