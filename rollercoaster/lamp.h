#ifndef LAMP_H
#define LAMP_H

#include "gameobject.h"
#include "sphere.h"

class ObjModel;

class Lamp : public GameObject
{
public:
  Lamp(glm::vec2 position, glm::vec3 colour = glm::vec3(0.0f));
  ~Lamp();

  void init(ShaderProgram *program);
  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  ObjModel *model_;
  Sphere bulb_;

  glm::vec3 colour_;
  glm::vec3 position_;
  float scale_;
};

#endif