#ifndef TREE_H
#define TREE_H

#include "gameobject.h"

class ObjModel;

class Tree : public GameObject
{
public:
  Tree(std::string model, glm::vec3 position = glm::vec3(0.0f));
  ~Tree();

  std::string program();

  void init(ShaderProgram *program);
  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  glm::vec3 position_;
  ObjModel *model_;
};

#endif
