#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

#include "include/gl/glew.h"
#include <gl/gl.h>

enum TextureFiltering
{
  TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
  TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
  TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
  TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
  TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
  TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
  TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

// Class that provides a texture for texture mapping in OpenGL
class Texture
{
public:
  Texture();
  
  void createFromData(BYTE *data, int width, int height, int bpp, GLenum format, bool mipmaps = false);
  bool load(std::string path, bool mipmaps = false);
  void bind(int texture = 0);

  void setFiltering(int magnification, int minification);

  void setSamplerParameter(GLenum parameter, GLenum value);

  int minificationFilter();
  int magnificationFilter();

  int width();
  int height();
  int bpp();

  void release();

private:
  int width_;
  int height_;
  int bpp_;

  UINT texture_; // Texture name
  UINT sampler_; // Sampler name
  bool mipmaps_;

  int minification_;
  int magnification_;

  std::string path_;
};

#endif
