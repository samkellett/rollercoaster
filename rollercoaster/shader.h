#ifndef SHADER_H
#define SHADER_H

#include "common.h"

// A class that provides a wrapper around an OpenGL shader
class Shader
{
public:
  Shader(std::string directory, std::string file, int type);
  void deleteShader();

  bool linesFromFile(std::string file, bool include_part, std::vector<std::string> *result);

  std::string name() const;
  int type() const;

  bool loaded();
  UINT shaderID();

  static const std::string DEFAULT_VERTEX_SHADER;
  static const std::string DEFAULT_FRAGMENT_SHADER;

private:
  std::string name_;
  UINT id_; // ID of shader
  int type_; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
  bool loaded_; // Whether shader was loaded and compiled
};

#endif
