#ifndef SUPPORT_H
#define SUPPORT_H

#include "common.h"
#include "matrixstack.h"
#include "vbo.h"

class ShaderProgram;

class Support
{
public:
  Support(glm::vec3 position);
  ~Support();

  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  UINT vao_;
  VBO vbo_;

  glm::vec3 position_;
};

#endif