#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "shaderprogram.h"

// Classes used in game
class Camera;
class Skybox;
class Shader;
class Plane;
class FreeTypeFont;
class Timer;
class Window;
class ObjModel;
class Sphere;
class Builder;

class Game 
{
public:
  enum {
    FPS = 30
  };

  static Game& instance();
  ~Game();

  Camera *camera();

  ShaderList *shaderPrograms();
  ShaderProgram *shaderPrograms(unsigned int index);

  WPARAM exec();

  void setHInstance(HINSTANCE hinstance);
  LRESULT processEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);

private:
  Game::Game();
  Game::Game(const Game&);
  void Game::operator=(const Game&);

  void init();
  void update();
  void render();

  void renderFPS();

  Window &window_;
  HINSTANCE hinstance_;

  double dt_;
  int fps_;
  Timer *timer_;
  bool active_;

  Skybox *skybox_;
  Camera *camera_;
  ShaderList *shader_programs_;
  Plane *terrain_;
  FreeTypeFont *font_;
  ObjModel *barrel_;
  ObjModel *horse_;
  Sphere *sphere_;
  Builder *builder_;
};

#endif
