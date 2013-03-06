#version 330

in vec2 texture_coord;
out vec4 output_colour;

uniform sampler2D sampler;
uniform vec4 colour;

void main()
{
	// Get the texel colour from the image
	vec4 texture_colour = texture(sampler, texture_coord);

	// The texel colour is a grayscale value -- apply to RGBA and combine with vColor
	output_colour = vec4(texture_colour.r) * colour;
}
