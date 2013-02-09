#include "vbo.h"

#include "common.h"

// Constructor -- initialise member variable m_bDataUploaded to false
VBO::VBO() :
  uploaded_(false)
{
}

// Create a VBO and optionally reserve space for data
void VBO::create(int size)
{
  glGenBuffers(1, &buffer_);
  
  data_.reserve(size);
  size_ = size;
}

// Release the VBO and any associated data
void VBO::release()
{
  glDeleteBuffers(1, &buffer_);

  uploaded_ = false;
  data_.clear();
}

// Maps the buffer to memory and returns a pointer to data.  
// iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY...
void *VBO::mapBufferToMemory(int usage)
{
  if(!uploaded_) {
    return NULL;
  }

  void *result = glMapBuffer(type_, usage);
  return result;
}

// Maps specified part of a buffer to memory and returns a pointer to data. 
// uiOffset is the data offset, and uiLength is the length of the data. 
void* VBO::mapSubBufferToMemory(int usage, UINT offset, UINT length)
{
  if(!uploaded_) {
    return NULL;
  }

  void *result = glMapBufferRange(type_, offset, length, usage);
  return result;
}

// Unmaps a previously mapped buffer
void VBO::unmapBuffer()
{
  glUnmapBuffer(type_);
}

// Binds a VBO.  iBufferType is the buffer type (e.g., GL_ARRAY_BUFFER, ...)
void VBO::bind(int type)
{
  type_ = type;
  glBindBuffer(type_, buffer_);
}

// Uploads the data to the GPU.  Afterwards, the data can be cleared.  
// iUsageHint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...
void VBO::uploadDataToGPU(int drawing)
{
  glBufferData(type_, data_.size(), &data_[0], drawing);
  uploaded_ = true;
  data_.clear();
}

// Adds data to the VBO.  
void VBO::addData(void *data, UINT size)
{
  data_.insert(data_.end(), (BYTE *) data, (BYTE *) data + size);
}

// Gets a pointer to the data.  Note this is only valid before uploading, since the data is cleared on upload.
void* VBO::data()
{
  if (uploaded_) {
    return NULL;
  }

  return (void*) data_[0];
}

// Returns the ID of the VBO
UINT VBO::buffer()
{
  return buffer_;
}
