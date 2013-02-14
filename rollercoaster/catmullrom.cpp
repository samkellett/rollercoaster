#include "catmullrom.h"

#include "vbo.h"

CatmullRom::CatmullRom()
{
}

CatmullRom::~CatmullRom()
{
}

glm::vec3 CatmullRom::interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
  glm::vec3 a = p1;
  glm::vec3 b = 0.5f * (-p0 + p2);
  glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
  glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

  return a + b*t + c*t*t + d*t*t*t;
}

void CatmullRom::createPathGeometry(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3)
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
  
  for (unsigned int i = 0; i < 100; ++i) {
    float t  = (float) i / 100.0f;
    glm::vec3 spline = interpolate(p0, p1, p2, p3, t);
    vbo.addData(&spline, sizeof(glm::vec3));
    vbo.addData(&texture_coord, sizeof(glm::vec2));
    vbo.addData(&normal, sizeof(glm::vec3));
  }
  // Upload the VBO to the GPU
  vbo.uploadDataToGPU(GL_STATIC_DRAW);

  // Set the vertex attribute locations
  GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

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

void CatmullRom::render()
{
  glBindVertexArray(vao_);

  glDrawArrays(GL_LINE_STRIP, 0, 100);
}
