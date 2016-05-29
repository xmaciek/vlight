#include "Engine.h"


void Engine::processEvent(SDL_Event *e) {
  switch(e->type) {
    case SDL_QUIT: running = false; break;
    case SDL_KEYDOWN: keyDown(e); break;
    case SDL_KEYUP: keyUp(e); break;
    case SDL_MOUSEBUTTONDOWN: mouseDown(e); break;   
    case SDL_VIDEORESIZE:
      initNewSurface(e->resize.w, e->resize.h, false);
      resize();
      break;
  }
}

void Engine::keyDown(SDL_Event *e) {
  if (e->key.keysym.sym==SDLK_ESCAPE) { running = false; return; }
  switch (CURRENT_SCREEN) {
    case SCR_PLAYSCREEN:
      switch (e->key.keysym.sym) {
        case SDLK_s: plane.pitch(true, false); break;
        case SDLK_w: plane.pitch(true, true); break;
        case SDLK_a: plane.roll(true, false); break;
        case SDLK_d: plane.roll(true, true); break;
        case SDLK_q: plane.yaw(true, false); break;
        case SDLK_e: plane.yaw(true, true); break;
        case SDLK_u: plane.changeSpeed(true, true); break;
        case SDLK_o: plane.changeSpeed(true, false); break;
        default: break;
      } break;
  }
}

void Engine::keyUp(SDL_Event *e) {
  switch (CURRENT_SCREEN) {
    case SCR_PLAYSCREEN:
      switch (e->key.keysym.sym) {
        case SDLK_s: plane.pitch(false, false); break;
        case SDLK_w: plane.pitch(false, true); break;
        case SDLK_a: plane.roll(false, false); break;
        case SDLK_d: plane.roll(false, true); break;
        case SDLK_q: plane.yaw(false, false); break;
        case SDLK_e: plane.yaw(false, true); break;
        case SDLK_u: plane.changeSpeed(false, true); break;
        case SDLK_o: plane.changeSpeed(false, false); break;
        default: break;
      } break;
  }
}

void Engine::mouseDown(SDL_Event *e) {}

void Engine::update() {
  switch (CURRENT_SCREEN) {
    case SCR_PLAYSCREEN: updatePlayScreen(); break;
    case SCR_LOADING: updateLoadingScreen(); break;
  }

    FPSCounter.update();
  
}

void Engine::resize() {
  glViewport(0, 0, WIDTH, HEIGHT);
  lblFps.move(4, HEIGHT-16);
  lblSpeed.move(4, HEIGHT-32);
  
  lblNorth.move(WIDTH-HEIGHT*0.15, HEIGHT-16);
  lblSouth.move(WIDTH-HEIGHT*0.15, HEIGHT*0.7);
  lblWest.move(WIDTH-HEIGHT*0.3+4, HEIGHT*0.85);
  lblEast.move(WIDTH-12, HEIGHT*0.85);
  
}


void Engine::initGameContent() {
  Font::init();
  updateStamp = 0;
  lblLoading.setText("Generating terrain, please wait...").setAlign(Label::alignCenter).move(WIDTH/2,HEIGHT/2);
  hudCircle = Circle(1440,200);
  Circle smallerCircle(1440,190);
  CURRENT_SCREEN = SCR_LOADING;
  
  barSize = hudCircle.size()/4;
  uint tmpSize = barSize*2*3;
  uint j=0;
  double tmpBarOutline[tmpSize];
  for (uint i=0; i<barSize; i++) {
    tmpBarOutline[j++] = hudCircle.getX(barSize*0.5+i);
    tmpBarOutline[j++] = hudCircle.getY(barSize*0.5+i);
    tmpBarOutline[j++] = 0;
  }
  for (uint i=barSize; i>0; i--) {
    tmpBarOutline[j++] = smallerCircle.getX(barSize*0.5+i);
    tmpBarOutline[j++] = smallerCircle.getY(barSize*0.5+i);
    tmpBarOutline[j++] = 0;
  }
  rightBarOutline = SHADER::makeBuffer(tmpBarOutline,sizeof(double)*tmpSize);
  
  j=0;
  while (j<tmpSize) {
    tmpBarOutline[j]*=-1;
    j+=3;
  }
  leftBarOutline = SHADER::makeBuffer(tmpBarOutline,sizeof(double)*tmpSize);
  
  j=0;
  double tmpBar[barSize*18];
  for (uint i=barSize; i>0; i--) {
    uint x = barSize*0.5 + i;
    tmpBar[j++] = hudCircle.getX(x);
    tmpBar[j++] = hudCircle.getY(x);
    tmpBar[j++] = 0;
    
    tmpBar[j++] = smallerCircle.getX(x-1);
    tmpBar[j++] = smallerCircle.getY(x-1);
    tmpBar[j++] = 0;
    
    tmpBar[j++] = smallerCircle.getX(x);
    tmpBar[j++] = smallerCircle.getY(x);
    tmpBar[j++] = 0;
    
    /**/
    tmpBar[j++] = smallerCircle.getX(x-1);
    tmpBar[j++] = smallerCircle.getY(x-1);
    tmpBar[j++] = 0;
    
    tmpBar[j++] = hudCircle.getX(x);
    tmpBar[j++] = hudCircle.getY(x);
    tmpBar[j++] = 0;
    
    tmpBar[j++] = hudCircle.getX(x-1);
    tmpBar[j++] = hudCircle.getY(x-1);
    tmpBar[j++] = 0;
    
  }
  rightBar = SHADER::makeBuffer(tmpBar,sizeof(double)*barSize*18);
  j=0;
  while (j<barSize*18) {
    tmpBar[j]*=-1;
    j+=3;
  }
  
  j=0; double tmp[3]; uint tmpSwap = sizeof(double)*3;
  while (j<barSize*18) {
    memcpy(tmp,&tmpBar[j],tmpSwap);
    memcpy(&tmpBar[j],&tmpBar[j+6],tmpSwap);
    memcpy(&tmpBar[j+6],tmp,tmpSwap);
    j+=9;
  }
  leftBar = SHADER::makeBuffer(tmpBar, sizeof(double)*barSize*18);
  
  hudCircle = Circle(360,200);
  double tmpHorizonLine[hudCircle.size()*3];
  j=0; uint i=0;
  while (i<hudCircle.size()) {
    tmpHorizonLine[j++] = hudCircle.getX(i);
    tmpHorizonLine[j++] = 0;
    tmpHorizonLine[j++] = hudCircle.getY(i);
    i++;
  }
  horizonLine = SHADER::makeBuffer(tmpHorizonLine, sizeof(double)*3*hudCircle.size());
  plane.setUp();
  
  lblNorth.setText("N");
  lblSouth.setText("S");
  lblWest.setText("W");
  lblEast.setText("E");
  
  
}

void Engine::updateWraper(void* param) {
  Engine* ptre = (Engine*)param;
  while (ptre->running) {
    ptre->update();
    ptre->delay(ptre->updateStamp);    
  }
}
  
void Engine::delay(uint& stamp) {
  uint now = SDL_GetTicks();
  if (stamp<=now) {
    stamp = now + 1000*CONSTANTS::DELTATIME;
    return;
  }
  SDL_Delay(stamp-now);
}

void Engine::updateLoadingScreen() {
//   CURRENT_SCREEN = SCR_LOADING;
}

void Engine::updatePlayScreen() {
  plane.update();
}
