#version 330

in vec3 colour;
in vec2 texture_coord;

out vec4 output_colour;

uniform sampler2D sampler0;
uniform bool not_textured;

void main()
{
	// Get the texel colour from the texture sampler
	vec4 texture_colour = texture(sampler0, texture_coord);

	// Combine object colour and texture
	output_colour = texture_colour * vec4(colour, 1.0f);
}
