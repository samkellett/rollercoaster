#ifndef GAME_H
#define GAME_H

#include "common.h"

// Classes used in game
class Camera;
class Skybox;
class Shader;
class ShaderProgram;
class Plane;
class FreeTypeFont;
class Timer;
class Window;
class ObjModel;
class Sphere;
class CatmullRom;

class Game 
{
public:
  enum {
    FPS = 30
  };

  static Game& instance();
  ~Game();

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
  std::vector<ShaderProgram *> *shader_programs_;
  Plane *terrain_;
  FreeTypeFont *font_;
  ObjModel *barrel_;
  ObjModel *horse_;
  Sphere *sphere_;
  CatmullRom *catmull_;
};

#endif
