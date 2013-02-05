#pragma once

#include "Texture.h"
#include "vertexBufferObject.h"
#include "./include/glm/gtc/type_ptr.hpp"

// Class for generating a xz plane of a given size
class CPlane
{
public:
	void Create(string sDirectory, string sFilename, float fWidth, float fHeight, float fTextureRepeat);
	void Render();
	void Release();
private:
	UINT m_uiVAO;
	CVertexBufferObject m_vboRenderData;
	CTexture m_tTexture;
	string m_sDirectory;
	string m_sFilename;
	float m_fwidth;
	float m_fheight;
};