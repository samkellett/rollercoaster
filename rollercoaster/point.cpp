#include "point.h"

#include "shaderprogram.h"

Point::Point() :
  sphere_(25, 25)
{
}

Point::Point(glm::vec3 position) :
  sphere_(25, 25), position_(position)
{
}

Point::Point(glm::vec3 position, glm::vec3 colour) :
  sphere_(25, 25), position_(position), colour_(colour)
{
}

Point::~Point()
{
}

void Point::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  program->setUniform("material.ambient", colour_);
  program->setUniform("matrices.modelview", modelview.top());
  
  modelview.translate(position_);
  sphere_.render();
}

void Point::setPosition(glm::vec3 position)
{
  position_ = position;
}

glm::vec3 Point::position() const
{
  return position_;
}

void Point::setColour(glm::vec3 colour)
{
  colour_ = colour;
}

glm::vec3 Point::colour() const
{
  return colour_;
}
