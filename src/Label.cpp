#include "Label.h"

const uchar Label::alignLeft = 0;
const uchar Label::alignCenter = 1;
const uchar Label::alignRight = 2;
const uchar Label::alignBottom = 3;
const uchar Label::alignMiddle = 4;
const uchar Label::alignTop = 5; 


Label& Label::clear() {
  text = std::string();
  return *this;
}

Label::Label(const std::string& str) {
//   font = Font::getFont();
  isVisible = true;
  isBlinking = false;
  blinkState = 0;
  blink = true;
  blinkInterval = 1;
  xAlign = Label::alignLeft;
  yAlign = Label::alignBottom;
  xOffset = yOffset = 0;
  x = 4;
  y = 0;
  text = str;
}

Label::~Label() {
//   if (font!=NULL) { delete font; font = NULL; }
}

Label& Label::setText(const std::string& str) {
  text = str;
  recalcOffset();
  return *this;  
}

std::string Label::getText() {
  return text;
}

//   Label& operator<<(const std::string& str);
//   Label& operator<<(const double& d);
//   Label& operator<<(const int& d);
//   Label& operator<<(const uint& d);
//   Label& operator<<(const char& c);

//   Label& setFont(const uint& f);
//   Label& move(const double& X, const double& Y);
//   Label& show();
//   Lavel& hide();

Label& Label::draw() {
  if (!isVisible) { return *this; }
  if (isBlinking && !blink) { return *this; }
  SHADER::pushMatrix();
    SHADER::translate(x-xOffset,y-yOffset,0);
//     glScaled(0.14,0.14,0.14);
    Font::getFont()->print(text);
  SHADER::popMatrix();
  return *this;
}

Label& Label::move(const double& X, const double& Y) {
  x = X; y = Y; return *this;  
}

Label& Label::recalcOffset() {
//   std::cout<<(Font::getFont()==NULL);
    switch (xAlign) {
    case Label::alignLeft: xOffset = 0; break;
    case Label::alignCenter: xOffset = (uint)Font::getFont()->textWidth(text)/2; break;
    case Label::alignRight: xOffset = Font::getFont()->textWidth(text); break;
    default: break;
  }
  switch (yAlign) {
    default: break;
  }
  return *this;
}

Label& Label::setAlign(const uchar& horizontal, const uchar& vertical) {
  xAlign = horizontal;
  yAlign = vertical;
  recalcOffset();
  return *this;  
}
  
Label& Label::setBlink(const bool& b, const double& i) {
  blinkInterval = i;
  isBlinking = b;
  return *this;
}
  
Label& Label::update() {
  if (isBlinking) {
    blinkState += CONSTANTS::DELTATIME;
    if (blinkState>=blinkInterval) {
      blinkState-=blinkInterval;
      blink = !blink;
    }
  }
  return *this;
}

Label& Label::setVisible(const bool& b) { isVisible = b; return *this; }

bool Label::getVisible() { return isVisible; }
bool Label::getBlinkState() { return blink; }

double Label::bottom() { return y-16; }
