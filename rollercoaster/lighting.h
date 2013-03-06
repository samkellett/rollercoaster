#ifndef LIGHTING_H
#define LIGHTING_H

#include "matrixstack.h"

class ShaderProgram;

class Lighting
{
public:
  static void white(glutil::MatrixStack &modelview, ShaderProgram *program);
};

#endif
