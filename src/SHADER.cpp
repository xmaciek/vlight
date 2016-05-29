#include "SHADER.hpp"

SHADER* SHADER::ptr = NULL;
double SHADER::deg2rad = atan(1)*4.0/180.0;

SHADER::SHADER() {
  glewExperimental=true;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    exit(1);
  }
  
  glGenVertexArrays(1, &vertexArrayID);
  glBindVertexArray(vertexArrayID);
  
  programID = loadShaders("vertexShader.vert", "fragmentShader.frag");
  glUseProgram(programID);
  
  modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
  viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
  projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
//   vertexLocation = glGetUniformLocation(programID, "vertexPosition");
  vertexLocation = 0;
  vertexUVlocation = 1;
//   ptr->verticesLocation = glGetUniformLocation(programID, "verticesUV");
  colorStanceLocation = glGetUniformLocation(programID, "colorStance");
  colorValueLocation = glGetUniformLocation(programID, "colorValue");
//   colorArrayLocation = glGetUniformLocation(programID, "colorArray");
  colorArrayLocation = 5;
  modelMatrix.push(glm::mat4(1));
  viewMatrix.push(glm::mat4(1));
  projectionMatrix.push(glm::mat4(1));
}

SHADER::~SHADER() {
//   delete ptr;
}

void SHADER::init() {
  static bool lock = false;
  if (lock) { return; }
  lock = true;
//   static view realView();
  ptr = new SHADER();

  
  glEnableVertexAttribArray(ptr->vertexLocation);

}

void SHADER::pushMatrix() {
  ptr->modelMatrix.push(ptr->modelMatrix.top());
  ptr->viewMatrix.push(ptr->viewMatrix.top());
  ptr->projectionMatrix.push(ptr->projectionMatrix.top());
}

void SHADER::popMatrix() {
  ptr->modelMatrix.pop();
  ptr->viewMatrix.pop();
  ptr->projectionMatrix.pop();
  syncMatrix();
}



void SHADER::translate(const double &X, const double &Y, const double &Z) {
  ptr->modelMatrix.top() = glm::translate(ptr->modelMatrix.top(), glm::vec3(X,Y,Z));
  syncMatrix();
}



void SHADER::rotate(const double &angle, const ubyte& X, const ubyte &Y, const ubyte &Z) {
  ptr->modelMatrix.top() = glm::rotate(ptr->modelMatrix.top(), (float)(angle*deg2rad), glm::vec3(X,Y,Z));
  syncMatrix();
}

void SHADER::rotateRad(const double &angle, const ubyte& X, const ubyte &Y, const ubyte &Z) {
  ptr->modelMatrix.top() = glm::rotate(ptr->modelMatrix.top(), (float)angle, glm::vec3(X,Y,Z));
  syncMatrix();
}

void SHADER::scale(const double &X, const double &Y, const double &Z) {
  ptr->modelMatrix.top() = glm::scale(ptr->modelMatrix.top(), glm::vec3(X,Y,Z));
  syncMatrix();
}


void SHADER::setOrtho(const double &minX, const double &maxX, const double &minY, const double &maxY, const double &minZ, const double &maxZ) {
  ptr->projectionMatrix.top() = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
  syncMatrix();
}

void SHADER::setPerspective(const double &fovY, const double &ratio, const double &min, const double &max) {
  ptr->projectionMatrix.top() = glm::perspective(fovY*deg2rad, ratio, min, max);
  syncMatrix();
}

void SHADER::syncMatrix() {  
  glUniformMatrix4fv(ptr->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(ptr->modelMatrix.top()));
  glUniformMatrix4fv(ptr->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(ptr->viewMatrix.top()));
  glUniformMatrix4fv(ptr->projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(ptr->projectionMatrix.top()));
}

void SHADER::multMatrix(const float* matrix) {
  ptr->modelMatrix.top() = glm::make_mat4(matrix)*ptr->modelMatrix.top(); 
  syncMatrix();
}


void SHADER::color(const double& R, const double& G, const double& B, const double& A) {  
  glDisableVertexAttribArray(ptr->vertexUVlocation); 
  glDisableVertexAttribArray(ptr->colorArrayLocation);
  glUniform1i(ptr->colorStanceLocation, SINGLE_COLOR);
  glUniform4f(ptr->colorValueLocation, (float)R, (float)G, (float)B, (float)A);
}
void SHADER::colorArray(const uint& index) { 
  glDisableVertexAttribArray(ptr->vertexUVlocation);
  glEnableVertexAttribArray(ptr->colorArrayLocation);
  glUniform1i(ptr->colorStanceLocation, ARRAY_COLOR);
  glBindBuffer(GL_ARRAY_BUFFER, index);
  glVertexAttribPointer(ptr->colorArrayLocation, 4, GL_DOUBLE, GL_TRUE, 0, (void*)0);
}

uint SHADER::makeBuffer(const double* array, const uint& size) {
  uint location=0;
  glGenBuffers(1, &location);
  glBindBuffer(GL_ARRAY_BUFFER, location);
  glBufferData(GL_ARRAY_BUFFER, size, array, GL_STATIC_DRAW);
  return location;
}

void SHADER::deleteBuffer(const uint& buffer) {
  glDeleteBuffers(1,&buffer);
}

void SHADER::texCoord(const uint& buffer) {  
  glDisableVertexAttribArray(ptr->colorArrayLocation);
  glEnableVertexAttribArray(ptr->vertexUVlocation);
  glUniform1i(ptr->colorStanceLocation, TEXTURE_COLOR);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(ptr->vertexUVlocation, 2, GL_DOUBLE, GL_TRUE, 0, (void*)0);
}


void SHADER::draw(const uint& type, const uint& buffer, const uint& size) {
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(ptr->vertexLocation, 3, GL_DOUBLE, GL_FALSE, 0, (void*)0);
  glDrawArrays(type, 0, size);
}

uint SHADER::loadShaders(const char* vertexShader, const char* fragmentShader) {
  uint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  uint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
 
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertexShader, std::ios::in);
  if (VertexShaderStream.is_open()) {
    std::string Line = "";
    while(getline(VertexShaderStream, Line)) {
      VertexShaderCode += "\n" + Line;     
    }
    VertexShaderStream.close();
  }
 
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragmentShader, std::ios::in);
  if (FragmentShaderStream.is_open()) {
    std::string Line = "";
    while(getline(FragmentShaderStream, Line)) {
      FragmentShaderCode += "\n" + Line;
    }
    FragmentShaderStream.close();
  }
 
  int Result = GL_FALSE;
  int InfoLogLength;
 
// Compile Vertex Shader
  printf("Compiling shader : %s\n", vertexShader);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);
 
// Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> VertexShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
 
// Compile Fragment Shader
  printf("Compiling shader : %s\n", fragmentShader);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);
 
// Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
 
// Link the program
  fprintf(stdout, "Linking program\n");
  uint ID = glCreateProgram();
  glAttachShader(ID, VertexShaderID);
  glAttachShader(ID, FragmentShaderID);
  glLinkProgram(ID);
 
// Check the program
  glGetProgramiv(ID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
  glGetProgramInfoLog(ID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
  fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
 
  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);
 
  return ID;
}
