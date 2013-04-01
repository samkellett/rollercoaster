#ifndef FACEVERTEXMESH_H
#define FACEVERTEXMESH_H

#include "common.h"
#include "vertex.h"

typedef struct {
	std::vector<unsigned int> id;
} TriangleList;

class FaceVertexMesh
{
public:
	FaceVertexMesh();
	~FaceVertexMesh();

  void createFromTriangleList(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &triangles);

	void render();
	
	void computeVertexNormals();
	glm::vec3 computeTriangleNormal(unsigned int texture_id);
	void computeTextureCoords(float x, float z);

private:
	std::vector<Vertex> vertices_;
	std::vector<unsigned int> triangles_;
	std::vector<TriangleList> on_triangle_;

	UINT vao_;
};

#endif