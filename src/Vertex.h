#pragma once

// #include <cstdlib>
#include <cmath>

#include "includeShortcut.h"



class Vertex {
private:
  
public:
  
//   double arr[3];
  double x,y,z;
  double& operator[](const uint& i);
  Vertex();
  Vertex(const double &X, const double &Y, const double &Z);
  Vertex(const double* ARR);
  void use() const;
  void translate() const;
  
  double const& getX() const;
  double const& getY() const;
  double const& getZ() const;
  
  
  double length() const;
  double distanceTo(const Vertex &v) const;
  double dotTo(const Vertex &v) const;
  double angleTo(Vertex two) const;
  
  Vertex& normalize();

  
  Vertex operator-() const;
  Vertex operator-(const Vertex &v) const;
  Vertex operator+(const Vertex &v) const;
  
  Vertex& operator+=(const Vertex &v);  
  Vertex& operator-=(const Vertex &v);
  
  /* --- scalar operators --- */
  Vertex operator/(const double &d) const;
  Vertex operator*(const double &d) const;
 
  Vertex& operator/=(const double &d);
  Vertex& operator*=(const double &d);
  /* --- --- */  
  
  
  /* --- cross product operator --- */
  Vertex operator*(const Vertex &v) const;
  /* --- --- */  
  
  bool operator==(const Vertex &v) const;
  bool operator<(const Vertex &v) const;
  bool operator>(const Vertex &v) const;
  
  
  
  static double dot(const Vertex &a, const Vertex &b);
  static Vertex cross(const Vertex &a, const Vertex &b);
  static Vertex normals(const Vertex &a, const Vertex &b, const Vertex &c);
  static double distanceToTriangle(const Vertex &a, const Vertex &b, const Vertex &c, const Vertex &p);
};