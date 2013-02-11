#ifndef VBO_H
#define VBO_H

#include "common.h"

#include "include/gl/glew.h"
#include <gl/gl.h>

// This class provides a wrapper around an OpenGL Vertex Buffer Object
class VBO
{
public:
  VBO();

  // Creates a VBO
  void create(int size = 0);

  // Binds the VBO
  void bind(int type = GL_ARRAY_BUFFER);

  // Releases the VBO
  void release();

  // Add data to the VBO
  void addData(void* data, UINT size);

  // Upload the VBO to the GPU
  void uploadDataToGPU(int usage);

  void *mapBufferToMemory(int usage);
  void *mapSubBufferToMemory(int usage, UINT offset, UINT length);
  void unmapBuffer();

  void* data();
  UINT buffer();
  
private:
  UINT buffer_;
  int size_;
  int type_;
  std::vector<BYTE> data_;

  bool uploaded_;
};

#endif
