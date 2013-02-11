#include "shader.h"

#include "include/gl/glew.h"
#include <gl/gl.h>
#include "./include/glm/gtc/type_ptr.hpp"

Shader::Shader() :
  loaded_(false)
{
}

// Loads a shader, stored as a text file with filename sFile.  The shader is of type iType (vertex, fragment, geometry, etc.)
bool Shader::loadShader(std::string file, int type)
{
  std::vector<std::string> lines;

  if(!linesFromFile(file, false, &lines)) {
    char message[1024];
    sprintf_s(message, "Cannot load shader\n%s\n", file.c_str());
    MessageBox(NULL, message, "Error", MB_ICONERROR);

    return false;
  }

  const char **program = new const char *[(int) lines.size()];
  for (int i = 0; i < (int) lines.size(); ++i) {
    program[i] = lines[i].c_str();
  }
  
  id_ = glCreateShader(type);

  glShaderSource(id_, (int) lines.size(), program, NULL);
  glCompileShader(id_);

  delete[] program;

  int status;
  glGetShaderiv(id_, GL_COMPILE_STATUS, &status);

  if(status == GL_FALSE) {
    char log[1024];
    char message[1536];

    int log_length;
    glGetShaderInfoLog(id_, 1024, &log_length, log);
    char shader_type[64];

    if (type == GL_VERTEX_SHADER) {
      sprintf_s(shader_type, "vertex shader");
    } else if (type == GL_FRAGMENT_SHADER) {
      sprintf_s(shader_type, "fragment shader");
    } else if (type == GL_GEOMETRY_SHADER) {
      sprintf_s(shader_type, "geometry shader");
    } else {
      sprintf_s(shader_type, "unknown shader type");
    }

    sprintf_s(message, "Error in %s!\n%s\nShader file not compiled.  The compiler returned:\n\n%s", shader_type, file.c_str(), log);

    MessageBox(NULL, message, "Error", MB_ICONERROR);
    return false;
  }

  type_ = type;
  loaded_ = true;

  return true;
}


// Loads a file into a vector of std::strings (result)
bool Shader::linesFromFile(std::string filepath, bool include_part, std::vector<std::string>* result)
{
  FILE* file;
  fopen_s(&file, filepath.c_str(), "rt");
  if(!file) {
    return false;
  }

  std::string directory;
  int index = -1;

  for (int i = (int) filepath.size() - 1; i == 0; --i) {
    if(filepath[i] == '\\' || filepath[i] == '/')
    {
      index = i;
      break;
    }
  }

  directory = filepath.substr(0, index + 1);

  // Get all lines from a file
  char line[255];
  bool in_include_part = false;

  while(fgets(line, 255, file)) {
    std::stringstream ss(line);
    std::string first;
    ss >> first;
    if(first == "#include") {
      std::string filename;
      ss >> filename;

      if((int) filename.size() > 0 && filename[0] == '\"' && filename[(int) filename.size() - 1] == '\"') {
        filename = filename.substr(1, (int)filename.size() - 2);
        linesFromFile(directory + filename, true, result);
      }
    } else if(first == "#include_part") {
      in_include_part = true;
    } else if(first == "#definition_part") {
      in_include_part = false;
    } else if(!in_include_part || (include_part && in_include_part)) {
      result->push_back(line);
    }
  }

  fclose(file);

  return true;
}


// Returns true if the shader was loaded and compiled
bool Shader::loaded()
{
  return loaded_;
}

// Returns the ID of the shader
UINT Shader::shaderID()
{
  return id_;
}

// Deletes the shader and frees GPU memory
void Shader::deleteShader()
{
  if(!loaded()) {
    return;
  }

  loaded_ = false;
  glDeleteShader(id_);
}
