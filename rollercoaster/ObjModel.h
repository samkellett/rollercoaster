#pragma once

#include "Common.h"
#include "Texture.h"
#include "VertexBufferObject.h"

// Class for handling obj files
class CObjModel
{
public:
	CObjModel();
	bool Load(string sFileName, string sMtlFileName);
	void Render();
	void Release();

	int GetPolygonCount();

private:
	bool m_bLoaded;
	int m_iAttrBitField;
	int m_iNumFaces;
	CVertexBufferObject m_vboModelData;
	UINT m_uiVAO;
	CTexture m_tAmbientTexture;

	bool LoadMaterial(string sFullMtlFileName);

};