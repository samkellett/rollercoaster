#version 330

in vec2 vTexCoord;
out vec4 vOutputColour;

uniform sampler2D gSampler;
uniform vec4 vColour;

void main()
{
	vec4 vTexColour = texture2D(gSampler, vTexCoord);	// Get the texel colour from the image
	vOutputColour = vec4(vTexColour.r) * vColour;			// The texel colour is a grayscale value -- apply to RGBA and combine with vColor
}