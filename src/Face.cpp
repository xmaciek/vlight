#include "Face.h"
 
Face::Face() {
  arr[0] = arr[1] = arr[2] = NULL;
}  

Face::Face(Vertex& A, Vertex& B, Vertex& C) {
  arr[0] = &A; arr[1] = &B; arr[2] = &C;
}

Vertex& Face::operator[](const uint& i) {
  return *arr[i];
}

Face& Face::setReference(Vertex& v, const uint& i) {
  arr[i] = &v;
  return *this;
}

Face& Face::genNormal() {
  normal = Vertex::normals(*arr[0], *arr[1], *arr[2]);
  return *this;
}

Vertex& Face::getNormal() {
  return normal;
}