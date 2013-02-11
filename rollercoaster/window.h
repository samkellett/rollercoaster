#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

#include "common.h"

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
public:
  static Window& instance();

  enum {
    WIDTH = 800,
    HEIGHT = 600,
  };

  HDC init(HINSTANCE hinstance);
  void deinit();

  void setDimensions(RECT dimensions);
  RECT dimensions();

  bool fullscreen() const;

  HDC hdc() const;
  HINSTANCE hinstance() const;
  HGLRC hrc() const;
  HWND hwnd() const;

private:
  Window();
  Window(const Window&);
  void operator=(const Window&);

  void create(std::string title);
  void initOpenGL();
  bool initGLEW();
  void registerOpenGLClass(HINSTANCE hInstance);

  bool fullscreen_;

  HDC hdc_;
  HINSTANCE hinstance_;
  HGLRC hrc_;
  HWND hwnd_;

  LPSTR class_;
  RECT  dimensions_;

  std::string name_;
};

#endif
