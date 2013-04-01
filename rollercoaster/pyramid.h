#ifndef PYRAMID_H
#define PYRAMID_H

#include "gameobject.h"

class Pyramid : public GameObject
{
public:
  Pyramid(glm::vec3 position = glm::vec3(0), float scale = 1.0f);
  ~Pyramid();

  void init(ShaderProgram *);
  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  UINT vao_;
  VBO vbo_;

  Texture texture_;
  std::string directory_;
  std::string filename_;

  float width_;
  float height_;

  glm::vec3 position_;
  float scale_;
};


#endif