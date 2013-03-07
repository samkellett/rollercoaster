#include "rollercoaster.h"

#include "camera.h"
#include "catmullrom.h"
#include "point.h"

Rollercoaster::Rollercoaster()
{
}

Rollercoaster::~Rollercoaster()
{
  while (!points_.empty()) {
    delete points_.back();
    points_.pop_back();
  }
}

glm::vec3 Rollercoaster::point(double d)
{
  float total_length = distances_[distances_.size() - 1];
  float length = d - (int) (d / total_length) * total_length;
  
  int segment;
  
	for (unsigned int i = 0; i < distances_.size(); i++) {
    if (length >= distances_[i] && length < distances_[i + 1]) {
      segment = i;
      break;
    }
  }
  
  float segment_length =  distances_[segment + 1] - distances_[segment];	
  float t = (length - distances_[segment]) / segment_length;	
  
  int p0 = ((segment - 1) + points_.size()) % points_.size();
  int p1 = segment;
  int p2 = (segment + 1) % points_.size();
  int p3 = (segment + 2) % points_.size();
  
  return CatmullRom::interpolate(points_.at(p0), points_.at(p0), points_.at(p0), points_.at(p0), t);
}

void Rollercoaster::buildPath()
{
  sampled_points_.clear(); 
  int n = 100;
  
  computeLength();
  float total_length = distances_[distances_.size() - 1];
  float spacing = total_length / n;
  
  for (int i = 0; i < n; i++) {
    sampled_points_.push_back(point(i * spacing));
  }
  
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  
  vbo_.create();
  vbo_.bind();
  
  glm::vec2 texture_coord(0.0f);
  glm::vec3 normal(0.0f, 1.0f, 0.0f);
  
  for (unsigned int i = 0; i < sampled_points_.size(); i++) {
    glm::vec3 v = sampled_points_[i];
    vbo_.addData(&v, sizeof(glm::vec3));
    vbo_.addData(&texture_coord, sizeof(glm::vec2));
    vbo_.addData(&normal, sizeof(glm::vec3));
  }
  
  vbo_.uploadDataToGPU(GL_STATIC_DRAW);
  
  GLsizei stride = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3);
  
  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
  
  // Texture coordinates
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *) sizeof(glm::vec3));
  
  // Normal vectors
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *) (sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void Rollercoaster::computeLength()
{
  distances_.clear();
  
  float length = 0.0f;
  distances_.push_back(length);
  
  for (unsigned int i = 1; i < points_.size(); i++) {
    Point *p1 = points_.at(i - 1);
    Point *p2 = points_.at(i);

    length += glm::distance(p1->position(), p2->position());
    distances_.push_back(length);
  }
  
  length += glm::distance(points_.back()->position(), points_.front()->position());
  distances_.push_back(length);
}

void Rollercoaster::keyboardHandler(double dt)
{
  if(GetKeyState(VK_SPACE) & 0x80) {
    Camera *camera = Game::instance().camera();
    Point *point = new Point(camera->position(), glm::vec3(1.0f));

    points_.push_back(point);
    if (points_.size() >= 4) {
      buildPath();
    }
  }
}

void Rollercoaster::update(glutil::MatrixStack &modelview, double dt)
{
}

void Rollercoaster::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  program->setUniform("texture", false);
  
  for (int i = 0; i < points_.size(); ++i) {
    Point *point = points_.at(i);

    modelview.push();
      points_.at(i)->render(modelview, program);
    modelview.pop();
  }
  
  program->setUniform("matrices.modelview", modelview.top());
  
  glBindVertexArray(vao_);
  
  glLineWidth(2.5f);
  glPointSize(5.0f);
  
  glDrawArrays(GL_LINE_LOOP, 0, sampled_points_.size());
  glDrawArrays(GL_POINTS, 0, sampled_points_.size());
}