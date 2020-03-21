#include "shader.h"

#include "GL/glew.h"
#include <OpenGL/gl3.h>
#include "./include/glm/gtc/type_ptr.hpp"

const std::string Shader::DEFAULT_VERTEX_SHADER = "main.vert";
const std::string Shader::DEFAULT_FRAGMENT_SHADER = "main.frag";

Shader::Shader(std::string directory, std::string file, int type) :
  loaded_(false)
{
  name_ = file.substr(0, file.find("."));
  file = directory + file;

  std::vector<std::string> lines;
  if(!linesFromFile(file, false, &lines)) {
    // char message[1024];
    printf("Cannot load shader\n%s\n", file.c_str());
    // MessageBox(NULL, message, "Error", MB_ICONERROR);

    return;
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
    // char shader_type[64];
    std::string shader_type;

    if (type == GL_VERTEX_SHADER) {
      shader_type = "vertex shader";
    } else if (type == GL_FRAGMENT_SHADER) {
      shader_type = "fragment shader";
    } else if (type == GL_GEOMETRY_SHADER) {
      shader_type = "geometry shader";
    } else {
      shader_type = "unknown shader type";
    }

    printf("Error in %s!\n%s\nShader file not compiled.  The compiler returned:\n\n%s", shader_type.c_str(), file.c_str(), log);

    // MessageBox(NULL, message, "Error", MB_ICONERROR);
    return;
  }

  type_ = type;
  loaded_ = true;
}

// Loads a file into a vector of std::strings (result)
bool Shader::linesFromFile(std::string filepath, bool include_part, std::vector<std::string>* result)
{
  // FILE* file;
  // fopen_s(&file, filepath.c_str(), "rt");
  FILE *file = fopen(filepath.c_str(), "rt");
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

std::string Shader::name() const
{
  return name_;
}

int Shader::type() const
{
  return type_;
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
