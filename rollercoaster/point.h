#ifndef POINT_H
#define POINT_H

#include "matrixstack.h"
#include "sphere.h"

#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

class ShaderProgram;

class Point
{
public:
  Point();
  Point(glm::vec3 position);
  Point(glm::vec3 position, glm::vec3 colour);
  ~Point();

  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

  void setPosition(glm::vec3 position);
  glm::vec3 position() const;

  void setColour(glm::vec3 colour);
  glm::vec3 colour() const;

private:
  Sphere sphere_;

  glm::vec3 position_;
  glm::vec3 colour_;
};

#endif
