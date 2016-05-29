#include "Quaternion.h"


Quaternion::Quaternion() {
  x = y = z = 0;
  w = 1;
}

Quaternion::Quaternion(const Vertex& v, const double& W) {
  x = v.getX();
  y = v.getY();
  z = v.getZ();
  w = W;
}



Quaternion::Quaternion(const double& angle, const double& X, const double& Y, const double& Z) {
  double rad = angle*CONSTANTS::deg2rad*0.5;
  double result = sin(rad);
  w = cos(rad);
  x = result * X;
  y = result * Y;
  z = result * Z;
  normalize();
}

Quaternion::Quaternion(const Vertex &dir, const Vertex& up) {
  Vertex tmp = dir*up;
  x=tmp.x;
  y=tmp.y;
  z=tmp.z;
  w = sqrt(pow(dir.length(),2) * pow(up.length(),2)) + Vertex::dot(dir, up);
  normalize();
}



Quaternion Quaternion::operator*(const Quaternion& Q) const {
  Quaternion R;
  R.w = w*Q.w - x*Q.x - y*Q.y - z*Q.z;
  R.x = w*Q.x + x*Q.w + y*Q.z - z*Q.y;
  R.y = w*Q.y + y*Q.w + z*Q.x - x*Q.z;
  R.z = w*Q.z + z*Q.w + x*Q.y - y*Q.x;
  return R.normalize();
}

Quaternion& Quaternion::operator*=(const Quaternion& q) {
  return *this = *this*q;
}

Quaternion& Quaternion::multMatrix(){

  float matrix[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
  matrix[0] = 1.0 - 2.0 * (y*y + z*z);
  matrix[1] = 2.0 * (x*y - w*z);
  matrix[2] = 2.0 * (x*z + w*y);
//   matrix[3] = 0;
  
  matrix[4] = 2.0 * (x*y + w*z);
  matrix[5] = 1.0 - 2.0 * (x*x + z*z);
  matrix[6] = 2.0 * (y*z - w*x);
//   matrix[7] = 0;  
    
  matrix[8] = 2.0 * (x*z - w*y);  
  matrix[9] = 2.0 * (y*z + w*x);  
  matrix[10] = 1.0 - 2.0 * (x*x + y*y);  
//   matrix[11] = 0;  
    
//   matrix[12] = 0;  
//   matrix[13] = 0;  
//   matrix[14] = 0;  
//   matrix[15] = 1.0;
  SHADER::multMatrix(matrix);
  return *this;
}

Quaternion& Quaternion::inverse()  {
  x = -x;
  y = -y;
  z = -z;
  w = -w;
  return *this;
}

Quaternion& Quaternion::conjugate() {
  x = -x;
  y = -y;
  z = -z;
  return *this;
}

Quaternion& Quaternion::normalize() {
  double len = sqrt(w*w + x*x + y*y + z*z);
  if (len==0) { len = 1; }
  w/=len;
  x/=len;
  y/=len;
  z/=len;
  return *this;
}

Vertex Quaternion::getVector() const {
  return Vertex(2.0*(x*z-w*y), 2.0*(y*z+w*x), 1.0-2.0*(x*x+y*y)).normalize();
}

Vertex Quaternion::getUpVector() const {
  return Vertex(2.0*(x*y-w*z), 1.0-2.0*(x*x+z*z), 2.0*(y*z+w*x)).normalize();
}

Vertex Quaternion::getRightVector() const {
  return Vertex(1.0-2.0*(y*y+z*z), 2.0*(x*y+w*z), 2.0*(x*z-w*y)).normalize();
}

Quaternion& Quaternion::rotateVector(Vertex &v) {
//   Quaternion V(v, 0);
//   Quaternion con(*this);
//   con.conjugate();
//   Quaternion result = (*this * V * con);
//   result.normalize();
// //   v = result.getVector();
//   v = Vertex(result.x, result.y, result.z);
    Quaternion V(v, 0);
//   V.Normalise();
  Quaternion con(*this);
  con.conjugate();
  Quaternion result = (con * V * *this);
  v[0]=result.x;
  v[1]=result.y;
  v[2]=result.z;
  
  
  
  return *this;
}

Quaternion& Quaternion::rotateLocal(const Quaternion& q) {
  *this = q * *this;
  return *this;  
}

Quaternion& Quaternion::rotateLocal(const double &angle, const uchar &X, const uchar &Y, const uchar &Z) {
  *this = Quaternion(angle,X,Y,Z) * *this;
  return *this;
}

Quaternion& Quaternion::rotateGlobal(const double &angle, const uchar &X, const uchar &Y, const uchar &Z) {
  *this *= Quaternion(angle,X,Y,Z);
  return *this;
}

Quaternion Quaternion::operator*(const double& scale) const {
  return Quaternion(x*scale, y*scale, z*scale, w*scale);
}

Quaternion Quaternion::operator-() const {
  return Quaternion(-x,-y,-z,-w);
}

Quaternion Quaternion::operator+(const Quaternion& q) const { 
  return Quaternion(x+q.x, y+q.y, z+q.z, w+q.w); 
}

Quaternion& Quaternion::operator+=(const Quaternion& q) {
  x+=q.x; y+=q.y; z+=q.z; w+=q.w;
  return *this;
}


Quaternion Quaternion::operator-(const Quaternion& q) const { 
  return Quaternion(x-q.x, y-q.y, z-q.z, w-q.w); 
}

Quaternion Quaternion::lerp(const Quaternion& q, const double& dt) {
  return (*this*(1.0-dt) + q*dt).normalize();
}

Quaternion Quaternion::slerp(const Quaternion& where, const double& dt) {
  Quaternion tmp;
  double dot = x*where.x + y*where.y + z*where.z + w*where.w;
  if (dot<0) {
    dot = -dot;
    tmp = -where;
    } else { tmp = where; }
    
  if (dot<0.95) {
    double angle = acos(dot);
    return (*this*sin(angle*(1.0-dt)) + tmp*((sin(angle*dt))/sin(angle)));
    } else { return lerp(tmp,dt); }
}  

Quaternion Quaternion::lookAt(const Vertex& target, const double& speed) {
  Vertex position(0,0,0);
  Vertex forwardVector(target - position);
  forwardVector.normalize();
  Vertex rotAxis(getVector()*forwardVector);
  rotAxis.normalize();
  double dot = acos(Vertex::dot(getVector(), forwardVector))*speed;
//   std::cout<<rotAxis[0]<<" "<<rotAxis[1]<<" "<<rotAxis[2]<<std::endl;
  Quaternion q(dot, rotAxis[0], rotAxis[1], rotAxis[2]);
  return *this * q;
  
}

