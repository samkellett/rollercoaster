#include "catmullrom.h"

#include "point.h"
#include "vbo.h"

glm::vec3 CatmullRom::interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
	float t2 = t * t;
  float t3 = t2 * t;

  glm::vec3 a = p1;
  glm::vec3 b = 0.5f * (-p0 + p2);
  glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
  glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

  return a + b*t + c*t2 + d*t3;
}

glm::vec3 CatmullRom::interpolate(Point *p0, Point *p1, Point *p2, Point *p3, float time)
{
  return interpolate(p0->position(), p1->position(), p2->position(), p3->position(), time);
}
