#ifndef BUILDER_H
#define BUILDER_H

#include "common.h"
#include "gameobject.h"

class Builder : public GameObject
{
public:
  Builder();
  ~Builder();

  void keyboardListener(double dt);

  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  void createPath();

  std::vector<glm::vec3> points_;

  unsigned int vao_;
};

#endif
