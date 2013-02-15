#ifndef SKYBOX_H
#define SKYBOX_H

#include "texture.h"
#include "vbo.h"

// This is a class for creating and rendering a skybox
class Skybox
{
public:
  void create(std::string directory, std::string front, std::string back, std::string left, std::string right, std::string top, std::string bottom, float size);
  void render();
  void release();

private:
  UINT vao_;
  VBO vbo_;
  Texture textures_[6];
  std::string directory_;

  std::string front_;
  std::string back_;
  std::string left_;
  std::string right_;
  std::string top_;
  std::string bottom_;
};

#endif
