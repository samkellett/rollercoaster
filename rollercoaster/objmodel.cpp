#include "objmodel.h"

ObjModel::ObjModel() :
  loaded_(false), attributes_(0)
{
}

/*-----------------------------------------------
  Name:    split
  Params:  s - std::string to split
       t - std::string to split according to
  Result:  Splits std::string according to some substd::string
       and returns it as a vector.
/*---------------------------------------------*/
std::vector<std::string> split(std::string s, std::string t)
{
  std::vector<std::string> res;
  while(true) {
    int pos = s.find(t);
    if(pos == -1){
      res.push_back(s); 
      break;
    }
    
    res.push_back(s.substr(0, pos));
//  s = s.substr(pos + 1, ESZ(s) - pos - 1);
    s = s.substr(pos + 1, (int) s.size() - pos - 1);
  }

  return res;
}

/*-----------------------------------------------
  Name:    getDirectoryPath
  Params:  sFilePath - file path
  Result:  Returns path of a directory from file path.
/*---------------------------------------------*/
std::string getDirectoryPath(std::string path)
{
  // Get directory path
  std::string directory = "";
  for (int i = (int) path.size() - 1; i >= 0; --i) {
    if(path[i] == '\\' || path[i] == '/') {
      directory = path.substr(0, i + 1);
      break;
    }
  }

  return directory;
}

/*-----------------------------------------------
  Name:    loadModel
  Params:  sFileName - full path mode file name
       sMtlFileName - relative path material file
  Result:  Loads obj model.
/*---------------------------------------------*/
bool ObjModel::load(std::string filename, std::string material)
{
  FILE* file;
  fopen_s(&file, filename.c_str(), "rt");

  if(file == NULL) {
    char message[1024];
    sprintf_s(message, "Cannot load obj model\n%s\n", filename.c_str());
    MessageBox(NULL, message, "Error", MB_ICONERROR);

    return false;
  }

  char line[255];

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> texture_coords;
  std::vector<glm::vec3> normals;

  faces_ = 0;

  while(fgets(line, 255, file)) {
    // Error flag, that can be set when something is inconsistent throughout data parsing
    bool error = false;

    // If it's an empty line, then skip
    if(strlen(line) <= 1) {
      continue;
    }

    // Now we're going to process line
    std::stringstream ss(line);
    std::string type;
    ss >> type;
    
    if(type == "#") { // If it's a comment, skip it
      continue;
    } else if(type == "v") { // Vertex
      glm::vec3 vNewVertex;
      int dim = 0;
      while(dim < 3 && ss >> vNewVertex[dim])dim++;
      vertices.push_back(vNewVertex);
      attributes_ |= 1;
    } else if(type == "vt") { // Texture coordinate
      glm::vec2 new_coord;
      int dim = 0;

      while(dim < 2 && ss >> new_coord[dim]) {
        dim++;
      }
      texture_coords.push_back(new_coord);
      attributes_ |= 2;
    } else if(type == "vn") { // Normal
      glm::vec3 new_normal;
      int dim = 0;

      while(dim < 3 && ss >> new_normal[dim]) {
        dim++;
      }
      new_normal = glm::normalize(new_normal);
      normals.push_back(new_normal);
      attributes_ |= 4;
    } else if(type == "f") { // Face definition
      std::string face_data;
      // This will run for as many vertex definitions as the face has
      // (for triangle, it's 3)
      while(ss >> face_data) {
        std::vector<std::string> data = split(face_data, "/");
        int vertex_index = -1;
        int texture_index = -1;
        int normal_index = -1;
      
        // If there were some vertices defined earlier
        if(attributes_ & 1) {
          if((int) data[0].size() > 0) {
            sscanf_s(data[0].c_str(), "%d", &vertex_index);
          } else {
            error = true;
          }
        }

        // If there were some texture coordinates defined earlier
        if(attributes_ & 2 && !error) {
          if((int) data.size() >= 1) {
            // Just a check whether face format isn't f v//vn
            // In that case, data[1] is empty std::string
            if((int) data[1].size() > 0) {
              sscanf_s(data[1].c_str(), "%d", &texture_index);
            } else {
              error = true;
            }
          } else {
            error = true;
          }
        }

        // If there were some normals defined defined earlier
        if(attributes_ & 4 && !error) {
          if((int) data.size() >= 2) {
            if((int) data[2].size() > 0) {
              sscanf_s(data[2].c_str(), "%d", &normal_index);
            } else {
              error = true;
            }
          } else {
            error = true;
          }
        }

        if(error) {
          fclose(file);
          return false;
        }
      
        // Check whether vertex index is within boundaries (indexed from 1)
        if(vertex_index > 0 && vertex_index <= (int) vertices.size()) {
          vbo_.addData(&vertices[vertex_index - 1], sizeof(glm::vec3));
        }
        if(texture_index > 0 && texture_index <= (int) texture_coords.size()) {
          vbo_.addData(&texture_coords[texture_index - 1], sizeof(glm::vec2));
        }
        if(normal_index > 0 && normal_index <= (int) normals.size()) {
          vbo_.addData(&normals[normal_index - 1], sizeof(glm::vec3));
        }
      }

      ++faces_;
    } else if(type == "s") { // Shading model, for now just skip it
      // Do nothing for now
    } else if(type == "usemtl") { // Material specified, skip it again
      // Do nothing for now
    }
  }

  fclose(file);

  if(attributes_ == 0) {
    return false;
  }

  // Create VBO
  vbo_.create();
  vbo_.bind();
  vbo_.uploadDataToGPU(GL_STATIC_DRAW);

  // Create one VAO
  glGenVertexArrays(1, &vao_); 
  glBindVertexArray(vao_);

  int stride = 0;
  if(attributes_ & 1) {
    stride += sizeof(glm::vec3);
  }
  if(attributes_ & 2) {
    stride += sizeof(glm::vec2);
  }
  if(attributes_ & 4) {
    stride += sizeof(glm::vec3);
  }

  if(attributes_ & 1) {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
  }

  // Texture coordinates
  if(attributes_ & 2) {
    glEnableVertexAttribArray(1);
    
    int offset = 0;
    if(attributes_ & 1) {
      offset += sizeof(glm::vec3);
    }

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) offset);
  }

  // Normal vectors
  if(attributes_ & 4) {
    glEnableVertexAttribArray(2);

    int offset = 0;
    if(attributes_ & 1) {
      offset += sizeof(glm::vec3);
    }
    if(attributes_ & 2) {
      offset += sizeof(glm::vec2);
    }

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) offset);
  }

  loaded_ = true;

  // Material should be in the same directory as model
  loadMaterial(getDirectoryPath(filename) + material);

  return true;
}

/*-----------------------------------------------
  Name:    renderModel
  Params:  none
  Result:  Guess what it does :)
/*---------------------------------------------*/
void ObjModel::render()
{
  if(!loaded_) {
    return;
  }

  glBindVertexArray(vao_);
  texture_.bind();
  glDrawArrays(GL_TRIANGLES, 0, faces_ * 3);
}

/*-----------------------------------------------
  Name:    loadMaterial
  Params:  sFullMtlFileName - full path to material file
  Result:  Loads material (currently only ambient
       texture).
/*---------------------------------------------*/
bool ObjModel::loadMaterial(std::string material)
{
  // For now, we'll just look for ambient texture, i.e. line that begins with map_Ka
  FILE* file;
  fopen_s(&file, material.c_str(), "rt");

  if(file == NULL) {
    return false;
  }

  char lines[255];
  while(fgets(lines, 255, file)) {
    std::stringstream ss(lines);
    std::string type;
    ss >> type;
    if(type == "map_Ka") {
      std::string line = lines;

      // Take the rest of line as texture name, remove newline character from end
      int from = line.find("map_Ka") + 6 + 1;
      std::string texture = line.substr(from, (int) line.size() - from - 1);

      // Texture should be in the same directory as material
      texture_.load(getDirectoryPath(material) + texture, true);
      texture_.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_NEAREST_MIPMAP);

      break;
    }
  }

  fclose(file);

  return true;
}

/*-----------------------------------------------
  Name:    releaseModel
  Params:  none
  Result:  Frees all used resources by model.
/*---------------------------------------------*/
void ObjModel::release()
{
  if(!loaded_) {
    return;
  }

  texture_.release();
  glDeleteVertexArrays(1, &vao_);
  vbo_.release();
  loaded_ = false;
}

/*-----------------------------------------------
  Name:    getPolygonCount
  Params:  none
  Result:  Returns model polygon count.
/*---------------------------------------------*/
int ObjModel::polygonCount()
{
  return faces_;
}