#include "terrain.h"

#include "common.h"

// Create the plane, including its geometry, texture mapping, normal, and colour
Terrain::Terrain() : GameObject(),
  directory_("resources/textures/"),
  filename_("grassfloor01.jpg"),
  width_(2000.0f),
  height_(2000.0f),
  texture_repeat_(50.0f)
{  
  // Load the texture
  texture_.load(directory_ + filename_, true);

  // Set parameters for texturing using sampler object
  texture_.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
  texture_.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texture_.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  // Use VAO to store state associated with vertices
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // Create a VBO
  vbo_.create();
  vbo_.bind();

  float half_width = width_ / 2.0f;
  float half_height = height_ / 2.0f;

  // Vertex positions
  glm::vec3 plane_vertices[4] = {
    glm::vec3(-half_width, 0.0f, -half_height), 
    glm::vec3(half_width, 0.0f, -half_height), 
    glm::vec3(-half_width, 0.0f, half_height), 
    glm::vec3(half_width, 0.0f, half_height), 
  };

  // Texture coordinates
  glm::vec2 texture_coords[4] = {
    glm::vec2(0.0f, 0.0f), 
    glm::vec2(texture_repeat_, 0.0f), 
    glm::vec2(0.0f, texture_repeat_), 
    glm::vec2(texture_repeat_, texture_repeat_)
  };

  // Plane normal
  glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

  // Put the vertex attributes in the VBO
  for (int i = 0; i < 4; ++i) {
    vbo_.addData(&plane_vertices[i], sizeof(glm::vec3));
    vbo_.addData(&texture_coords[i], sizeof(glm::vec2));
    vbo_.addData(&normal, sizeof(glm::vec3));
  }

  // Upload the VBO to the GPU
  vbo_.uploadDataToGPU(GL_STATIC_DRAW);

  // Set the vertex attribute locations
  GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

  // Texture coordinates
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) sizeof(glm::vec3));

  // Normal vectors
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (sizeof(glm::vec3) + sizeof(glm::vec2)));
}

// Release resources
Terrain::~Terrain()
{
  texture_.release();
  glDeleteVertexArrays(1, &vao_);
  vbo_.release();
}

void Terrain::update(glutil::MatrixStack &, double)
{
}

// Render the plane as a triangle strip
void Terrain::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  program->setUniform("matrices.modelViewMatrix", modelview.top());
  program->setUniform("matrices.normalMatrix", camera_->normalMatrix(modelview.top()));

  glBindVertexArray(vao_);
  texture_.bind();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
