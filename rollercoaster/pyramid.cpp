#include "pyramid.h"

// Create the plane, including its geometry, texture mapping, normal, and colour
Pyramid::Pyramid(glm::vec3 position, float scale) : GameObject(),
  position_(position), scale_(scale),

  directory_("resources/textures/"),
  filename_("pyramid.jpg"),
  width_(10.0f),
  height_(7.5f)
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

  glm::vec3 a = glm::vec3(0.0f, height_, 0.0f);
	glm::vec3 b = glm::vec3(half_width, 0.0f, -half_width);
	glm::vec3 c = glm::vec3(-half_width, 0.0f, -half_width);
	glm::vec3 d = glm::vec3(half_width, 0.0f, half_width);
	glm::vec3 e = glm::vec3(-half_width, 0.0f, half_width);

  // Vertex positions
  glm::vec3 plane_vertices[12] = {
    a, b, c,
    a, d, b,
    a, e, d,
    a, c, e
  };

  // Texture coordinates
  glm::vec2 texture_coords[3] = {
    glm::vec2(0.5f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f)
  };

  glm::vec3 n1 = glm::normalize(glm::cross(c - a, b - a));
  glm::vec3 n2 = glm::normalize(glm::cross(b - a, d - a));
  glm::vec3 n3 = glm::normalize(glm::cross(d - a, e - a));
  glm::vec3 n4 = glm::normalize(glm::cross(e - a, c - a));

  glm::vec3 na = (n1 + n2 + n3 + n4) / 4.0f;
  glm::vec3 nb = (n1 + n2) / 2.0f;
  glm::vec3 nc = (n1 + n4) / 2.0f;
  glm::vec3 nd = (n2 + n3) / 2.0f;
  glm::vec3 ne = (n3 + n4) / 2.0f;

  // Plane normal
  glm::vec3 normals[12] = {
    na, nb, nc,
    na, nd, nb,
    na, ne, nd,
    na, nc, ne
  };


  // Put the vertex attributes in the VBO
  for (int i = 0; i < 12; ++i) {
    vbo_.addData(&plane_vertices[i], sizeof(glm::vec3));
    vbo_.addData(&texture_coords[i % 3], sizeof(glm::vec2));
    vbo_.addData(&normals[i], sizeof(glm::vec3));
  }

  // Upload the VBO to the GPU
  vbo_.uploadDataToGPU(GL_STATIC_DRAW);

  // Set the vertex attribute locations
  GLsizei stride = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3);

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
Pyramid::~Pyramid()
{
  texture_.release();
  glDeleteVertexArrays(1, &vao_);
  vbo_.release();
}

void Pyramid::init(ShaderProgram *)
{
  Game &game = Game::instance();
  position_ = glm::vec3(position_.x, game.height(position_), position_.z);
}

void Pyramid::update(glutil::MatrixStack &, double)
{
}

// Render the plane as a triangle strip
void Pyramid::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  modelview.scale(scale_);
  modelview.translate(position_);

  Lighting::diffuseSpecular(modelview, program);
  program->setUniform("matrices.modelview", modelview.top());

  glBindVertexArray(vao_);
  texture_.bind();
  glDrawArrays(GL_TRIANGLES, 0, 12);
}
