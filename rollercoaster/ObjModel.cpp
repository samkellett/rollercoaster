//#include "common_header.h"

#include "objModel.h"

CObjModel::CObjModel()
{
  m_bLoaded = false;
  m_iAttrBitField = 0;
}

/*-----------------------------------------------

Name:    split

Params:  s - string to split
     t - string to split according to

Result:  Splits string according to some substring
     and returns it as a vector.

/*---------------------------------------------*/

vector<string> split(string s, string t)
{
  vector<string> res;
  while(1)
  {
    int pos = s.find(t);
    if(pos == -1){res.push_back(s); break;}
    res.push_back(s.substr(0, pos));
//    s = s.substr(pos+1, ESZ(s)-pos-1);
    s = s.substr(pos+1, (int)s.size()-pos-1);
  }
  return res;

}

/*-----------------------------------------------

Name:    getDirectoryPath

Params:  sFilePath - file path

Result:  Returns path of a directory from file path.

/*---------------------------------------------*/

string getDirectoryPath(string sFilePath)
{
  // Get directory path
  string sDirectory = "";
  for (int i = (int) sFilePath.size()-1; i >= 0; i--) {
    if(sFilePath[i] == '\\' || sFilePath[i] == '/')
    {
      sDirectory = sFilePath.substr(0, i+1);
      break;
    }
  }
  return sDirectory;
}

/*-----------------------------------------------

Name:    loadModel

Params:  sFileName - full path mode file name
     sMtlFileName - relative path material file

Result:  Loads obj model.

/*---------------------------------------------*/

bool CObjModel::Load(string sFileName, string sMtlFileName)
{
  FILE* fp;
  fopen_s(&fp, sFileName.c_str(), "rt");

  if(fp == NULL) {
    char message[1024];
    sprintf_s(message, "Cannot load obj model\n%s\n", sFileName.c_str());
    MessageBox(NULL, message, "Error", MB_ICONERROR);
    return false;
  }

  char line[255];

  vector<glm::vec3> vVertices;
  vector<glm::vec2> vTexCoords;
  vector<glm::vec3> vNormals;

  m_iNumFaces = 0;

  while(fgets(line, 255, fp))
  {
    // Error flag, that can be set when something is inconsistent throughout
    // data parsing
    bool bError = false;

    // If it's an empty line, then skip
    if(strlen(line) <= 1)
      continue;

    // Now we're going to process line
    stringstream ss(line);
    string sType;
    ss >> sType;
    // If it's a comment, skip it
    if(sType == "#")
      continue;
    // Vertex
    else if(sType == "v")
    {
      glm::vec3 vNewVertex;
      int dim = 0;
      while(dim < 3 && ss >> vNewVertex[dim])dim++;
      vVertices.push_back(vNewVertex);
      m_iAttrBitField |= 1;
    }
    // Texture coordinate
    else if(sType == "vt")
    {
      glm::vec2 vNewCoord;
      int dim = 0;
      while(dim < 2 && ss >> vNewCoord[dim])dim++;
      vTexCoords.push_back(vNewCoord);
      m_iAttrBitField |= 2;
    }
    // Normal
    else if(sType == "vn")
    {
      glm::vec3 vNewNormal;
      int dim = 0;
      while(dim < 3 && ss >> vNewNormal[dim])dim++;
      vNewNormal = glm::normalize(vNewNormal);
      vNormals.push_back(vNewNormal);
      m_iAttrBitField |= 4;
    }
    // Face definition
    else if(sType == "f")
    {
      string sFaceData;
      // This will run for as many vertex definitions as the face has
      // (for triangle, it's 3)
      while(ss >> sFaceData)
      {
        vector<string> data = split(sFaceData, "/");
        int iVertIndex = -1, iTexCoordIndex = -1, iNormalIndex = -1;
      
        // If there were some vertices defined earlier
        if(m_iAttrBitField&1)
        {
          if((int)data[0].size() > 0)sscanf_s(data[0].c_str(), "%d", &iVertIndex);
          else bError = true;
        }
        // If there were some texture coordinates defined earlier
        if(m_iAttrBitField&2 && !bError)
        {
          if((int)data.size() >= 1)
          {
            // Just a check whether face format isn't f v//vn
            // In that case, data[1] is empty string
            if((int)data[1].size() > 0)sscanf_s(data[1].c_str(), "%d", &iTexCoordIndex);
            else bError = true;
          }
          else bError = true;
        }
        // If there were some normals defined defined earlier
        if(m_iAttrBitField&4 && !bError)
        {
          if((int)data.size() >= 2)
          {
            if((int)data[2].size() > 0)sscanf_s(data[2].c_str(), "%d", &iNormalIndex);
            else bError = true;
          }
          else bError = true;
        }
        if(bError)
        {
          fclose(fp);
          return false;
        }
      
        // Check whether vertex index is within boundaries (indexed from 1)
        if(iVertIndex > 0 && iVertIndex <= (int)vVertices.size())
          m_vboModelData.AddData(&vVertices[iVertIndex-1], sizeof(glm::vec3));
        if(iTexCoordIndex > 0 && iTexCoordIndex <= (int)vTexCoords.size())
          m_vboModelData.AddData(&vTexCoords[iTexCoordIndex-1], sizeof(glm::vec2));
        if(iNormalIndex > 0 && iNormalIndex <= (int)vNormals.size())
          m_vboModelData.AddData(&vNormals[iNormalIndex-1], sizeof(glm::vec3));
      }
      m_iNumFaces++;
    }
    // Shading model, for now just skip it
    else if(sType == "s")
    {
      // Do nothing for now
    }
    // Material specified, skip it again
    else if(sType == "usemtl")
    {
      // Do nothing for now
    }
  }

  fclose(fp);

  if(m_iAttrBitField == 0)
    return false;

  // Create VBO

  m_vboModelData.Create();
  m_vboModelData.Bind();

  m_vboModelData.UploadDataToGPU(GL_STATIC_DRAW);

  // Create one VAO

  glGenVertexArrays(1, &m_uiVAO); 
  glBindVertexArray(m_uiVAO);
  int iDataStride = 0;
  if(m_iAttrBitField&1)iDataStride += sizeof(glm::vec3);
  if(m_iAttrBitField&2)iDataStride += sizeof(glm::vec2);
  if(m_iAttrBitField&4)iDataStride += sizeof(glm::vec3);

  if(m_iAttrBitField&1)
  {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, iDataStride, 0);
  }
  // Texture coordinates
  if(m_iAttrBitField&2)
  {
    glEnableVertexAttribArray(1);
    int iDataOffset = 0;
    if(m_iAttrBitField&1)iDataOffset += sizeof(glm::vec3);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, iDataStride, (void*)iDataOffset);
  }
  // Normal vectors
  if(m_iAttrBitField&4)
  {
    glEnableVertexAttribArray(2);
    int iDataOffset = 0;
    if(m_iAttrBitField&1)iDataOffset += sizeof(glm::vec3);
    if(m_iAttrBitField&2)iDataOffset += sizeof(glm::vec2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, iDataStride, (void*)iDataOffset);
  }
  m_bLoaded = true;

  // Material should be in the same directory as model
  LoadMaterial(getDirectoryPath(sFileName)+sMtlFileName);

  return true;
}

/*-----------------------------------------------

Name:    renderModel

Params:  none

Result:  Guess what it does :)

/*---------------------------------------------*/

void CObjModel::Render()
{
  if(!m_bLoaded)
    return;
  glBindVertexArray(m_uiVAO);
  m_tAmbientTexture.Bind();
  glDrawArrays(GL_TRIANGLES, 0, m_iNumFaces*3);
}

/*-----------------------------------------------

Name:    loadMaterial

Params:  sFullMtlFileName - full path to material file

Result:  Loads material (currently only ambient
     texture).

/*---------------------------------------------*/

bool CObjModel::LoadMaterial(string sFullMtlFileName)
{
  // For now, we'll just look for ambient texture, i.e. line that begins with map_Ka
  FILE* fp;
  fopen_s(&fp, sFullMtlFileName.c_str(), "rt");

  if(fp == NULL)
    return false;

  char line[255];

  while(fgets(line, 255, fp))
  {
    stringstream ss(line);
    string sType;
    ss >> sType;
    if(sType == "map_Ka")
    {
      string sLine = line;
      // Take the rest of line as texture name, remove newline character from end
      int from = sLine.find("map_Ka")+6+1;
      string sTextureName = sLine.substr(from, (int)sLine.size()-from-1);
      // Texture should be in the same directory as material
      m_tAmbientTexture.Load(getDirectoryPath(sFullMtlFileName)+sTextureName, true);
      m_tAmbientTexture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_NEAREST_MIPMAP);
      break;
    }
  }
  fclose(fp);

  return true;
}

/*-----------------------------------------------

Name:    releaseModel

Params:  none

Result:  Frees all used resources by model.

/*---------------------------------------------*/

void CObjModel::Release()
{
  if(!m_bLoaded)
    return;
  m_tAmbientTexture.Release();
  glDeleteVertexArrays(1, &m_uiVAO);
  m_vboModelData.Release();
  m_bLoaded = false;
}

/*-----------------------------------------------

Name:    getPolygonCount

Params:  none

Result:  Returns model polygon count.

/*---------------------------------------------*/

int CObjModel::GetPolygonCount()
{
  return m_iNumFaces;
}