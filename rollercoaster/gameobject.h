#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "common.h"
#include "game.h"
#include "matrixstack.h"
#include "shaderprogram.h"

class GameObject
{
public:
  virtual void update(glutil::MatrixStack &modelview, double dt) = 0;
  virtual void render(glutil::MatrixStack &modelview, ShaderProgram *program) = 0;

  virtual std::string program() 
  {
    return "main";
  };

protected:
  GameObject()
  {
  }
};

#endif
