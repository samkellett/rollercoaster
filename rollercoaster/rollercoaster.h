#ifndef ROLLERCOASTER_H
#define ROLLERCOASTER_H

#include "gameobject.h"

class Point;

typedef std::vector<Point *> PointList;

class Rollercoaster : public GameObject
{
public:
  Rollercoaster();
  ~Rollercoaster();

  glm::vec3 point(double t);
  
  void keyboardHandler(double dt);

  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  void buildPath();
  void computeLength();
  
  UINT vao_;
  VBO vbo_;
  
  PointList points_;
  std::vector<float> distances_;
  std::vector<glm::vec3> sampled_points_;
};

#endif
