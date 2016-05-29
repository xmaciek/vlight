#include "Circle.h"

Circle::Circle(const uint& n, const double& r) {
  double angle = 360.0/n;
  for (double i=0; i<360.0; i+=angle) {   
    x.push_back(sin(i*CONSTANTS::deg2rad)*r);
    y.push_back(cos(i*CONSTANTS::deg2rad)*r);
  }

}

double& Circle::getX(const uint& i) { return x.at(i); }
double& Circle::getY(const uint& i) { return y.at(i); }

uint Circle::size() const { return x.size(); }