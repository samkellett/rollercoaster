#ifndef HUD_H
#define HUD_H

#include "gameobject.h"

class HUD : public GameObject
{
public:
  HUD();
  ~HUD();
  
  std::string program();

  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  FreeTypeFont font_;
};

#endif