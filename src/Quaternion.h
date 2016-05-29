#pragma once

#include "includeShortcut.h"
#include "Vertex.h"

class Quaternion {
public:
  double x,y,z,w;
  
public:
  Quaternion();
  Quaternion(const Vertex &v, const double &w);
  Quaternion(const double &angle, const double &X, const double &Y, const double &Z);
  Quaternion(const Vertex &dir, const Vertex &up);
  Quaternion& conjugate();
  Quaternion& inverse();
  Quaternion& normalize();
  Quaternion& createMatrix(double *mat) const;
  Quaternion& multMatrix();
  Quaternion& createFromAngles(const double& angle, const double &X, const double &Y, const double &Z);
  Quaternion& rotateVector(Vertex &v);
  Vertex getVector() const;
  Vertex getUpVector() const;
  Vertex getRightVector() const;
  Quaternion& rotateLocal(const Quaternion& q);
  Quaternion& rotateLocal(const double &angle, const uchar &X, const uchar &Y, const uchar &Z);
  Quaternion& rotateGlobal(const double &angle, const uchar &X, const uchar &Y, const uchar &Z);
  Quaternion operator*(const Quaternion &Q) const;
  Quaternion& operator*=(const Quaternion &Q);
  Quaternion operator*(const double& scale) const;
  Quaternion operator-() const;
  Quaternion operator+(const Quaternion& q) const;
  Quaternion& operator+=(const Quaternion& q);
  Quaternion operator-(const Quaternion& q) const;
  Quaternion slerp(const Quaternion& where, const double& dt);
  Quaternion lerp(const Quaternion& where, const double& dt);
  Quaternion lookAt(const Vertex& v, const double& speed);
};




