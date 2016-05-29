#pragma once

#include "includeShortcut.h"

class Circle {
private:
  std::vector<double> x,y;
public:
  Circle(const uint& n=32, const double& r=1);
  double& getX(const uint& i);
  double& getY(const uint& i);
  uint size() const;

};