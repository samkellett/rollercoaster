#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "common.h"
#include "texture.h"
#include "vbo.h"

std::vector<std::string> split(std::string s, std::string t);
std::string getDirectoryPath(std::string path);

// Class for handling obj files
class ObjModel
{
public:
  ObjModel(std::string filename, std::string material);
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
