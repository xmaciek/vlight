#pragma once

#include "includeShortcut.h"
#include "Texture.h"
#include "Quaternion.h"

class Cloud {

private:
  Vertex postion;
  Texture texture;
  Quaternion orientation;
  uint square, squareUV;

public:
  Cloud& lookAt(const Vertex& v);
  Cloud& draw();
  Cloud& setUp();

};
