#include "Vertex.h"

/*radian->stopien*/
const double rad2deg = 180.0/(atan(1)*4.0);

Vertex::Vertex() {
  x = 0.0;
  y = 0.0;
  z = 0.0;
}
  
Vertex::Vertex(const double &X, const double &Y, const double &Z) {
  x = X;
  y = Y;
  z = Z;
}
  
Vertex::Vertex(const double* ARR) {
  x = ARR[0];
  y = ARR[1];
  z = ARR[2];
}  
  
  
double& Vertex::operator[](const uint& i) {
  switch(i) {
    case 0: return x; break;
    case 1: return y; break;
    case 2: return z; break;
    default: return x; break;
  }
}
  
  
Vertex Vertex::operator-() const {
  return Vertex(-x,-y,-z);
};
  
Vertex Vertex::operator-(const Vertex &v) const {
  return Vertex(x-v.x, y-v.y, z-v.z); 
};
  
Vertex Vertex::operator+(const Vertex &v) const {
  return Vertex(x+v.x, y+v.y, z+v.z); 
};
  
Vertex& Vertex::operator+=(const Vertex &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
};  

Vertex& Vertex::operator-=(const Vertex &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
};
  
Vertex Vertex::operator/(const double &d) const {
  return Vertex(x/d, y/d, z/d); 
};
    
Vertex Vertex::operator*(const double &d) const {
  return Vertex(x*d, y*d, z*d); 
};
  
Vertex& Vertex::operator/=(const double &d) {
  x/=d; y/=d; z/=d;
  return *this;
};
    
Vertex& Vertex::operator*=(const double &d) {
  x*=d; y*=d; z*=d;
  return *this;
};
  
  
Vertex Vertex::operator*(const Vertex &v) const {
  return Vertex((y*v.z) - (z*v.y), (z*v.x) - (x*v.z), (x*v.y) - (y*v.x));
};

  
  
  
bool Vertex::operator==(const Vertex &v) const {
  return (x==v.x && y==v.y && z==v.z); };
  
bool Vertex::operator<(const Vertex &v) const {
  if (x<v.x) { return true; }
  if (y<v.y) { return true; }
  if (z<v.z) { return true; }
  return false;
};
  
bool Vertex::operator>(const Vertex &v) const {
  if (x>v.x) { return true; }
  if (y>v.y) { return true; }
  if (z>v.z) { return true; }
  return false;
};
  
  
  
  
void Vertex::use() const {
//   glVertex3d(x,y,z);  
} 

void Vertex::translate() const {
  SHADER::translate(x,y,z);
}

double const& Vertex::getX() const { return x; }
double const& Vertex::getY() const { return y; }
double const& Vertex::getZ() const { return z; }



double Vertex::length() const {
  return sqrt((x*x)+(y*y)+(z*z));
}

double Vertex::distanceTo(const Vertex &v) const {
  return sqrt(pow(x-v.x,2) + pow(y-v.y,2) + pow(z-v.z,2));
}

Vertex& Vertex::normalize() {
  double len = length();
  if (len==0.0) { len = 1.0; }
  x /= len;
  y /= len;
  z /= len;
//   std::cout<<len<<" "<<x<<" "<<y<<" "<<z<<std::endl;
//   printf("%f %f %f %f\n",len,x,y,z);
  return *this;
}
  
double Vertex::dotTo(const Vertex &v) const {
  return (x*v.x)+(y*v.y)+(z*v.z);
}
  


double Vertex::angleTo(Vertex two) const {
  Vertex one(*this);
  one.normalize();
  two.normalize();
  return acos(one.dotTo(two))*rad2deg;
}


Vertex Vertex::cross(const Vertex &a, const Vertex &b) {
  return a*b;
}

double Vertex::dot(const Vertex &a, const Vertex &b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}


Vertex Vertex::normals(const Vertex &a, const Vertex &b, const Vertex &c) {
//   Vertex v1 = b-a;
//   Vertex v2 = c-a;
  return ((b-a)*(c-a)).normalize();
}


double Vertex::distanceToTriangle(const Vertex &a, const Vertex &b, const Vertex &c, const Vertex &p) {
  Vertex v(p-a);
  Vertex n(Vertex::normals(a,b,c));
  v -= n * Vertex::dot(p,n);
  return v.length();
  }
