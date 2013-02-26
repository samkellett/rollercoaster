#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "common.h"

class Shader;

// A class the provides a wrapper around an OpenGL shader program
class ShaderProgram
{
public:
  ShaderProgram();

  void create();
  void deleteProgram();

  bool addShader(Shader* shader);
  bool link();

  void use();

  UINT id();

  // Setting vectors
  void setUniform(std::string name, glm::vec2 *vectors, int count = 1);
  void setUniform(std::string name, const glm::vec2 vector);

  void setUniform(std::string name, glm::vec3 *vectors, int count = 1);
  void setUniform(std::string name, const glm::vec3 vector);

  void setUniform(std::string name, glm::vec4 *vectors, int count = 1);
  void setUniform(std::string name, const glm::vec4 vector);

  // Setting floats
  void setUniform(std::string name, float* values, int count = 1);
  void setUniform(std::string name, const float value);

  // Setting 3x3 matrices
  void setUniform(std::string name, glm::mat3* matrices, int count = 1);
  void setUniform(std::string name, const glm::mat3 matrix);

  // Setting 4x4 matrices
  void setUniform(std::string name, glm::mat4* matrices, int count = 1);
  void setUniform(std::string name, const glm::mat4 matrix);

  // Setting integers
  void setUniform(std::string name, int* values, int count = 1);
  void setUniform(std::string name, const int value);


private:
  UINT id_; // ID of program
  bool linked_; // Whether program was linked and is ready to use
};

#endif
