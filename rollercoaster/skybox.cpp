#include "skybox.h"

#include "common.h"
#include "camera.h"

#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

// Create the skybox
// Skybox downloaded from http://www.alusion-fr.com/an1ffa1.htm
Skybox::Skybox() : GameObject(),
  directory_("resources/skyboxes/faesky02/"),
  front_("faesky02right.jpg"),
  back_("faesky02left.jpg"),
  left_("faesky02front.jpg"),
  right_("faesky02back.jpg"),
  top_("faesky02up.jpg"),
  bottom_("faesky02down.jpg"),
  size_(500.0f)
{
  textures_[0].load(directory_ + front_);
  textures_[1].load(directory_ + back_);
  textures_[2].load(directory_ + left_);
  textures_[3].load(directory_ + right_);
  textures_[4].load(directory_ + top_);
  textures_[5].load(directory_ + bottom_);
 
  for (int i = 0; i < 6; i++) {
    textures_[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
    textures_[i].setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    textures_[i].setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  vbo_.create();
  vbo_.bind();

  float s = size_;
  glm::vec3 vertices[24] = {
    glm::vec3(s, s, s), glm::vec3(s, -s, s), glm::vec3(-s, s, s), glm::vec3(-s, -s, s), // Front face
    glm::vec3(-s, s, -s), glm::vec3(-s, -s, -s), glm::vec3(s, s, -s), glm::vec3(s, -s, -s), // Back face
    glm::vec3(-s, s, s), glm::vec3(-s, -s, s), glm::vec3(-s, s, -s), glm::vec3(-s, -s, -s), // Left face
    glm::vec3(s, s, -s), glm::vec3(s, -s, -s), glm::vec3(s, s, s), glm::vec3(s, -s, s), // Right face
    glm::vec3(-s, s, -s), glm::vec3(s, s, -s), glm::vec3(-s, s, s), glm::vec3(s, s, s), // Top face
    glm::vec3(s, -s, -s), glm::vec3(-s, -s, -s), glm::vec3(s, -s, s), glm::vec3(-s, -s, s), // Bottom face
  };

  glm::vec2 texture_coords[4] = {
    glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
  };

  glm::vec3 normals[6] = {
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(-1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f)
  };

  glm::vec4 colour = glm::vec4(1, 1, 1, 1);

  for (int i = 0; i < 24; ++i) {
    vbo_.addData(&vertices[i], sizeof(glm::vec3));
    vbo_.addData(&texture_coords[i%4], sizeof(glm::vec2));
    vbo_.addData(&normals[i/4], sizeof(glm::vec3));
  }

  vbo_.uploadDataToGPU(GL_STATIC_DRAW);

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

Skybox::~Skybox()
{
  for (int i = 0; i < 6; ++i) {
    textures_[i].release();
  }

  glDeleteVertexArrays(1, &vao_);
  vbo_.release();
}

void Skybox::update(glutil::MatrixStack &modelview, double)
{
  // Translate the modelview matrix to the camera eye point so skybox stays centred around camera
  glm::vec3 eye = Game::instance().camera()->position();
  modelview.translate(eye);
}

// Render the skybox
void Skybox::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  program->setUniform("matrices.modelview", modelview.top());

  glDepthMask(0);
  glBindVertexArray(vao_);

  for (int i = 0; i < 6; ++i) {
    textures_[i].bind();
    glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
  }

  glDepthMask(1);
}
