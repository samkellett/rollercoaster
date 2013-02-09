#ifndef PLANE_H
#define PLANE_H

#include "texture.h"
#include "vbo.h"

#include "./include/glm/gtc/type_ptr.hpp"

// Class for generating a xz plane of a given size
class Plane
{
public:
  void create(string directory, string filename, float width, float height, float texture_repeat);
  void render();
  void release();

private:
  UINT vao_;
  VBO vbo_;
  Texture texture_;
  string directory_;
  string filename_;
  float width_;
  float height_;
};

#endif
