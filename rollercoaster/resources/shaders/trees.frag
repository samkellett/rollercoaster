#version 330

in vec3 colour;
in vec2 texture_coord;

out vec4 output_colour;
uniform sampler2D sampler0;

void main()
{
	// Get the texel colour from the texture sampler
	vec4 texture_colour = texture(sampler0, texture_coord);

	
	// Alpha mask out the black:
	float cutoff = distance(texture_colour.rgb, vec3(1.0));
	if (cutoff < 0.75) {
		discard;
	}

	output_colour = texture_colour * vec4(colour, 1.0f);
}
