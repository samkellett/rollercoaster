#version 330

in vec2 vTexCoord;
out vec4 vOutputColour;

uniform sampler2D gSampler;
uniform vec4 vColour;

void main()
{
	// Get the texel colour from the image
	vec4 vTexColour = texture(gSampler, vTexCoord);

	// The texel colour is a grayscale value -- apply to RGBA and combine with vColor
	vOutputColour = vec4(vTexColour.r) * vColour;
}
