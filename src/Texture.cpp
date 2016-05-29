#include "Texture.h"

Texture::Texture() {
  width = height = type = bpp = ID = 0;
  resized = false;
}
 
 
void Texture::use() {
//   glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::erase() {
  glDeleteTextures(1, &ID);
  width = height = type = bpp = ID = 0;
}

void Texture::fromData(uchar* data, const uint& w, const uint& h, const uint& b, const uint& t) {
  fileLocation = std::string();
  width = w;
  owidth = w;
  height = h;
  oheight = h;
  width = pow2(width);
  height = pow2(height);
  bpp = b;
  if (oheight!=height || owidth!=width) {
    resized = true;
    uchar* newData = new uchar[width*height*bpp];
    uchar* oldData = data;
    memset(newData, 0, width*height*bpp);
    for (uint i=0; i<oheight; i++) {
      memcpy(newData+(i*width*bpp), oldData+(i*owidth*bpp), bpp*owidth);
    }    
    data = newData;
    delete[] oldData;
  }
  
  type = t;
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);  
  glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
  delete[] data;
}

void Texture::load(const std::string &src) {
  fileLocation = src;
  std::fstream file(src.c_str(), std::fstream::in | std::fstream::binary);
  if (!file.is_open()) { std::cout<<"No such file: "<<src<<std::endl; return; }
  
  uchar header[12];
  file.read((char*)header, 12);
  file.read((char*)&width, 2);
  file.read((char*)&height, 2);
  file.read((char*)&bpp, 2);
  
  bpp /= 8;
  if (bpp==3) { type = GL_RGB; }
  else { type = GL_RGBA; }
  
  uint size = width*height*bpp;
  uchar* loadedData = new uchar[size];
  file.read((char*)loadedData, size);
  file.close();
  
  uchar swap;
  for (uint i=0; i<size; i+=bpp) {
    swap = loadedData[i+2];
    loadedData[i+2] = loadedData[i];
    loadedData[i] = swap;
  }
  
//   owidth = width;
//   oheight = height;
//   width = pow2(width);
//   height = pow2(height);
//   if (height!=oheight || width!=owidth) {
//     resized = true;
//     uchar* newData = new uchar[width*height*bpp];
//     uchar* oldData = loadedData;
//     memset(newData, 0, width*height*bpp);
//     for (uint i=0; i<oheight; i++) {
//       memcpy(newData+(i*width*bpp), oldData+(i*owidth*bpp), bpp*owidth);
//     }    
//     loadedData = newData;
//     delete[] oldData;
//   }
  
  
  fromData(loadedData,width,height,bpp,type);
  
//   delete[] loadedData;
}

uint Texture::pow2(const uint& a) {
  uint i=1;
  while (i<a) { i*=2; }
  return i;
}

bool Texture::operator<(const std::string& txt) const {
  return fileLocation<txt;
}

bool Texture::operator>(const std::string& txt) const {
  return fileLocation>txt;
}

bool Texture::operator==(const std::string& txt) const {
  return fileLocation==txt;
}



