#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

#include "common.h"

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK fakeWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class Window {
public:
  enum {
    WIDTH = 960,
    HEIGHT = 540,
  };

  Window();

  HDC init(HINSTANCE hinstance);
  void deinit();

  void setDimensions(RECT dimensions);
  RECT dimensions();

  HDC hdc() const;
  HINSTANCE hinstance() const;
  HGLRC hrc() const;
  HWND hwnd() const;

private:
  Window(const Window&);
  void operator=(const Window&);

  void create(std::string title);
  void initOpenGL();
  bool initGLEW();
  void registerOpenGLClass(HINSTANCE hInstance);

  HDC hdc_;
  HINSTANCE hinstance_;
  HGLRC hrc_;
  HWND hwnd_;

  LPSTR class_;
  RECT dimensions_;

  std::string name_;
};

#endif
