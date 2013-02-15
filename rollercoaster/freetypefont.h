#ifndef FREETYPEFONT_H
#define FREETYPEFONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture.h"
#include "vbo.h"

class ShaderProgram;

// This class is a wrapper for FreeType fonts and their usage with OpenGL
class FreeTypeFont
{
public:
  FreeTypeFont();

  bool loadFont(std::string file, int size);
  bool loadSystemFont(std::string name, int size);

  int textWidth(std::string text, int size);

  void print(std::string sText, int x, int y, int size = -1);
  void printf(int x, int y, int size, char* text, ...);

  void release();

  void setShaderProgram(ShaderProgram *shader_program);

private:
  void createChar(int index);

  Texture textures_[256];

  int adv_x_[256];
  int adv_y_[256];

  int bearing_x_[256];
  int bearing_y_[256];

  int char_width_[256];
  int char_height_[256];

  int loaded_size_;
  int new_line_;

  bool loaded_;

  UINT vao_;
  VBO vbo_;

  FT_Library freetype_;
  FT_Face freetype_face_;
  ShaderProgram* fonts_;
};

#endif
