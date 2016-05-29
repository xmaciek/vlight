#pragma once

#include "includeShortcut.h"
#include "Font.h"

class Label {
public:
  static const uchar alignLeft;
  static const uchar alignCenter;
  static const uchar alignRight;
  static const uchar alignBottom;
  static const uchar alignMiddle;
  static const uchar alignTop; 
  
private:
  double x,y;
  bool isVisible;
  bool isBlinking;
  bool blink;
  double blinkState;
  double blinkInterval;
  
  uchar xAlign, yAlign;
  double xOffset, yOffset;
  
  Font* font;
  std::string text;
  Label& recalcOffset();
  
public:
  
//   static const uint FontSmall=0;
//   static const uint FontMedium=1;
//   static const uint FontLarge=2;
  
  Label(const std::string& str="");
  ~Label();
  Label& setText(const std::string& str);
  std::string getText();
  Label& clear();
  
  template<class T> 
  Label& operator<<(const T& param) {
    std::stringstream xx;
    std::string t;
    xx<<param;
    std::getline(xx,t);
    text += t;
    recalcOffset();
    return *this;
  }
  
  Label& setAlign(const uchar& horizontal, const uchar& vertical=Label::alignBottom);
  
  Label& setFont(const uint& f);
  Label& move(const double& X, const double& Y);
  Label& show();
  Label& hide();
  Label& setVisible(const bool& b);
  bool getVisible();
  
  double bottom();
  
  Label& setBlink(const bool& b, const double& i=1.0);
  bool getBlinkState();
  
  Label& draw();
  Label& update();


};
