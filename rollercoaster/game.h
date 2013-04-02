#ifndef GAME_H
#define GAME_H

#include "common.h"
#include <hash_map>

#include "window.h"
#include "freetypefont.h"
#include "timer.h"

class GameObject;
class ShaderProgram;

class Camera;
class Terrain;
class Rollercoaster;

typedef std::vector<GameObject *> GameObjectList;
typedef std::vector<char *> ShaderProgramList;
typedef std::hash_map<std::string, ShaderProgram *> ShaderProgramMap;

class Game
{
public:
  enum {
    FPS = 30
  };

  static Game& instance();
  ~Game();

  Camera *camera();
  float height(glm::vec3 point);

  glm::vec3 point();
  glm::vec3 futurePoint(double t);

  void switchTime();
  bool nighttime();

  int fps();
  int count();
  WPARAM exec();
  Window &window();
  void setHInstance(HINSTANCE hinstance);
  LRESULT processEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

private:
  Game::Game();
  Game::Game(const Game&);
  void Game::operator=(const Game&);

  void init();
  void registerObjects();
  void loop();
  
  Camera *camera_;
  Terrain *terrain_;
  Rollercoaster *rollercoaster_;

  bool nighttime_;

  GameObjectList objects_;
  ShaderProgramList programs_;
  ShaderProgramMap shader_programs_;

  // Boring shit
  Window window_;
  HINSTANCE hinstance_;

  double dt_;
  int fps_;
  double elapsed_;
  int count_;
  Timer timer_;
};

#endif
