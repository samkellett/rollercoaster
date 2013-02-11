#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "common.h"
#include "texture.h"
#include "vbo.h"

// Class for handling obj files
class ObjModel
{
public:
  ObjModel();
  bool load(std::string file, std::string material);
  void render();
  void release();

  int polygonCount();

private:
  bool loaded_;
  int attributes_;
  int faces_;
  VBO vbo_;
  UINT vao_;
  Texture texture_;

  bool loadMaterial(std::string filename);

};

#endif
