// #include <windows.h>

#include "game.h"

#include <GLFW/glfw3.h>
#include <GL/glew.h>

// LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
// {
//   return Game::instance().processEvents(window, message, w_param, l_param);
// }

// int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
// {
//   Game &game = Game::instance();
//   game.setHInstance(hinstance);

//   return game.exec();
// }

int main() {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
  window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE; 
  if (glewInit() != GLEW_OK) {
    return -1;
  }

  Game &game = Game::instance();
  game.exec(window);

  glfwTerminate();
  return 0;
}
