#include "cart.h"

#include "objmodel.h"
#include "camera.h"
#include "game.h"

Cart::Cart() :
  model_(MODEL("carriage")),
  scale_(1.0f),
  frame_(glm::vec3(0), glm::vec3(0))
{
}

Cart::~Cart()
{
  delete model_;
}

void Cart::update(glutil::MatrixStack &modelview, double dt)
{
  int count = 40;
  glm::vec3 p_average;
  glm::vec3 q_average;

  for (int i = 0; i < count; ++i) {
    p_average += Game::instance().futurePoint(i);
    q_average += Game::instance().futurePoint(1 + i);
  }

  glm::vec3 p = p_average / (float) count;
  glm::vec3 q = q_average / (float) count;
  frame_ = Frame(p, q);

  if (Game::instance().camera()->state() == Camera::BIRD) {
    scale_ = 8.0f;
  } else {
    scale_ = 1.0f;
  }
}

void Cart::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  modelview.translate(frame_.p + 1.5f * frame_.b);
  modelview *= glm::mat4(glm::mat3(frame_.t, frame_.b, frame_.n));
  modelview.scale(scale_);

  Lighting::diffuseSpecular(program);
  program->setUniform("matrices.modelview", modelview.top());

  model_->render();
}
