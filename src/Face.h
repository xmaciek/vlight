#pragma once

#include "includeShortcut.h"
#include "Vertex.h"

class Face {
private:
  Vertex* arr[3];
  Vertex normal;
  
public:
  Face();
  Face(Vertex& A, Vertex& B, Vertex& C);
  Vertex& operator[](const uint& i);
  Face& setReference(Vertex& v, const uint& i);
  
  Face& genNormal();
  Vertex& getNormal();
  
  
  
  
  
};