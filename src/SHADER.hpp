#pragma once

/* version 0.3 */

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <stack>
#include <string>
#include <fstream>

#define SINGLE_COLOR 0
#define ARRAY_COLOR 1
#define TEXTURE_COLOR 2

#ifndef ubyte
  #define ubyte unsigned char
#endif

class SHADER {
public:
  ~SHADER();
  static void init();
  
  
  static void pushMatrix();
  static void popMatrix();
  static void clearMatrix();
  static void syncMatrix();
  static void multMatrix(const float* matrix);
  
  
  static double deg2rad;
  static void rotate(const double &angle, const ubyte& X, const ubyte &Y, const ubyte &Z);
  static void rotateRad(const double &angle, const ubyte& X, const ubyte &Y, const ubyte &Z);
  static void translate(const double &X, const double &Y, const double &Z);
  static void scale(const double &X, const double &Y, const double &Z);
  
  static void color(const double& R, const double& G, const double& B, const double& A);
  static void colorArray(const uint& index);

  static uint makeBuffer(const double* array, const uint& size);
  static void deleteBuffer(const uint& buffer);
  
  static void texCoord(const uint& buffer);
  static void draw(const uint& type, const uint& buffer, const uint& size);
  
  static void setOrtho(const double &minX, const double &maxX, const double &minY, const double &maxY, const double &minZ=-1.0, const double &maxZ=1.0);
  static void setPerspective(const double &fovY, const double &ratio, const double &min, const double &max);
//   static void lookAt()
  
private:
  SHADER();
  SHADER(const SHADER&);
  SHADER& operator=(const SHADER&);
  
  uint loadShaders(const char* vertexShader, const char* fragmentShader);
  uint programID;
  uint vertexArrayID;
  uint dummyBuffer;
  static SHADER* ptr;
  std::stack<glm::mat4> modelMatrix, viewMatrix, projectionMatrix;

  uint modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation;
  uint vertexLocation, vertexUVlocation;
  uint colorStanceLocation, colorValueLocation, colorArrayLocation;
  
  
  
  
};





