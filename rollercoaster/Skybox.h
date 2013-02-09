#ifndef SKYBOX_H
#define SKYBOX_H

#include "texture.h"
#include "vbo.h"

// This is a class for creating and rendering a skybox
class Skybox
{
public:
  void create(string directory, string front, string back, string left, string right, string top, string bottom, float size);
  void render();
  void release();

private:
  UINT vao_;
  VBO vbo_;
  Texture textures_[6];
  string directory_;

  string front_;
  string back_;
  string left_;
  string right_;
  string top_;
  string bottom_;
};

#endif
