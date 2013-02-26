#ifndef SKYBOX_H
#define SKYBOX_H

#include "gameobject.h"

#include "texture.h"
#include "vbo.h"

// This is a class for creating and rendering a skybox
class Skybox : public GameObject
{
public:
  Skybox();
  ~Skybox();
  
  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  UINT vao_;
  VBO vbo_;

  Texture textures_[6];
  float size_;
  std::string directory_;

  std::string front_;
  std::string back_;
  std::string left_;
  std::string right_;
  std::string top_;
  std::string bottom_;
};

#endif
