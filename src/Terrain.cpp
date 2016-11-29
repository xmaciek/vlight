#include "Terrain.h"

#include "dice.hpp"

Terrain::Terrain() {
  size = 400;
  setDensity(256);
  numOfThreads = 1;
}

Terrain::~Terrain() {
//   glDeleteLists(displayList, 1);
}

static uint64_t getRangeSegment( uint64_t i, uint64_t n, uint64_t maxRange )
{
    if ( n == i ) {
        return maxRange;
    }
    return i * ( maxRange / n );
}

Terrain& Terrain::generate(const uint& seed) {
  m_seed = seed;
  progress = 0;
//   phases=(n*n)*0.02;
  phases=800;
  magnitude=0.1;
  
  // creating default flat grid, will be distorted in threaded run
  ss = step*-0.5*n;
  double sX = ss, sY = ss;
  for (uint i=0; i<n; i++) {
    sX=ss;
    for (uint j=0; j<n; j++) {  
      vertices.push_back(Vertex(sY,0.0,sX));
      sX+=step;
    }
    sY+=step;
  }
  progress += 0.01;
  
  
    // threaded generation
    std::cout << "number of working threads: " << numOfThreads << std::endl;
    std::vector<std::thread*> ths;
    for ( uint i=0; i<numOfThreads; i++ ) {
        ths.push_back( new std::thread( generateStatic, this,
                                        getRangeSegment( i, numOfThreads, vertices.size() ),
                                        getRangeSegment( i + 1, numOfThreads, vertices.size() ) ) );
    }
    for ( uint i=0; i<numOfThreads; i++ ) {
        ths[i]->join();
        delete ths[i];
    }
    ths.clear();


  // grid has been distorted, now creating 3d triangles based on vertices
  for (uint i=0; i<n-1; i++) {
  for (uint j=0; j<n-1; j++) {
    faces.push_back(Face(vertices[i*n+j], vertices[i*n+j+1], vertices[i*n+j+n]));
    faces[faces.size()-1].genNormal();
    faces.push_back(Face(vertices[i*n+j+n], vertices[i*n+j+1], vertices[i*n+j+n+1]));
    faces[faces.size()-1].genNormal();
    progress += 0.3/(n*n);
  }
  }

  // estimating water level
  std::vector<double> levels;
  for (uint i=0; i<vertices.size(); i++) { levels.push_back(vertices[i][1]); }
  std::sort(levels.begin(), levels.end());
  waterLevel = (levels[0]+levels[levels.size()-1])*0.51;
  return *this;
}
  
Terrain& Terrain::makeList() {
  double* tmpList = new double[faces.size()*9];
  uint j=0;
  for (uint i=0; i<faces.size(); i++) {
    tmpList[j++] = faces[i][0][0];
    tmpList[j++] = faces[i][0][1];
    tmpList[j++] = faces[i][0][2];
    tmpList[j++] = faces[i][1][0];
    tmpList[j++] = faces[i][1][1];
    tmpList[j++] = faces[i][1][2];
    tmpList[j++] = faces[i][2][0];
    tmpList[j++] = faces[i][2][1];
    tmpList[j++] = faces[i][2][2];
  }

  displayList = SHADER::makeBuffer(tmpList, sizeof(double)*9*faces.size());
  delete[] tmpList;

  double* waterSurfaceCorners = new double[18];
  j=0;
  while (j<18) {
    
    waterSurfaceCorners[j++] = -ss;
    waterSurfaceCorners[j++] = waterLevel;
    waterSurfaceCorners[j++] = ss;
    
    waterSurfaceCorners[j++] = ss;
    waterSurfaceCorners[j++] = waterLevel;
    waterSurfaceCorners[j++] = -ss;
    
    waterSurfaceCorners[j++] = -ss;
    waterSurfaceCorners[j++] = waterLevel;
    waterSurfaceCorners[j++] = -ss;
    
    
    waterSurfaceCorners[j++] = ss;
    waterSurfaceCorners[j++] = waterLevel;
    waterSurfaceCorners[j++] = ss;
    
    waterSurfaceCorners[j++] = ss;
    waterSurfaceCorners[j++] = waterLevel;
    waterSurfaceCorners[j++] = -ss;
    
    waterSurfaceCorners[j++] = -ss;
    waterSurfaceCorners[j++] = waterLevel;
    waterSurfaceCorners[j++] = ss;
  }
  waterSurface = SHADER::makeBuffer(waterSurfaceCorners,sizeof(double)*18);
  delete[] waterSurfaceCorners;
  
  double div=vertices[0][1], offset=vertices[0][1];
  uint size = faces.size()*3;
  j=0;
  float* maxes = new float[size];
  for (uint i=0; i<faces.size(); i++) {
    maxes[j++] = faces[i][0][1];
    if (maxes[j-1]>div) { div = maxes[j-1]; }
    if (maxes[j-1]<offset) { offset = maxes[j-1]; }
    maxes[j++] = faces[i][1][1];
    if (maxes[j-1]>div) { div = maxes[j-1]; }
    if (maxes[j-1]<offset) { offset = maxes[j-1]; }
    maxes[j++] = faces[i][2][1];
    if (maxes[j-1]>div) { div = maxes[j-1]; }
    if (maxes[j-1]<offset) { offset = maxes[j-1]; }
  }
  div-=offset;
  for (uint i=0; i<size; i++) {
    maxes[i]-=offset;
    maxes[i]/=div;
//     std::cout<<i<<": "<<maxes[i]<<"\n";
  }
  double* maxesArray = new double[size*4];
  j=0;
  for (uint i=0; i<size;i++) {
    maxesArray[j++] = maxes[i];
    maxesArray[j++] = maxes[i];
    maxesArray[j++] = maxes[i];
    maxesArray[j++] = 1;
    
  }
  delete[] maxes;
  maxesList = SHADER::makeBuffer(maxesArray,sizeof(double)*size*4);
  delete[] maxesArray;
//   std::cout<<"displayList: "<<displayList<<std::endl;
  
  return *this;
}
  
double Terrain::getProgress() { return progress; }

Terrain& Terrain::draw() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  SHADER::pushMatrix();
    SHADER::colorArray(maxesList);
    SHADER::draw(GL_TRIANGLES, displayList, faces.size()*3);
    SHADER::color(0.1, 0.4, 1.0, 0.4);
    SHADER::draw(GL_TRIANGLES, waterSurface, 18);
  SHADER::popMatrix();
  return *this;
}


double Terrain::getWaterLevel() { return waterLevel; }

double Terrain::getSize() {
  return size;
}

Terrain& Terrain::setDensity(const uint& i) {
  n=i;
  step = double(size)/n;
  return *this;
}

Terrain& Terrain::save() {
//   std::fstream water("water.mtl");
//   water<<"newmtl Water\n";
//   water<<"Ns 96.078431\n";
//   water<<"Ka 0.000000 0.000000 0.000000\n";
//   water<<"Kd 0.048081 0.096225 0.640000\n";
//   water<<"Ks 0.500000 0.500000 0.500000\n";
//   water<<"Ni 1.000000\n";
//   water<<"d 1.000000\n";
//   water<<"illum 2\n";
//   water.close();
//   
//   std::sstream ss;
//   std::string txt;
//   ss<<seed;
//   getline(txt,ss);
//   txt+=std::string(".obj");
//   std::fstream land(txt.c_str());
//   land<<"mtllib Water.mtl\n";
//   for (uint i=0; i<vertices.size(); i++) {
//     land<<"v "<<vertices[i][0]<<" "<<vertices[i][1]<<" "<<vertices[i][2]<<"\n"
//   }
//   land<<"v "<<-ss<<" "<<waterLevel<<" "<<-ss<<"\n";
//   land<<"v "<<-ss<<" "<<waterLevel<<" "<<ss<<"\n";
//   land<<"v "<<ss<<" "<<waterLevel<<" "<<ss<<"\n";
//   land<<"v "<<ss<<" "<<waterLevel<<" "<<-ss<<"\n";
//   
  
  return *this;
}


Terrain& Terrain::makeTexture() {
  uint size = n*n*4;
  
  double min,max;
  double heights[vertices.size()];
  heights[0] = vertices[0][1];
  min = heights[0];
  max = heights[0];
  for (uint i=1; i<vertices.size(); i++) {
    heights[i] = vertices[i][1];
    if (heights[i]<min) { min = heights[i]; }
    if (heights[i]>max) { max = heights[i]; }
  }
  
  if (min<0) { min*=-1; }
  max += min;
  for (uint i=0; i<vertices.size(); i++) {
    heights[i] += min;
    heights[i] /= max;
  }
  
  
  uchar* data = new uchar[size];
  uint j=0;
  for (uint i=0; i<vertices.size(); i++) {
    if (vertices[i][1]>waterLevel) {
      data[j++] = 255*heights[i];
      data[j++] = 255*heights[i];
      data[j++] = 255*heights[i];
    }
    else {
      data[j++] = 0;
      data[j++] = 128*heights[i];
      data[j++] = 128*heights[i]+128;
    }
    data[j++] = 256*0.95;
  }
  
  texture.fromData(data,n,n,4,GL_RGBA);
  
  /*mapa koordynaty*/
  double* map = new double[18];
  memset(map,0,sizeof(double)*18);
  map[3] = 0.3;
  map[7] = 0.3;
  map[10] = 0.3;
  map[12] = 0.3;
  map[15] = 0.3;
  map[16] = 0.3;
  mapID = SHADER::makeBuffer(map,sizeof(double)*18);
  delete[] map;
  
  
  /*mapa UV*/
  map = new double[12];
  memset(map,0,sizeof(double)*12);
  double xratio = double(texture.owidth)/texture.width;
  double yratio = double(texture.oheight)/texture.height;
//   map[2] = yratio;
//   map[5] = xratio;
//   map[7] = xratio;
//   map[8] = yratio;
//   map[10] = yratio;
//   map[11] = xratio;
  map[0] = xratio;
  map[2] = xratio;
  map[3] = yratio;
  map[8] = xratio;
  map[9] = yratio;
  map[11] = yratio;
//   map[8] = xratio;
//   map[9] = yratio;
//   map[10] = 
  mapUVID = SHADER::makeBuffer(map,sizeof(double)*12);
  delete[] map;
  
  
  map = new double[9];
  memset(map,0,sizeof(double)*9);
  map[1] = 0.3;
  map[6] = 0.3;
  mapOutlineID = SHADER::makeBuffer(map,sizeof(double)*9);
  delete[] map;
  
  
  return *this;
  
}

Terrain& Terrain::drawMap(const double& w, const double& h) {
  SHADER::pushMatrix();
    SHADER::translate(w,h,0);
    
    glLineWidth(7);
    SHADER::color(0,0,0,1);
    SHADER::draw(GL_LINE_STRIP,mapOutlineID,3);
    glLineWidth(3);
    SHADER::color(0,1,0,1);
    SHADER::draw(GL_LINE_STRIP,mapOutlineID,3);
    glLineWidth(2);
    
    texture.use();
    SHADER::texCoord(mapUVID);
    SHADER::draw(GL_TRIANGLES,mapID,6);
  SHADER::popMatrix();
  return *this;
}

int Terrain::generateStatic( void* param, uint64_t from, uint64_t to )
{
    static_cast<Terrain*>( param )->genThreded( from, to );
    return 0;
}

Terrain& Terrain::setThreads(const uint& i) {
  numOfThreads = i;
  return *this;
}


void Terrain::genThreded( uint64_t from, uint64_t to ) {
  thread_local Vertex randAngle;
  thread_local Vertex tmp2;
  thread_local Vertex trans;
  thread_local uint localPhases = phases;
  thread_local double localMagnitude = magnitude;
  thread_local double max = step*n;
  thread_local uint i;
  thread_local uint j;
  thread_local uint k;
  thread_local Dice dice( m_seed );

  for (i=0; i<3; i++) {
  for (j=0; j<localPhases; j++) {
      trans[0] = dice.nextd() * max - max / 2;
      trans[2] = dice.nextd() * max - max / 2;
      randAngle[0] = dice.nextd() - 0.5;
      randAngle[2] = dice.nextd() - 0.5;
      randAngle.normalize();

    for (k=from; k<to; k++) {
      tmp2 = trans + vertices[k];
      tmp2[1]=0;
      tmp2.normalize();
      if ( randAngle.angleTo( tmp2 ) >= 90 ) {
        vertices[k][1] += localMagnitude;
      } else {
        vertices[k][1] -= localMagnitude;
      }
    }
    
  }
  localPhases/=10;
  localMagnitude*=1.5;
  } 
  
}
