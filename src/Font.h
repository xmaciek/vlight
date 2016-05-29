#pragma once

#include "includeShortcut.h"
#include "Texture.h"

class Font {
public:
  
  Font();
  Font(const std::string &str);
  
  void load(const std::string &str);
  void erase();
  uint print(const std::string &str);
  double textWidth(const std::string &str);
  
  static Font* getFont();
  static void init();
private:
  static Font* f;
  bool created;
  Texture chars[128];
  uint squares[128];
  uint squaresUV[128];
//   uint widths[128];
//   double percWidths[128];
};