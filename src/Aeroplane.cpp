#include "Aeroplane.h"

Aeroplane::Aeroplane() {
  position = Vertex(0,5,0);
  speed = 1.2;

  direction = Vertex(0,0,-1);
  upVector = Vertex(0,1,0);

  velocity = direction * speed;
  rotXr = rotXl = rotYr = rotYl = rotZr = rotZl = 0;
  rotX = rotY = rotZ = crotX = crotY = crotZ = 0;
  topSpeed = 5; minSpeed = 0.1; nominalSpeed = 1.2; acceleration = 0;
  accelI = accelD = 0;
  
  gravityForce = pow(9.84,0.5);
  isStalling = false;
}

Aeroplane& Aeroplane::pitch(const bool& pressed, const bool& up) {
  if (pressed) {
    if (up) { 
      rotXl = CONSTANTS::DELTATIME*10; }
    else { 
      rotXr = -CONSTANTS::DELTATIME*10; }
  }
  else {
    if (up) {
      rotXl = 0;}
    else {
      rotXr = 0;}
  }
  
  rotX = rotXl + rotXr;
  return *this;
}

Aeroplane& Aeroplane::yaw(const bool& pressed, const bool& left) {
  if (pressed) {
    if (left) { rotYl = CONSTANTS::DELTATIME*5; }
    else { rotYr = -CONSTANTS::DELTATIME*5; }
  }
  else {
    if (left) {rotYl = 0;}
    else {rotYr = 0;}
  }
  
  rotY = rotYl + rotYr;
  return *this;
  
}

Aeroplane& Aeroplane::roll(const bool& pressed, const bool& left) {
  
  if (pressed) {
    if (left) { rotZl = CONSTANTS::DELTATIME*20; }
    else { rotZr = -CONSTANTS::DELTATIME*20; }
  }
  else {
    if (left) {rotZl = 0;}
    else {rotZr = 0;}
  }
  
  rotZ = rotZl+rotZr;
  return *this;
}

Aeroplane& Aeroplane::changeSpeed(const bool& pressed, const bool& inc) {
  if (pressed) {
    if (inc) { accelI = 0.004; }
    else { accelD = -0.004; }
  }  
  else {
    if (inc) { accelI = 0; }
    else { accelD = 0; }
  }
  return *this;
}

Aeroplane& Aeroplane::update() {
  acceleration = accelD + accelI;
  if ((speed>topSpeed && acceleration>0 ) || (speed<minSpeed && acceleration<0)) { acceleration = 0; }
  
  speed += acceleration;
  
  lerp(crotX,rotX,CONSTANTS::DELTATIME/3);
  lerp(crotY,rotY,CONSTANTS::DELTATIME/4);
  lerp(crotZ,rotZ,CONSTANTS::DELTATIME/2);
  qx = Quaternion(crotX,1,0,0);
  qy = Quaternion(crotY,0,1,0);
  qz = Quaternion(crotZ,0,0,1);
  
  Quaternion qtmp = orientation;
  qtmp = qy * qtmp;
  qtmp = qx * qtmp;
  qtmp = qz * qtmp;
  orientation = qtmp;
  
  Vertex tmp(0,0,-1);
  orientation.rotateVector(tmp);
  direction = tmp;

  tmp = Vertex(0,1,0);
  orientation.rotateVector(tmp);
  upVector = tmp;

  
  
  
  double ang = upVector.angleTo(Vertex(0,-1,0));
  lift = velocity.length()*(((ang)+120)/300.0);
  Vertex grav(0,-gravityForce * (1.0-std::min(1.0,lift)),0);
  grav[1] *= 0.25;
  velocity = (direction * speed) + grav;

  
  if (grav[1]<1.0) {
    isStalling = true;
    ang = direction.angleTo(Vertex(0,1,0));
    ang = std::min(10.0,ang)*CONSTANTS::DELTATIME;
    orientation = orientation.lookAt(Vertex(0,1,0), grav[1]*ang*4);
  }
  else { isStalling = false; }
  
  
  position += velocity*CONSTANTS::DELTATIME;  
  return *this;
}

Aeroplane& Aeroplane::draw() {
 
  
  return *this;
}

Quaternion Aeroplane::getOrientation() {
  return orientation;
}

Vertex Aeroplane::getPosition() {
  return position;
}

Vertex Aeroplane::getDirection() { return direction; }

Vertex Aeroplane::getVelocity() { return velocity; }

double Aeroplane::getSpeed() { return speed; } 

bool Aeroplane::stall() { return isStalling; }

double Aeroplane::getLiftForce() { return lift; }

double Aeroplane::getTopSpeed() {
  return topSpeed;
}

Aeroplane& Aeroplane::lerp(double& source, const double& target, const double& spd) {
  double distance = target - source;
  double d = distance;
  if (d<0) { d=-d; }
  if (d<spd) { source = target; return *this; }
  d = distance / d;
  source += d*spd;
  
  
  return *this;
}

double Aeroplane::getAngle() {
  Vertex tmpDir = direction;
  tmpDir[1] = -tmpDir[2];
  tmpDir[2] = 0;
  tmpDir.normalize();
  double tmpAngle = tmpDir.angleTo(Vertex(0,1,0));
  if (tmpDir.angleTo(Vertex(-1,0,0))>90) { tmpAngle = 360.0 - tmpAngle; }
  return tmpAngle;
}

Aeroplane& Aeroplane::setUp() {
  double* tmp = new double[12];
  memset(tmp,0,sizeof(double)*12);
  tmp[1]=5;
  tmp[3]=tmp[4]=-5;
  tmp[7]=-2;
  tmp[9]=5;
  tmp[10]=-5;
  smallPlane = SHADER::makeBuffer(tmp, sizeof(double)*12);
  delete[] tmp;
  return *this;
}

Aeroplane& Aeroplane::drawSmall(const double& X, const double& Y, const double& scale) {
  SHADER::pushMatrix();  
  SHADER::translate(X+position[0]*scale,Y-position[2]*scale,0);

  SHADER::rotate(getAngle(),0,0,1);
  SHADER::draw(GL_LINE_LOOP, smallPlane, 4); 
  SHADER::popMatrix();
  return *this;
}

