#include "builder.h"

#include "game.h"
#include "camera.h"
#include "catmullrom.h"
#include "vbo.h"

Builder::Builder()
{
  points_.push_back(glm::vec3(-500, 10, -200));
  points_.push_back(glm::vec3(0, 10, -200));
  points_.push_back(glm::vec3(0, 10, 200));
  points_.push_back(glm::vec3(-500, 10, 200));
}

Builder::~Builder() 
{
}

void Builder::keyboardListener(double)
{
  if(GetKeyState(VK_SPACE) & 0x80) {
    Camera *camera = Game::instance().camera();
    points_.push_back(camera->position());
  }
}

void Builder::update(glutil::MatrixStack &modelview, double dt)
{
  createPath();
  keyboardListener(dt);
}

void Builder::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  program->setUniform("textured", false);
  program->setUniform("matrices.modelview", modelview.top());

  glBindVertexArray(vao_);

  glLineWidth(5.0f);
  glDrawArrays(GL_LINE_STRIP, 0, 100 * points_.size());
}

void Builder::createPath()
{
  // Use VAO to store state associated with vertices
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // Create a VBO
  VBO vbo;
  vbo.create();
  vbo.bind();

  glm::vec2 texture_coord(0.0f, 0.0f);
  glm::vec3 normal(1.0f, 1.0f, 1.0f);
  
  for (unsigned int i = 0; i <= points_.size() - 4; ++i) {
    for (unsigned int j = 0; j < 100; ++j) {
      float t  = (float) j / 100.0f;
      glm::vec3 spline = CatmullRom::interpolate(points_[i], points_[i + 1], points_[i + 2], points_[i + 3], t);
      vbo.addData(&spline, sizeof(glm::vec3));
      vbo.addData(&texture_coord, sizeof(glm::vec2));
      vbo.addData(&normal, sizeof(glm::vec3));
    }
  }

  // Upload the VBO to the GPU
  vbo.uploadDataToGPU(GL_STATIC_DRAW);

  // Set the vertex attribute locations
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
