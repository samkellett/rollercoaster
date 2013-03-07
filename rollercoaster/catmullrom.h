#ifndef CATMULLROM_H
#define CATMULLROM_H

#include "common.h"

class Point;

class CatmullRom
{
public:
  static glm::vec3 interpolate(Point *p0, Point *p1, Point *p2, Point *p3, float time);
  static glm::vec3 interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float time);
};

#endif
