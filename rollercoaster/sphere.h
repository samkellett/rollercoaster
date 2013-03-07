#ifndef SPHERE_H
#define SPHERE_H

#include "texture.h"
#include "vbo.h"

// Class for generating a unit sphere
class Sphere
{
public:
  Sphere();
  Sphere(int slices, int stacks);

  void create(int slices, int stacks);
  void create(std::string directory, std::string front, int slices, int stacks);
  
  void render();
  void release();

private:
  UINT vao_;
  VBO vbo_;
  Texture texture_;
  std::string directory_;
  std::string filename_;
  int triangles_;
};

#endif
