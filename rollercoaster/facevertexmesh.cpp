#include "facevertexmesh.h"

FaceVertexMesh::FaceVertexMesh()
{
}

FaceVertexMesh::~FaceVertexMesh()
{
}

// Compute the normal of a triangle using the cross product
glm::vec3 FaceVertexMesh::computeTriangleNormal(unsigned int triangle_id)
{
	Vertex v0, v1, v2;
	glm::vec3 normal, p, q;
	
	v0 = vertices_[triangles_[3 * triangle_id]];
	v1 = vertices_[triangles_[3 * triangle_id + 1]];
	v2 = vertices_[triangles_[3 * triangle_id + 2]];

	p = v1.position - v0.position;
	q = v2.position - v0.position;
	normal = glm::normalize(glm::cross(p, q)); 

	return normal;
}


void FaceVertexMesh::computeTextureCoords(float x, float z)
{
	// Set texture coords based on the x and z coordinates
	for (unsigned int i = 0; i < vertices_.size(); i++) {
		vertices_[i].texture.s = vertices_[i].position.x / x;
		vertices_[i].texture.t = vertices_[i].position.z / z;
	}
}

void FaceVertexMesh::computeVertexNormals()
{
	for (unsigned int i = 0; i < vertices_.size(); i++) {
		Vertex v = vertices_[i];
		
		// Get normal for each triangle around v in one ring neighbourhood
		glm::vec3 normal = glm::vec3(0, 0, 0);
		int faces = 0;

		for (unsigned int j = 0; j < on_triangle_[i].id.size(); ++j) {
			int triangle_id = on_triangle_[i].id[j];
			normal += computeTriangleNormal(triangle_id);
			++faces;
		}

		normal = glm::normalize(normal / (float) faces);
		vertices_[i].normal = normal;
	}
}

void FaceVertexMesh::createFromTriangleList(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &triangles)
{
	// Set the vertices and indices
	vertices_ = vertices;
	triangles_ = triangles;
	
	// Now we must fill the onTriangle list
	on_triangle_.resize(vertices_.size());
	unsigned int count = (unsigned int) (triangles.size() / 3);

	for (unsigned int t = 0; t < count; t++) {
		on_triangle_[triangles_[t * 3]].id.push_back(t);
		on_triangle_[triangles_[t * 3 + 1]].id.push_back(t);
		on_triangle_[triangles_[t * 3 + 2]].id.push_back(t);
	}

	// Compute vertex normals and texture coords
	computeVertexNormals();
	computeTextureCoords(20.0f, 20.0f);
	
	// Create a VAO 
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// Create a VBO for the vertex data
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);

	// Fill the vertices VBO
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

	// Generate a VGO for the indices and bind it
	GLuint vbo_indices_;
	glGenBuffers(1, &vbo_indices_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices_);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles_.size() * sizeof(GLuint), &triangles_[0], GL_STATIC_DRAW);
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

void FaceVertexMesh::render()
{
	glBindVertexArray(vao_);

	// Draw
	glDrawElements(GL_TRIANGLES, triangles_.size(), GL_UNSIGNED_INT, (char *) NULL + 0);
}
