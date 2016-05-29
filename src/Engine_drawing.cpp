#include "Engine.h"

void Engine::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  switch(CURRENT_SCREEN) {
    case SCR_MAINMENU: break;
    case SCR_PLAYSCREEN: drawPlayScreen(); break;
    case SCR_LOADING: drawloadingScreen(); break;
    default: break;
  }

}

void Engine::drawPlayScreen() {
    glClearColor(0.329, 0.607, 0.721, 1);
  
  /* 3D */  
  double ratio = double(WIDTH)/HEIGHT;
  SHADER::pushMatrix();
    SHADER::setPerspective(90/ratio,ratio,0.1,512);
    plane.getOrientation().conjugate().multMatrix();
    (-plane.getPosition()).translate();
    terrain.draw();
  SHADER::popMatrix();
  
  glDisable(GL_DEPTH_TEST);
  
  /*mapka*/
  SHADER::pushMatrix();
    SHADER::setOrtho(0,ratio,0,1);
    terrain.drawMap(ratio-0.3,1.0-0.3);
    SHADER::color(1,0,0,1);
    SHADER::setOrtho(0,WIDTH,0,HEIGHT);
    plane.drawSmall(WIDTH-HEIGHT*0.15, HEIGHT*0.85,(HEIGHT*0.3)/terrain.getSize());
    lblNorth.draw();
    lblSouth.draw();
    lblWest.draw();
    lblEast.draw();
  SHADER::popMatrix();
  
  /*hud info*/
  SHADER::pushMatrix();
    SHADER::setOrtho(0,WIDTH,0,HEIGHT);  
    lblFps.setText("FPS: ")<<FPSCounter.count;
    lblFps<<"; calc: "<<FPSCounter.FPS<<" ( "<<FPSCounter.average<<" ms/fm )";
    lblFps.draw();
    lblSpeed.setText("Speed: ")<<plane.getSpeed();
    lblSpeed.draw();
  SHADER::popMatrix();
  
  
  SHADER::color(0,1,0,1);
  
  /*kolo horyzontu*/
  SHADER::pushMatrix();
    double tmpScale = plane.getDirection()[1];
    double tmpScale2 = tmpScale*1.5;
    if (tmpScale<0) { tmpScale = -tmpScale; }
    tmpScale = 1.0-tmpScale*0.8;
    SHADER::translate(0,100*tmpScale2,0);
    SHADER::scale(tmpScale,tmpScale,tmpScale);
    SHADER::setOrtho(-100,100, -100,100, 0,180);
    plane.getOrientation().inverse().conjugate().multMatrix();
    glLineWidth(5);
    SHADER::color(0,0,0,1);
    SHADER::draw(GL_LINE_LOOP, horizonLine, hudCircle.size());
    glLineWidth(2);
    SHADER::color(0,1,0,1);
    SHADER::draw(GL_LINE_LOOP, horizonLine, hudCircle.size());
  SHADER::popMatrix();

  /*prawy i lewy pasek ( hud bars - jak kolwiek to nazwac po polsku )*/
  SHADER::pushMatrix();
    SHADER::setOrtho(0,400*ratio,0,400);
    SHADER::translate(200*ratio,200,0);
    uint quickFix;
    quickFix = (barSize*6)*(plane.getSpeed()/plane.getTopSpeed());
    quickFix -= quickFix%6; 
    SHADER::draw(GL_TRIANGLES, leftBar, quickFix);
    
    quickFix = (barSize*std::min(plane.getLiftForce(),2.0))*3;
    quickFix -= quickFix%6; 
    if (plane.getLiftForce()<=1) { SHADER::color(1,plane.getLiftForce(),0,1); }
    SHADER::draw(GL_TRIANGLES, rightBar, quickFix);
    
    glLineWidth(5);
    SHADER::color(0,0,0,1);
    SHADER::draw(GL_LINE_LOOP, rightBarOutline, barSize*2);
    SHADER::draw(GL_LINE_LOOP, leftBarOutline, barSize*2);
    glLineWidth(2);
    SHADER::color(0,1,0,1);
    SHADER::draw(GL_LINE_LOOP, rightBarOutline, barSize*2);
    SHADER::draw(GL_LINE_LOOP, leftBarOutline, barSize*2);
  SHADER::popMatrix();
}

void Engine::drawloadingScreen() {
  SHADER::pushMatrix();
    SHADER::setOrtho(0,WIDTH,0,HEIGHT);
    lblLoading.draw();
  SHADER::popMatrix();
  SDL_GL_SwapBuffers();
  terrain.generate(seed).makeList().makeTexture();
  CURRENT_SCREEN = SCR_PLAYSCREEN;
}