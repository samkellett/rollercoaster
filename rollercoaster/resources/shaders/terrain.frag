#version 330

uniform sampler2D sampler0, sampler1;
uniform float min_height, max_height;

in float height;
in vec3 colour;
in vec2 texture_coord;

out vec4 output_colour;

void main()
{
	float level = clamp(2 * (height - min_height) / (max_height - min_height), 0, 2);
	float cutoff = 0.8f;

	if (level < cutoff) {
		vec4 sand = texture(sampler0, texture_coord);
		output_colour = sand * vec4(colour, level / cutoff);
	} else {
		vec4 sand = texture(sampler0, texture_coord);
		vec4 grass = texture(sampler1, texture_coord);

		output_colour = mix(sand, grass, level - cutoff) * vec4(colour, 1.0f);
	}
}
