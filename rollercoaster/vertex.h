#ifndef VERTEX_H
#define VERTEX_H

#include "common.h"

struct Vertex
{
	Vertex()
  {
  }

	Vertex(glm::vec3 p, glm::vec2 t, glm::vec3 n) :
    position(p), texture(t), normal(n)
	{
	}

	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;
};

#endif