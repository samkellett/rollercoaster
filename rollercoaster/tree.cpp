#include "tree.h"

#include "objmodel.h"

Tree::Tree(std::string model, glm::vec3 position) :
  model_(new ObjModel("resources/models/" + model + ".obj", model + ".mtl")), position_(position)
{
}

Tree::~Tree()
{
}

std::string Tree::program()
{
  return "trees";
}

void Tree::init(ShaderProgram *program)
{
  GameObject::init(program);

  Game &game = Game::instance();
  position_ = glm::vec3(position_.x, game.height(position_), position_.z);
}

void Tree::update(glutil::MatrixStack &, double)
{
}

void Tree::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  modelview.translate(position_);

  Lighting::diffuseSpecular(modelview, program);
  program->setUniform("matrices.modelview", modelview.top());

  model_->render();

  glDisable(GL_BLEND);
}
