#pragma once

#include "includeShortcut.h"

class fpsMeter {
// private:
public:
  uint count;
  uint framesDone;
  uint lastUpdate;
  uint timeTotal, timeStart;
  double highest;
  double FPS;
  double average;
  double top;
  
public:  
  fpsMeter();
  fpsMeter& beginFrame();
  fpsMeter& endFrame();
  fpsMeter& clear();
  fpsMeter& update();
  
  
};
