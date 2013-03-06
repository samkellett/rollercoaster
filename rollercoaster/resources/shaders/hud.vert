#version 330

// Structure for matrices
uniform struct Matrices
{
	mat4 projection;
	mat4 modelview;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 coord;

out vec2 texture_coord;

void main()
{
	// Transform the point
	gl_Position = matrices.projection * matrices.modelview * vec4(position, 0.0, 1.0);

	// Pass through the texture coord
	texture_coord = coord;
}