#ifndef LIGHTING_H
#define LIGHTING_H

#include "matrixstack.h"

class ShaderProgram;

class Lighting
{
public:
  static void global(glutil::MatrixStack &modelview, ShaderProgram *program);
  static void diffuseSpecular(ShaderProgram *program, float specular = 0.0f, float diffuse = 1.0f, float ambient = 0.5f);

  static void registerSource(glm::vec3 position, glm::vec3 colour);
private:
  static std::vector<glm::vec3> sources_;
  static std::vector<glm::vec3> colours_;
};

#endif
