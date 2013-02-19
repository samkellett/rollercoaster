#ifndef BUILDER_H
#define BUILDER_H

#include "common.h"

class ShaderProgram;

class Builder
{
public:
  Builder();
  ~Builder();

  void keyboardListener(double dt);

  void update(double dt);
  void render();

private:
  void createPath();

  ShaderProgram *main_;
  std::vector<glm::vec3> points_;

  unsigned int vao_;
};

#endif