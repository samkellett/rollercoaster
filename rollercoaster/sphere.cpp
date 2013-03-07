#include "sphere.h"

#include "common.h"

Sphere::Sphere()
{
  create(10, 10);
}

Sphere::Sphere(int slices, int stacks)
{
  create(slices, stacks);
}

// Create a unit sphere 
void Sphere::create(std::string directory, std::string filename, int slices, int stacks)
{
  // check if filename passed in -- if so, load texture
  texture_.load(directory + filename);

  directory_ = directory;
  filename_ = filename;

  texture_.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
  texture_.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texture_.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

  create(slices, stacks);
}

void Sphere::create(int slices, int stacks)
{  
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  vbo_.create();
  vbo_.bind();

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> texture_coords;
  std::vector<glm::vec3> normals;

  for (int i = 0; i < stacks; ++i) {
    float phi = (i / (float) stacks) * (float) M_PI;
    float next_phi;
    if (phi != stacks - 1) {
      next_phi = ((i + 1) / (float) stacks) * (float) M_PI;
    } else {
      next_phi = (float) M_PI;
    }

    for (int j = 0; j < slices; ++j) {
      float theta = (j / (float) slices) * 2 * (float) M_PI;
      float next_theta = (((j + 1) % slices) / (float) slices) * 2 * (float) M_PI;
      
      glm::vec3 v1 = glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
      glm::vec3 v2 = glm::vec3(cos(next_theta) * sin(phi), sin(next_theta) * sin(phi), cos(phi));
      glm::vec3 v3 = glm::vec3(cos(theta) * sin(next_phi), sin(theta) * sin(next_phi), cos(next_phi));
      glm::vec3 v4 = glm::vec3(cos(next_theta) * sin(next_phi), sin(next_theta) * sin(next_phi), cos(next_phi));
      
      glm::vec2 t1 = glm::vec2(j / (float) slices, i / (float) stacks);
      glm::vec2 t2 = glm::vec2((j + 1) / (float) slices, i / (float) stacks);
      glm::vec2 t3 = glm::vec2(j / (float) slices, (i + 1) / (float) stacks);
      glm::vec2 t4 = glm::vec2((j + 1) / (float) slices, (i + 1) / (float) stacks);

      vertices.push_back(v1);
      vertices.push_back(v4);
      vertices.push_back(v2);

      vertices.push_back(v1);
      vertices.push_back(v3);
      vertices.push_back(v4);

      normals.push_back(v1);
      normals.push_back(v4);
      normals.push_back(v2);

      normals.push_back(v1);
      normals.push_back(v3);
      normals.push_back(v4);

      texture_coords.push_back(t1);
      texture_coords.push_back(t4);
      texture_coords.push_back(t2);

      texture_coords.push_back(t1);
      texture_coords.push_back(t3);
      texture_coords.push_back(t4);
    }
  }


  for (int i = 0; i < (int) vertices.size(); ++i) {
    vbo_.addData(&vertices[i], sizeof(glm::vec3));
    vbo_.addData(&texture_coords[i], sizeof(glm::vec2));
    vbo_.addData(&normals[i], sizeof(glm::vec3));
  }
  triangles_ = vertices.size() /  3;
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

// Render the sphere as a set of triangles
void Sphere::render()
{
  glBindVertexArray(vao_);

  texture_.bind();
  glDrawArrays(GL_TRIANGLES, 0, triangles_ * 3);
}

// Release memory on the GPU 
void Sphere::release()
{
  texture_.release();
  glDeleteVertexArrays(1, &vao_);
  vbo_.release();
}