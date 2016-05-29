#pragma once

#include "includeShortcut.h"
#include "Vertex.h"
#include "Quaternion.h"

class Aeroplane {
private:
  Vertex position;
  Quaternion orientation,qx,qy,qz;
  Vertex direction, velocity, upVector, rightVector;
  double speed;
  double lift;
  /* używane podczas sterowania */
  double crotX, crotY, crotZ;
  double rotX, rotY, rotZ;
  
  /* użyte jako tymczasowe wartości graniczne podczas sterowania */
  double rotXr, rotXl, rotYr, rotYl, rotZr, rotZl;
  /**/
  double topSpeed, minSpeed, nominalSpeed, acceleration;
  double accelI, accelD;
  bool isStalling;
  double gravityForce;
  Aeroplane& lerp(double& source, const double& target, const double& speed);
  uint smallPlane;
public:
  Aeroplane();
  Aeroplane& pitch(const bool& pressed, const bool& up);
  Aeroplane& yaw(const bool& pressed, const bool& left);
  Aeroplane& roll(const bool& pressed, const bool& left);
  Aeroplane& changeSpeed(const bool& pressed, const bool& inc);
  Aeroplane& update();
  Aeroplane& draw();
  double getAngle();
  Quaternion getOrientation();
  Vertex getPosition();
  Vertex getDirection();
  Vertex getVelocity();
  double getSpeed();
  double getTopSpeed();
  bool stall();
  double getLiftForce();
  Aeroplane& drawSmall(const double& X, const double& Y, const double& scale);
  Aeroplane& setUp();
};
