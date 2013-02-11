#ifndef SHADER_H
#define SHADER_H

#include "common.h"

// A class that provides a wrapper around an OpenGL shader
class Shader
{
public:
  Shader();

  bool loadShader(std::string file, int type);
  void deleteShader();

  bool linesFromFile(std::string file, bool include_part, std::vector<std::string> *result);

  bool loaded();
  UINT shaderID();

private:
  UINT id_; // ID of shader
  int type_; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
  bool loaded_; // Whether shader was loaded and compiled
};

#endif
