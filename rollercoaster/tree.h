#ifndef TREE_H
#define TREE_H

#include "gameobject.h"

class ObjModel;
namespace data {
  struct tree;
}

class Tree : public GameObject
{
public:
  Tree(std::string model, glm::vec3 position = glm::vec3(0.0f));
  Tree(const data::tree &tree);
  ~Tree();

  std::string program();

  void init(ShaderProgram *program);
  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  glm::vec3 position_;
  float scale_;
  float rotation_;

  ObjModel *model_;
};

#endif
