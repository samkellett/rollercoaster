#ifndef TERRAIN_H
#define TERRAIN_H

#include "gameobject.h"
#include "facevertexmesh.h"
#include "include\freeimage\FreeImage.h"

class Terrain : public GameObject
{
public:
	Terrain();
	~Terrain();

	float height(glm::vec3 point);

  void init(ShaderProgram *program);
  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

  std::string program();

private:
  char *heightmap_filename_;

  float min_height_;
  float max_height_;

	unsigned int width_;
  unsigned int height_;

	float *heightmap_;
	FaceVertexMesh mesh_;
	
  UINT height_texture_;

	float sizex_;
  float sizez_;
  float scaley_;

  std::vector<char *> texture_images_;
  std::vector<Texture> textures_;

	glm::vec3 origin_;
	FIBITMAP *dib_;

	glm::vec3 toImageCoordinates(glm::vec3 point);
	glm::vec3 toWorldCoordinates(glm::vec3 point);
	bool imageBytes(char *filename, BYTE **data, unsigned int &width, unsigned int &height);
};

#endif