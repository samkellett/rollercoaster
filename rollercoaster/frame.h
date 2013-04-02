#ifndef FRAME_H
#define FRAME_H

#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

struct Frame
{
  glm::vec3 p, q, t, n, b;
  Frame(glm::vec3 p, glm::vec3 q) : 
    p(p), q(q), t(q - p), 
    n(glm::normalize(glm::cross(t, glm::vec3(0, 1, 0)))), 
    b(glm::normalize(glm::cross(n, t)))
  {
  }

  std::vector<glm::vec3> points;
  void generatePoints(int amount)
  {
    float angle = 360.0f / (float) amount;
    float scale = 0.5f;

    for (float i = 0; i < 360.0f; i += angle) {
      float rads = glm::radians(i);
      glm::vec3 c = n * cos(rads) + b * sin(rads);
      c *= scale;

      points.push_back(p + c);
    }
  }
};

#endif