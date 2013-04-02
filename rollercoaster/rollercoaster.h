#ifndef ROLLERCOASTER_H
#define ROLLERCOASTER_H

#include "gameobject.h"
#include "frame.h"
#include "facevertexmesh.h"
#include "support.h"

class Point;
typedef std::vector<Point *> PointList;

class Rollercoaster : public GameObject
{
public:
  Rollercoaster();
  ~Rollercoaster();

  double t();
  glm::vec3 point();
  glm::vec3 point(double t);

  void addDerivative(glm::vec3 position);

  void init(ShaderProgram *program);
  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  static const int FACES;

  double t_;
  
//  UINT vao_;
//  VBO vbo_;
  FaceVertexMesh mesh_;

  PointList dirivatives_;
  std::vector<float> distances_;

  std::vector<Support *> supports_;
  std::vector<Frame> frames_;
};

#endif
