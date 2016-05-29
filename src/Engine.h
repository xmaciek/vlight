#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mixer.h>

#include "includeShortcut.h"
#include "SHADER.hpp"

#include "Texture.h"
#include "Terrain.h"
#include "Aeroplane.h"
#include "Quaternion.h"
#include "Font.h"
#include "Label.h"
#include "fpsMeter.h"
#include "Circle.h"

class Engine {
public:
  Engine(int argc, char** argv);
  void execute();
  
private:
  
  Engine(Engine const&); 
  Engine& operator=(Engine const&);
  
  void draw();
  void update();
  
  ulong seed;
  
  uint updateStamp;
  static void updateWraper(void* param);
  
  void delay(uint& stamp);
  
  void cleanup();
  void resize();
  
  bool running;
  
  bool initSDL();
  void initGL();
  void initGameContent();
  
  bool initNewSurface(const uint& W, const uint& H, bool F);
  SDL_Surface* display;
  int WIDTH, HEIGHT;
  
  void processEvent(SDL_Event *e);
  
  ubyte CURRENT_SCREEN;
  static const ubyte SCR_MAINMENU = 0;
  static const ubyte SCR_PLAYSCREEN = 1;
  static const ubyte SCR_LOADING = 2;
  
  void keyDown(SDL_Event *e);
  void keyUp(SDL_Event *e);
  void mouseDown(SDL_Event *e);
  /* =========== */
  
  void drawPlayScreen();
  void updatePlayScreen();
  fpsMeter FPSCounter;
  
  Label lblLoading;
  void drawloadingScreen();
  void updateLoadingScreen();
  
  
  Terrain terrain;
  Aeroplane plane;
  
  Label lblFps;
  Label lblSpeed;
  
  Label lblNorth,lblSouth,lblWest,lblEast;
  
  Circle hudCircle;
  
  uint leftBarOutline, leftBar, barSize;
  uint rightBarOutline, rightBar;
  uint horizonLine;
  
};