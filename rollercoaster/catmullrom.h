#ifndef CATMULLROM_H
#define CATMULLROM_H

#include "common.h"

class CatmullRom
{
public:
  CatmullRom();
  ~CatmullRom();

  glm::vec3 interpolate(glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, glm::vec3 &p4, float time);
  void createPathGeometry(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3);

  void render();

private:
  unsigned int vao_;
};

#endif
