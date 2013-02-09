#ifndef SPHERE_H
#define SPHERE_H

#include "texture.h"
#include "vbo.h"

// Class for generating a unit sphere
class Sphere
{
public:
  void create(string directory, string front, int slices, int stacks);
  void render();
  void release();

private:
  UINT vao_;
  VBO vbo_;
  Texture texture_;
  string directory_;
  string filename_;
  int triangles_;
};

#endif
