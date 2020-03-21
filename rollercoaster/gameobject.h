#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "common.h"
#include "game.h"
#include "matrixstack.h"
#include "shaderprogram.h"
#include "lighting.h"

class GameObject
{
public:
  virtual void init(ShaderProgram *program)
  {
    program->setUniform("sampler0", 0);
  }

  virtual void update(glutil::MatrixStack &modelview, double dt) = 0;
  virtual void render(glutil::MatrixStack &modelview, ShaderProgram *program) = 0;

  virtual void mouseHandler(double, GLFWwindow *) {}
  virtual void keyboardHandler(double, GLFWwindow *) {}

  virtual std::string program() 
  {
    return "main";
  };

protected:
  GameObject() {}
};

typedef std::vector<GameObject *> GameObjectList;

#endif
