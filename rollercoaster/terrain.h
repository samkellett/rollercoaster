#ifndef TERRAIN_H
#define TERRAIN_H

#include "gameobject.h"

#include "texture.h"
#include "vbo.h"

#include "./include/glm/gtc/type_ptr.hpp"

// Create the planar terrain
// Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
// Class for generating a xz plane of a given size
class Terrain : public GameObject
{
public:
  Terrain();
  ~Terrain();

  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  UINT vao_;
  VBO vbo_;

  Texture texture_;
  std::string directory_;
  std::string filename_;

  float width_;
  float height_;
  float texture_repeat_;
};

#endif
