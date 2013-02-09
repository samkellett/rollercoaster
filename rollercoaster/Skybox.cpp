#include "common.h"

#include "skybox.h"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

// Create a skybox of a given size with six textures
void Skybox::create(string directory, string front, string back, string left, string right, string top, string bottom, float size)
{
  textures_[0].load(directory + front);
  textures_[1].load(directory + back);
  textures_[2].load(directory + left);
  textures_[3].load(directory + right);
  textures_[4].load(directory + top);
  textures_[5].load(directory + bottom);

  directory_ = directory;

  front_ = front;
  back_ = back;
  left_ = left;
  right_ = right;
  top_ = top;
  bottom_ = bottom;
   
  for (int i = 0; i < 6; i++) {
    textures_[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
    textures_[i].setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    textures_[i].setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  vbo_.create();
  vbo_.bind();

  glm::vec3 vertices[24] = {
    glm::vec3(size, size, size), glm::vec3(size, -size, size), glm::vec3(-size, size, size), glm::vec3(-size, -size, size), // Front face
    glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size), glm::vec3(size, size, -size), glm::vec3(size, -size, -size), // Back face
    glm::vec3(-size, size, size), glm::vec3(-size, -size, size), glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size), // Left face
    glm::vec3(size, size, -size), glm::vec3(size, -size, -size), glm::vec3(size, size, size), glm::vec3(size, -size, size), // Right face
    glm::vec3(-size, size, -size), glm::vec3(size, size, -size), glm::vec3(-size, size, size), glm::vec3(size, size, size), // Top face
    glm::vec3(size, -size, -size), glm::vec3(-size, -size, -size), glm::vec3(size, -size, size), glm::vec3(-size, -size, size), // Bottom face
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

// Render the skybox
void Skybox::render()
{
  glDepthMask(0);
  glBindVertexArray(vao_);

  for (int i = 0; i < 6; ++i) {
    textures_[i].bind();
    glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
  }

  glDepthMask(1);
}

// Release the storage assocaited with the skybox
void Skybox::release()
{
  for (int i = 0; i < 6; ++i) {
    textures_[i].release();
  }

  glDeleteVertexArrays(1, &vao_);
  vbo_.release();
}
