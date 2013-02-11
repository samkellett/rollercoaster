#include "freetypefont.h"

#include "include/glm/gtc/matrix_transform.hpp"

#include "common.h"
#include "shader.h"
#include "shaderprogram.h"

#pragma comment(lib, "lib/freetype2410.lib")

FreeTypeFont::FreeTypeFont() :
  loaded_(false)
{
}

inline int next_p2(int n)
{
  int res = 1; 
  while (res < n) {
    res <<= 1; 
  }
  return res;
}

// Creates one single character (its texture).
void FreeTypeFont::createChar(int index)
{
  FT_Load_Glyph(freetype_face_, FT_Get_Char_Index(freetype_face_, index), FT_LOAD_DEFAULT);

  FT_Render_Glyph(freetype_face_->glyph, FT_RENDER_MODE_NORMAL);
  FT_Bitmap* bitmap = &freetype_face_->glyph->bitmap;

  int width = bitmap->width;
  int height = bitmap->rows;

  // Sam: I don't know what iTW and iTH stand for...
  int twidth = next_p2(width);
  int theight = next_p2(height);

  GLubyte* data = new GLubyte[twidth*theight];

  // Copy glyph data and add dark pixels elsewhere
  for (int ch = 0; ch < theight; ++ch) {
    for (int cw = 0; cw < twidth; ++cw) {
      data[ch * twidth + cw] = (ch >= height || cw >= width) ? 0 : bitmap->buffer[(height - ch - 1) * width + cw];
    }
  }
 
  // And create a texture from it
  textures_[index].createFromData(data, twidth, theight, 8, GL_DEPTH_COMPONENT, false);
  textures_[index].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);

  textures_[index].setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  textures_[index].setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Calculate glyph data
  adv_x_[index] = freetype_face_->glyph->advance.x >> 6;
  bearing_x_[index] = freetype_face_->glyph->metrics.horiBearingX >> 6;
  char_width_[index] = freetype_face_->glyph->metrics.width >> 6;

  adv_y_[index] = (freetype_face_->glyph->metrics.height - freetype_face_->glyph->metrics.horiBearingY) >> 6;
  bearing_y_[index] = freetype_face_->glyph->metrics.horiBearingY >> 6;
  char_height_[index] = freetype_face_->glyph->metrics.height >> 6;

  new_line_ = std::max(new_line_, (int) (freetype_face_->glyph->metrics.height >> 6));

  // Rendering data, texture coordinates are always the same, so now we waste a little memory
  glm::vec2 quad[] =
  {
    glm::vec2(0.0f, (float) (-adv_y_[index] + theight)),
    glm::vec2(0.0f, (float) -adv_y_[index]),
    glm::vec2((float) twidth, (float) (-adv_y_[index] + theight)),
    glm::vec2((float) twidth, (float) -adv_y_[index])
  };

  glm::vec2 texture_quad[] = {
    glm::vec2(0.0f, 1.0f),
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f)
  };

  // Add this char to VBO
  for (int i = 0; i < 4; ++i) {
    vbo_.addData(&quad[i], sizeof(glm::vec2));
    vbo_.addData(&texture_quad[i], sizeof(glm::vec2));
  }

  delete[] data;
}


// Loads an entire font with the given path sFile and pixel size iPXSize
bool FreeTypeFont::loadFont(std::string file, int size)
{
  BOOL error = FT_Init_FreeType(&freetype_);
  
  error = FT_New_Face(freetype_, file.c_str(), 0, &freetype_face_);
  if(error) {
    char message[1024];
    sprintf_s(message, "Cannot load font\n%s\n", file.c_str());

    MessageBox(NULL, message, "Error", MB_ICONERROR);
    return false;
  }

  FT_Set_Pixel_Sizes(freetype_face_, size, size);
  loaded_size_ = size;

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  vbo_.create();
  vbo_.bind();

  for (int i = 0; i < 128; ++i) {
    createChar(i);
  }

  loaded_ = true;

  FT_Done_Face(freetype_face_);
  FT_Done_FreeType(freetype_);
  
  vbo_.uploadDataToGPU(GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void *) (sizeof(glm::vec2)));

  return true;
}

// Loads a system font with given name (sName) and pixel size (iPXSize)
bool FreeTypeFont::loadSystemFont(std::string name, int size)
{
  char windows_directory[512]; 
  GetWindowsDirectory(windows_directory, 512);
  
  std::string path = windows_directory;
  path += "\\Fonts\\";
  path += name;

  return loadFont(path, size);
}


// Prints text at the specified location (x, y) with the given pixel size (size)
void FreeTypeFont::print(std::string text, int x, int y, int size)
{
  if(!loaded_) {
    return;
  }

  glBindVertexArray(vao_);
  fonts_->setUniform("gSampler", 0);

  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int cursor_x = x;
  int cursor_y = y;

  if(size == -1) {
    size = loaded_size_;
  }

  float fScale = (float) size / (float) loaded_size_;
  for (int i = 0; i < (int) text.size(); ++i) {
    if(text[i] == '\n') {
      cursor_x = x;
      cursor_y -= new_line_ * size / loaded_size_;
    } else {
      int index = (int) text[i];
      cursor_x += bearing_x_[index] * size / loaded_size_;

      if(text[i] != ' ') {
        textures_[index].bind();

        glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3((float) cursor_x, (float) cursor_y, 0.0f));
        modelview = glm::scale(modelview, glm::vec3(fScale));
        fonts_->setUniform("matrices.modelViewMatrix", modelview);
        
        // Draw character
        glDrawArrays(GL_TRIANGLE_STRIP, index*4, 4);
      }

      cursor_x += (adv_x_[index] - bearing_x_[index]) * size / loaded_size_;
    }
  }

  glDisable(GL_BLEND);
}


// Print formatted text at the location (x, y) with specified pixel size (iPXSize)
void FreeTypeFont::printf(int x, int y, int size, char* text, ...)
{
  char buf[512];

  // Sam: this is cool.
  va_list ap;
  va_start(ap, text);
  vsprintf_s(buf, text, ap);
  va_end(ap);

  print(buf, x, y, size);
}

// Deletes all font textures
void FreeTypeFont::release()
{
  // Sam: any reason this is 128 whereas the arrays are initiated as 256?
  for (int i = 0; i < 128; i++) {
    textures_[i].release();
  }

  vbo_.release();
  glDeleteVertexArrays(1, &vao_);
}

// Sets shader programme that font uses
void FreeTypeFont::setShaderProgram(ShaderProgram* shader_program)
{
  fonts_ = shader_program;
}