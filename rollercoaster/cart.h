#ifndef CART_H
#define CART_H

#include "gameobject.h"
#include "frame.h"

class ObjModel;

class Cart : public GameObject
{
public:
  Cart();
  ~Cart();

  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  ObjModel *model_;
  Frame frame_;

  glm::vec3 position_;
  float scale_;
};

#endif