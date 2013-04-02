#include "support.h"

#include "game.h"
#include "lighting.h"
#include "shaderprogram.h"

Support::Support(glm::vec3 position) : 
  position_(position)
{
  float height = Game::instance().height(position_);
  glm::vec3 ground = glm::vec3(position_.x, height, position_.z);

  float angle = 360.0f / 8;
  float scale = 0.4f;

  std::vector<glm::vec3> top;
  std::vector<glm::vec3> bottom;

  for (float i = 0; i < 360.0f; i += angle) {
    float rads = glm::radians(i);
    glm::vec3 c = glm::vec3(1, 0, 0) * cos(rads) + glm::vec3(0, 0, 1) * sin(rads);
    c *= scale;

    top.push_back(position_ + c);
    bottom.push_back(ground + c);
  }

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  top.push_back(top.front());
  bottom.push_back(bottom.front());

  vbo_.create();
  vbo_.bind();
   
  glm::vec2 texture_coord(0.0f);
  glm::vec3 normal(0.0f, 1.0f, 0.0f);

  for (unsigned int i = 0; i < top.size(); i++) {
    vbo_.addData(&top[i], sizeof(glm::vec3));
    vbo_.addData(&texture_coord, sizeof(glm::vec2));
    vbo_.addData(&normal, sizeof(glm::vec3));

    vbo_.addData(&bottom[i], sizeof(glm::vec3));
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

Support::~Support()
{
}

void Support::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  Lighting::diffuseSpecular(program, 0.7f, 1.0f, 0.9f);
  program->setUniform("matrices.modelview", modelview.top());
  program->setUniform("materials.ambient", glm::vec3(1, 1, 0));

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
}