#include "tree.h"

#include <iostream>
#include <fstream>

#include "objmodel.h"
#include "resources/data/trees.h"

Tree::Tree(std::string model, glm::vec3 position) :
  model_(MODEL(model)), 
  position_(position),
  scale_(1.0f),
  rotation_(0.0f)
{
}

Tree::Tree(const data::tree &tree) :
  model_(MODEL(tree.model)),
  position_(tree.x, 0.0f, tree.z),
  scale_(tree.scale),
  rotation_(tree.rotation)
{
}

Tree::~Tree()
{
  delete model_;
}

std::string Tree::program()
{
  return "trees";
}

void Tree::init(ShaderProgram *program)
{
  GameObject::init(program);

  Game &game = Game::instance();
  position_ = glm::vec3(position_.x, game.height(position_) - 1.0f, position_.z);
}

void Tree::update(glutil::MatrixStack &, double)
{
}

void Tree::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  modelview.translate(position_);
  modelview.rotateY(rotation_);
  modelview.scale(scale_);

  Lighting::diffuseSpecular(program, 0.5f, 0.5f, 0.5f);
  program->setUniform("matrices.modelview", modelview.top());

  model_->render();

  glDisable(GL_BLEND);
}
