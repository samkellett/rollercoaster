#include <windows.h>

#include "game.h"

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
  return Game::instance().processEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
  Game &game = Game::instance();
  game.setHInstance(hinstance);

  return game.exec();
}
