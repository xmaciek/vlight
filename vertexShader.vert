#version 330 core

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormals;

/*layout(location = 3)*/ uniform int colorStance;
/*layout(location = 4)*/ uniform vec4 colorValue;
layout(location = 5) in vec4 colorArray;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

flat out int fragmentStance;
out vec4 fragmentColor;
out vec2 fragmentUV;

void colorFunction() {
  fragmentStance = 0;
  fragmentColor = colorValue;
}

void colorArrayFunction() {
  float red = colorArray[0];
  float green = colorArray[1];
  float blue = colorArray[2];
  float alpha = colorArray[3];
  float tmp = colorArray[0];
  if (tmp<0.45) {
//     blue = blue*0.5+0.5;
//     green *= 0.75;
//     red = 0;
    blue *= 0.2;
//     red = 1;

  } 
  if ( tmp>=0.45 && tmp<0.55 ) {
    blue *= 0.325;
    green=min(green*1.35,1.0);
    
    red = 1;
  }
  if ( tmp>=0.55 && tmp<0.65 ) {
    blue = 0;
    red = 0;
  }
  if (tmp>=0.65 && tmp<0.8) {
    blue = 0.1;
    green *=0.8;
    red = 0.1;
  }
  fragmentStance = 0;
  fragmentColor = vec4(red,green,blue,alpha);
}

void colorTextureFunction() {
  fragmentStance = 1;
  fragmentUV = vertexUV;
  fragmentColor = vec4(1,1,1,1);
}


void main(){
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition,1);
  switch(colorStance) {
    case 0: colorFunction(); break;
    case 1: colorArrayFunction(); break;
    case 2: colorTextureFunction(); break;
    default: fragmentColor = vec4(1,1,1,1); break;
  }
 
}
