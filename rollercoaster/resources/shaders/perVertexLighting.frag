#version 330

// Interpolated colour using colour calculated in the vertex shader
in vec3 colour;

// Interpolated texture coordinate using texture coordinate from the vertex shader
in vec2 texture_coord;

// The output colour
out vec4 output_colour;

// The texture sampler
uniform sampler2D sampler;

// A flag indicating if texture-mapping should be applied
uniform bool textured;

void main()
{
	// Get the texel colour from the texture sampler
	vec4 texture_colour = texture(sampler, texture_coord);

	if (textured) {
		// Combine object colour and texture
		output_colour = texture_colour * vec4(colour, 1.0f);
	} else {
		// Just use the colour instead
		output_colour = vec4(colour, 1.0f);
	}
}
