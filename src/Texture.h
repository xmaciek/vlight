#pragma once

#include "includeShortcut.h"

class Texture { 
  uint pow2(const uint& a);
public:
  
  Texture();
  uint width, height;
  uint owidth, oheight;
  bool resized;
  char bpp; 
  uint ID;
  uint type; 
  std::string fileLocation;
  
  void use();
  void erase();
  void load(const std::string &src);
  void fromData(uchar* data, const uint& w, const uint& h, const uint& b, const uint& t);
  
  bool operator<(const std::string& txt) const;
  bool operator>(const std::string& txt) const;
  bool operator==(const std::string& txt) const;
};