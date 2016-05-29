#include "Engine.h"

Engine::Engine(int argc, char** argv) {
  display = NULL;
  WIDTH = 640;
  HEIGHT = 360;
  seed = time(NULL);
  argc--; argv++;
  std::string param;
  while (argc>0) {
    param = std::string(argv[0]);
    if (param==std::string("--seed")) { seed = atoi(argv[1]); argc--; argv++; }
    else if (param==std::string("--width")) { WIDTH = atoi(argv[1]); argc--; argv++; }
    else if (param==std::string("--height")) { HEIGHT = atoi(argv[1]); argc--; argv++; }
    else if (param==std::string("--density")) { terrain.setDensity((uint)atoi(argv[1])); argc--; argv++; }
    else if (param==std::string("--threads")) { terrain.setThreads((uint)atoi(argv[1])); argc--; argv++; }
    argc--; argv++;    
  }
  
}

bool Engine::initSDL() {
  std::cout<<"Initializing SDL..."<<std::endl;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)<0) { return false; }
  
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
 
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
 
  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);
 
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
  
  display = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_DOUBLEBUF | SDL_OPENGL | SDL_RESIZABLE);
  if (display==NULL) { return false; }
  return true;
}

void Engine::initGL() {
  std::cout<<"Initializing OpenGL..."<<std::endl;
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  SHADER::init();
  
  
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  
  resize();
}

void Engine::execute() {
  
  running = initSDL();
  if (!running) { return; }
  initGL();
  initGameContent();
  
  SDL_Event event;
  
  std::thread updateThread(updateWraper,this);
  
  while (running) {
    while (SDL_PollEvent(&event)) { processEvent(&event); }
    FPSCounter.beginFrame();
    draw();
    FPSCounter.endFrame();
    SDL_GL_SwapBuffers();
    
  }
  updateThread.join();
  cleanup();
}

void Engine::cleanup() {
  SDL_FreeSurface(display);
  SDL_Quit();
}


bool Engine::initNewSurface(const uint& W, const uint& H, bool F) {
  WIDTH = W;
  HEIGHT = H;
  SDL_Surface *tmp = display, *tmp2 = NULL;
  if (F) { tmp2 = SDL_SetVideoMode(W, H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN); }
  else { tmp2 = SDL_SetVideoMode(W, H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_RESIZABLE); }
  if (tmp2==NULL) { return false; }
  display = tmp2;
  if (tmp!=NULL) { SDL_FreeSurface(tmp); }
  return true;
}
