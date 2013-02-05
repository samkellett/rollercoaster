#include "Common.h"

#include "Plane.h"

// Create the plane, including its geometry, texture mapping, normal, and colour
void CPlane::Create(string sDirectory, string sFilename, float fWidth, float fHeight, float fTextureRepeat)
{
	
	m_fwidth = fWidth;
	m_fheight = fHeight;

	// Load the texture
	m_tTexture.Load(sDirectory+sFilename, true);

	m_sDirectory = sDirectory;
	m_sFilename = sFilename;

	// Set parameters for texturing using sampler object
	m_tTexture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	m_tTexture.SetSamplerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_tTexture.SetSamplerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);

	// Create a VBO
	m_vboRenderData.Create();
	m_vboRenderData.Bind();

	float fhalfWidth = m_fwidth / 2.0f;
	float fhalfHeight = m_fheight / 2.0f;

	// Vertex positions
	glm::vec3 vPlaneVertices[4] = 
	{
		glm::vec3(-fhalfWidth, 0.0f, -fhalfHeight), 
		glm::vec3(fhalfWidth, 0.0f, -fhalfHeight), 
		glm::vec3(-fhalfWidth, 0.0f, fhalfHeight), 
		glm::vec3(fhalfWidth, 0.0f, fhalfHeight), 
	};

	// Texture coordinates
	glm::vec2 vPlaneTexCoords[4] =
	{
		glm::vec2(0.0f, 0.0f), 
		glm::vec2(fTextureRepeat, 0.0f), 
		glm::vec2(0.0f, fTextureRepeat), 
		glm::vec2(fTextureRepeat, fTextureRepeat)
	};

	// Plane normal
	glm::vec3 vPlaneNormal = glm::vec3(0.0f, 1.0f, 0.0f);

	// Put the vertex attributes in the VBO
	for (int i = 0; i < 4; i++) {
		m_vboRenderData.AddData(&vPlaneVertices[i], sizeof(glm::vec3));
		m_vboRenderData.AddData(&vPlaneTexCoords[i], sizeof(glm::vec2));
		m_vboRenderData.AddData(&vPlaneNormal, sizeof(glm::vec3));
	}

	// Upload the VBO to the GPU
	m_vboRenderData.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei istride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
	
}

// Render the plane as a triangle strip
void CPlane::Render()
{
	glBindVertexArray(m_uiVAO);
	m_tTexture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
}

// Release resources
void CPlane::Release()
{
	m_tTexture.Release();
	glDeleteVertexArrays(1, &m_uiVAO);
	m_vboRenderData.Release();
}