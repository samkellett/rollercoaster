#ifndef GAME_H
#define GAME_H

#include "common.h"
#include <hash_map>

#include "window.h"

class Camera;
class GameObject;
class ShaderProgram;
class Timer;
class Window;

// to be removed
class FreeTypeFont;
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

  WPARAM exec();

  void setHInstance(HINSTANCE hinstance);
  LRESULT processEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);

private:
  Game::Game();
  Game::Game(const Game&);
  void Game::operator=(const Game&);

  void init();
  void registerObjects();
  void loop();
  
  void renderFPS();

  Window window_;
  HINSTANCE hinstance_;

  double dt_;
  int fps_;
  Timer *timer_;

  std::vector<GameObject *> objects_;
  std::hash_map<std::string, ShaderProgram *> shader_programs_;

  Camera *camera_;
  FreeTypeFont *font_;
  ObjModel *barrel_;
  ObjModel *horse_;
  Sphere *sphere_;
  Builder *builder_;
};

#endif
