#include "Font.h"

Font* Font::f = NULL;
Font* Font::getFont() {
  init();
  return f;
}

void Font::init() {
  static bool lock = false;
  if (lock) { return; }
  lock = true;
  f = new Font();
  f->load("x");
}

Font::Font() {
//   displayList = glGenLists(128);
}

Font::Font(const std::string &str) {
//   displayList = glGenLists(128);
//   load(str);
}

void Font::load(const std::string &str) {
  for (uint i=32; i<128; i++) {
    std::stringstream ss;
    ss<<i;
    std::string number;
    ss>>number;
    chars[i].load(std::string("fonts/")+number+std::string(".tga"));
    if (chars[i].ID==0) { continue; }
    double tmpSquare[18];
    memset(tmpSquare, 0, sizeof(double)*18);
    tmpSquare[3] = chars[i].owidth;
    tmpSquare[7] = chars[i].oheight;
    tmpSquare[10] = chars[i].oheight;
    tmpSquare[12] = chars[i].owidth;
    tmpSquare[15] = chars[i].owidth;
    tmpSquare[16] = chars[i].oheight;
    squares[i] = SHADER::makeBuffer(tmpSquare, sizeof(double)*18);
//     std::cout<<"square id: "<<squares[i]<<"\n";
    double tmpSquareUV[12];
    memset(tmpSquareUV,0,sizeof(double)*12);
    tmpSquareUV[2] = (double)chars[i].owidth/chars[i].width;
    tmpSquareUV[5] = (double)chars[i].oheight/chars[i].height;
    tmpSquareUV[7] = (double)chars[i].oheight/chars[i].height;
    tmpSquareUV[8] = (double)chars[i].owidth/chars[i].width;    
    tmpSquareUV[10] = (double)chars[i].owidth/chars[i].width;
    tmpSquareUV[11] = (double)chars[i].oheight/chars[i].height; 
    squaresUV[i] = SHADER::makeBuffer(tmpSquareUV, sizeof(double)*12);  
//     std::cout<<"squareUV id: "<<squaresUV[i]<<"\n";
  }
}

void Font::erase() {
  for (uint i=0; i<128; i++) {
    chars[i].erase();
  }  
}

uint Font::print(const std::string &str) {
  SHADER::pushMatrix();
  for (uint i=0; i<str.size(); i++) {
    uint charID = str[i];
    chars[charID].use();
    SHADER::texCoord(squaresUV[charID]);
//     std::cout<<squaresUV[charID]<<" - "<<squares[charID]<<"\n";
    SHADER::draw(GL_TRIANGLES, squares[charID], 6);
    SHADER::translate(chars[charID].owidth,0,0);
  }
  SHADER::popMatrix();
  return 0;
}

double Font::textWidth(const std::string &str) {
  double w = 0;
  for (uint i=0; i<str.size(); i++) {
    w+=chars[(uint)str[i]].owidth;
  }
  return w; 
}


