#ifndef GAME_H
#define GAME_H

#include "common.h"
#include <hash_map>

#include "window.h"
#include "freetypefont.h"
#include "timer.h"

class Camera;
class GameObject;
class ShaderProgram;

typedef std::hash_map<std::string, ShaderProgram *> ShaderProgramMap;
typedef std::vector<GameObject *> GameObjectList;

class Game
{
public:
  enum {
    FPS = 30
  };

  static Game& instance();
  ~Game();

  Camera *camera();
  Window &window();
  int fps();

  WPARAM exec();

  void setHInstance(HINSTANCE hinstance);
  LRESULT processEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

private:
  Game::Game();
  Game::Game(const Game&);
  void Game::operator=(const Game&);

  void init();
  void registerObjects();
  void loop();
  
  Window window_;
  HINSTANCE hinstance_;

  double dt_;
  int fps_;
  double elapsed_;
  int count_;
  Timer timer_;

  Camera *camera_;
  GameObjectList objects_;
  ShaderProgramMap shader_programs_;
};

#endif
