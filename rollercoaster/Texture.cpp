#include "texture.h"

#include "common.h"

#include "include\freeimage\FreeImage.h"
#pragma comment(lib, "lib/FreeImage.lib")

Texture::Texture() :
  mipmaps_(false)
{
}

// Create a texture from the data stored in bData.  
void Texture::createFromData(BYTE *data, int width, int height, int bpp, GLenum format, bool mipmaps)
{
  path_ = "";
  mipmaps_ = mipmaps;
  width_ = width;
  height_ = height;
  bpp_ = bpp;

  // Generate an OpenGL texture ID for this texture
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  if(format == GL_RGBA || format == GL_BGRA || format == GL_RGB || format == GL_BGR) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  }

  if(mipmaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  glGenSamplers(1, &sampler_);
}

// Loads a 2D texture given the filename (sPath).  bGenerateMipMaps will generate a mipmapped texture if true
bool Texture::load(std::string path, bool mipmaps)
{
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  FIBITMAP* dib(0);

  fif = FreeImage_GetFileType(path.c_str(), 0); // Check the file signature and deduce its format

  // If still unknown, try to guess the file format from the file extension
  if(fif == FIF_UNKNOWN) { 
    fif = FreeImage_GetFIFFromFilename(path.c_str());
  }
  
  // If still unknown, return failure
  if(fif == FIF_UNKNOWN) {
    return false;
  }

  // Check if the plugin has reading capabilities and load the file
  if(FreeImage_FIFSupportsReading(fif)) {
    dib = FreeImage_Load(fif, path.c_str());
  }

  if(!dib) {
    char message[1024];
    sprintf_s(message, "Cannot load image\n%s\n", path.c_str());
    MessageBox(NULL, message, "Error", MB_ICONERROR);

    return false;
  }

  BYTE* data = FreeImage_GetBits(dib); // Retrieve the image data

  // If somehow one of these failed (they shouldn't), return failure
  if(data == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0) {
    return false;
  }

  GLenum format;
  int bada = FreeImage_GetBPP(dib);
  
  if(FreeImage_GetBPP(dib) == 32) {
    format = GL_RGBA;
  } else if(FreeImage_GetBPP(dib) == 24) {
    format = GL_BGR;
  } else if(FreeImage_GetBPP(dib) == 8) {
    format = GL_LUMINANCE;
  }

  createFromData(data, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, mipmaps);
  FreeImage_Unload(dib);

  path_ = path;

  return true;
}

void Texture::setSamplerParameter(GLenum parameter, GLenum value)
{
  glSamplerParameteri(sampler_, parameter, value);
}

// Uses a sampler object to set texture filtering parameters for the magnification and minificaiton filters
void Texture::setFiltering(int magnification, int minification)
{
  minification_ = minification;
  magnification_ = magnification;

  // Set magnification filter
  if(magnification == TEXTURE_FILTER_MAG_NEAREST) {
    glSamplerParameteri(sampler_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  } else if(magnification == TEXTURE_FILTER_MAG_BILINEAR) {
    glSamplerParameteri(sampler_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  // Set minification filter
  if(minification == TEXTURE_FILTER_MIN_NEAREST) {
    glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  } else if(minification == TEXTURE_FILTER_MIN_BILINEAR) {
    glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  } else if(minification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP) {
    glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  } else if(minification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP) {
    glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  } else if(minification == TEXTURE_FILTER_MIN_TRILINEAR) {
    glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  }
}

// Binds a texture for rendering
void Texture::bind(int texture)
{
  glActiveTexture(GL_TEXTURE0 + texture);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glBindSampler(texture, sampler_);
}

// Frees memory on the GPU of the texture
void Texture::release()
{
  glDeleteSamplers(1, &sampler_);
  glDeleteTextures(1, &texture_);
}

int Texture::minificationFilter()
{
  return minification_;
}

int Texture::magnificationFilter()
{
  return magnification_;
}

int Texture::width()
{
  return width_;
}

int Texture::height()
{
  return height_;
}

int Texture::bpp()
{
  return bpp_;
}