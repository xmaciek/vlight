#pragma once

// #include <GL/gl.h>
// #include <GL/glu.h>

#include "SHADER.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mixer.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <typeinfo>
#include <thread>
#include <chrono>
#include <mutex>

// using namespace std;
#define randf() (double)rand()/RAND_MAX
#define DEBUG std::cout<<"HERE OK"<<std::endl;


// typedef unsigned char uchar;
#define uchar unsigned char

class CONSTANTS {
public:
  static const double DELTATIME;
  static const double PI;
  static const double FI;
  static const double FI2;
  static const double deg2rad;
  static const double rad2deg;

};