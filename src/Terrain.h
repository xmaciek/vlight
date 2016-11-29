#pragma once

#include "includeShortcut.h"
#include "Vertex.h"
#include "Face.h"
#include "Texture.h"

#include <cstdint>

class Terrain {
private:
  std::vector<Vertex> vertices;
  std::vector<Face> faces;
  uint displayList,maxesList,waterSurface;
  double waterLevel;
  double progress;
  double ss;
  double step;
  double magnitude;
  double size;
  uint64_t m_seed;
  uint n;
  uint phases;
  Texture texture;
  uint mapID, mapUVID, mapOutlineID;
  std::mutex mtxRand;
  std::mutex mtxGen;
  uint numOfThreads;
  void genThreded( uint64_t from, uint64_t to );
  
public:
  Terrain();
  ~Terrain();
  Terrain& generate(const uint& seed);
  Terrain& makeList();
  Terrain& draw();
  double getWaterLevel();
  double getProgress();
  Terrain& save();
  Terrain& setDensity(const uint& i);
  Terrain& setThreads(const uint& i);
  
  static int generateStatic( void* param, uint64_t from, uint64_t to );
  
  
  Terrain& makeTexture();
  Terrain& drawMap(const double& w, const double& h);
  double getSize();
};
