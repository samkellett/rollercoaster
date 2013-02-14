#version 330

// Interpolated colour using colour calculated in the vertex shader
in vec3 vColour;

// Interpolated texture coordinate using texture coordinate from the vertex shader
in vec2 vTexCoord;

// The output colour
out vec4 vOutputColour;

// The texture sampler
uniform sampler2D gSampler;

// A flag indicating if texture-mapping should be applied
uniform bool bUseTexture;

void main()
{
	// Get the texel colour from the texture sampler
	vec4 vTexColour = texture(gSampler, vTexCoord);	

	if (bUseTexture) {
		// Combine object colour and texture
		vOutputColour = vTexColour * vec4(vColour, 1.0f);	 
	} else {
		// Just use the colour instead
		vOutputColour = vec4(vColour, 1.0f);
	}
}
