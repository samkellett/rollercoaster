#include "shaderprogram.h"

#include "shader.h"

#include "include/gl/glew.h"
#include <gl/gl.h>
#include "./include/glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram()
{
  linked_ = false;
}

// Creates a new shader program
void ShaderProgram::create()
{
  id_ = glCreateProgram();
}

// Adds a compiled shader to a program
bool ShaderProgram::addShader(Shader* shader)
{
  if(!shader->loaded()) {
    return false;
  }

  glAttachShader(id_, shader->shaderID());
  return true;
}

// Performs final linkage of the OpenGL shader program
bool ShaderProgram::link()
{
  glLinkProgram(id_);
  int status;
  glGetProgramiv(id_, GL_LINK_STATUS, &status);

  if (status == FALSE) {
    char log[1024];
    char message[1536];
    int log_length;

    glGetProgramInfoLog(id_, 1024, &log_length, log);
    sprintf_s(message, "Error! Shader program %s wasn't linked! The linker returned:\n\n%s", log);
    MessageBox(NULL, message, "Error", MB_ICONERROR);

    return false;
  }

  linked_ = status == GL_TRUE;
  return linked_;
}

// Deletes the program and frees memory on the GPU
void ShaderProgram::deleteProgram()
{
  if(!linked_) {
    return;
  }

  linked_ = false;
  glDeleteProgram(id_);
}

// Instructs OpenGL to use this program
void ShaderProgram::use()
{
  if(linked_) {
    glUseProgram(id_);
  }
}

// Returns the OpenGL program ID
UINT ShaderProgram::id()
{
  return id_;
}

// A collection of functions to set uniform variables inside shaders

// Setting floats

void ShaderProgram::setUniform(std::string name, float* values, int count)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform1fv(location, count, values);
}

void ShaderProgram::setUniform(std::string name, const float value)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform1fv(location, 1, &value);
}

// Setting vectors

void ShaderProgram::setUniform(std::string name, glm::vec2* vectors, int count)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform2fv(location, count, (GLfloat *) vectors);
}

void ShaderProgram::setUniform(std::string name, const glm::vec2 vector)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform2fv(location, 1, (GLfloat *) &vector);
}

void ShaderProgram::setUniform(std::string name, glm::vec3* vectors, int count)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform3fv(location, count, (GLfloat *) vectors);
}

void ShaderProgram::setUniform(std::string name, const glm::vec3 vector)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform3fv(location, 1, (GLfloat *) &vector);
}

void ShaderProgram::setUniform(std::string name, glm::vec4* vectors, int count)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform4fv(location, count, (GLfloat *) vectors);
}

void ShaderProgram::setUniform(std::string name, const glm::vec4 vector)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform4fv(location, 1, (GLfloat *) &vector);
}

// Setting 3x3 matrices

void ShaderProgram::setUniform(std::string name, glm::mat3* matrices, int count)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniformMatrix3fv(location, count, false, (GLfloat *) matrices);
}

void ShaderProgram::setUniform(std::string name, const glm::mat3 matrix)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniformMatrix3fv(location, 1, false, (GLfloat *) &matrix);
}

// Setting 4x4 matrices

void ShaderProgram::setUniform(std::string name, glm::mat4 *matrices, int count)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniformMatrix4fv(location, count, false, (GLfloat *) matrices);
}

void ShaderProgram::setUniform(std::string name, const glm::mat4 matrix)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniformMatrix4fv(location, 1, false, (GLfloat *) &matrix);
}

// Setting integers

void ShaderProgram::setUniform(std::string name, int* values, int count)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform1iv(location, count, values);
}

void ShaderProgram::setUniform(std::string name, const int value)
{
  int location = glGetUniformLocation(id_, name.c_str());
  glUniform1i(location, value);
}
