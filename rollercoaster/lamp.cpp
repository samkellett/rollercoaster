#include "lamp.h"

#include "objmodel.h"

Lamp::Lamp(glm::vec2 position, glm::vec3 colour) :
  model_(MODEL("lamp")),
  position_(position.x, 0.0f, position.y),
  scale_(35.0f),
  colour_(colour)
{
}

Lamp::~Lamp()
{
  delete model_;
}

void Lamp::init(ShaderProgram *program)
{
  GameObject::init(program);

  Game &game = Game::instance();
  position_ = glm::vec3(position_.x, game.height(position_) - 1.0f, position_.z);

  Lighting::registerSource(glm::vec3(position_.x, position_.y + 48.0f, position_.z), colour_);
}

void Lamp::update(glutil::MatrixStack &modelview, double dt)
{
}

void Lamp::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  modelview.push();
    modelview.translate(position_.x, position_.y + 43.0f, position_.z);
    modelview.scale(1.5f);

    program->setUniform("matrices.modelview", modelview.top());
    program->setUniform("not_textured", true);

    program->setUniform("material.ambient", colour_);
    program->setUniform("material.diffuse", glm::vec3(0.0f));
    program->setUniform("material.specular", glm::vec3(1.0f));
    
    bulb_.render();
  modelview.pop();

  modelview.translate(position_);
  modelview.scale(scale_);

  Lighting::diffuseSpecular(program, 1.0f, 1.0f, 0.8f);
  program->setUniform("matrices.modelview", modelview.top());
  program->setUniform("not_textured", false);

  model_->render();
}
