#include "window.h"

#include "include/gl/glew.h"
#include "include/gl/wglew.h"
#include <gl/gl.h>

#define SIMPLE_OPENGL_CLASS_NAME "simple_openGL_class_name"

Window::Window()
{
}

// A message handler for the dummy window
LRESULT CALLBACK fakeWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  PAINTSTRUCT ps;
  switch (msg) {
    case WM_PAINT:                  
      BeginPaint(hwnd, &ps);              
      EndPaint(hwnd, &ps);          
    break;

    default:
      return DefWindowProc(hwnd, msg, wparam, lparam); // Default window procedure
    break;
  }

  return 0;
}

// A function to register the dummy window
void Window::registerOpenGLClass(HINSTANCE hInstance)
{
  static bool class_registered = false;
  if (class_registered) {
    return;
  }

  WNDCLASSEX wc;
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
  wc.lpfnWndProc = (WNDPROC) fakeWinProc;
  wc.cbClsExtra = 0; wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
  wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR+1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = SIMPLE_OPENGL_CLASS_NAME;

  RegisterClassEx(&wc);
  class_registered = true;
}

// Create a dummy window, intialise GLEW, and then delete the dummy window
bool Window::initGLEW()
{
  static bool glew_initialised = false;
  if (glew_initialised) {
    return true;
  }

  registerOpenGLClass(hinstance_);

  HWND fake_hwnd = CreateWindow(
    SIMPLE_OPENGL_CLASS_NAME, "FAKE", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
    0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
    NULL, hinstance_, NULL
  );

  hdc_ = GetDC(fake_hwnd);

  // First, choose false pixel format
  PIXELFORMATDESCRIPTOR pfd;
  memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  pfd.iLayerType = PFD_MAIN_PLANE;
 
  int format = ChoosePixelFormat(hdc_, &pfd);
  if (format == 0) {
    return false;
  }

  if (!SetPixelFormat(hdc_, format, &pfd)) {
    return false;
  }

  // Create the false, old style context (OpenGL 2.1 and before)
  HGLRC fake_hrc = wglCreateContext(hdc_);
  wglMakeCurrent(hdc_, fake_hrc);

  bool result = true;

  if (glewInit() != GLEW_OK) {
    MessageBox(hwnd_, "Couldn't initialize GLEW!", "Fatal Error", MB_ICONERROR);
    result = false;
  }
  glew_initialised = true;

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(fake_hrc);
  DestroyWindow(fake_hwnd);

  return result;
}

// Initialise GLEW and create the real game window
HDC Window::init(HINSTANCE hinstance) 
{
  hinstance_ = hinstance;
  if (!initGLEW()) {
    return false;
  }

  name_ = "Rollercoaster";
  create("WELCOME TO SAM'S FUCKING AWESOME ROLLERCOASTER ON A MOTHERFUCKING TROPICAL ISLAND!");

  // If we never got a valid window handle, quit the program
  return hwnd_ == NULL ? NULL : hdc_;
}

// Create the game window
void Window::create(std::string title) 
{
  WNDCLASSEX wcex;
  memset(&wcex, 0, sizeof(WNDCLASSEX));
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_OWNDC;
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wcex.hIcon = LoadIcon(hinstance_, IDI_WINLOGO);
  wcex.hIconSm = LoadIcon(hinstance_, IDI_WINLOGO);
  wcex.hCursor = LoadCursor(hinstance_, IDC_ARROW);
  wcex.hInstance = hinstance_;
  wcex.lpfnWndProc = WinProc;
  wcex.lpszClassName = name_.c_str();
  wcex.lpszMenuName = NULL;
  RegisterClassEx(&wcex);

  int x = (GetSystemMetrics(SM_CXSCREEN) - Window::WIDTH) / 2;
  int y = (GetSystemMetrics(SM_CYSCREEN) - Window::HEIGHT) / 2;
  hwnd_ = CreateWindowEx(0, name_.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, x, y, Window::WIDTH, Window::HEIGHT, NULL, NULL, hinstance_, NULL);

  // Initialise OpenGL here
  initOpenGL();
  
  ShowWindow(hwnd_, SW_SHOW);
  GetClientRect(hwnd_, &dimensions_);

  UpdateWindow(hwnd_);

  // ShowCursor(FALSE);
  SetFocus(hwnd_);
}

// Initialise OpenGL, including the pixel format descriptor and the OpenGL version
void Window::initOpenGL()
{
  hdc_ = GetDC(hwnd_);

  bool error = false;
  PIXELFORMATDESCRIPTOR pfd;

  int major_version = 3;
  int minor_version = 3;

  if (WGLEW_ARB_create_context && WGLEW_ARB_pixel_format) {
    const int format_attributes[] = {
      WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
      WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
      WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
      WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
      WGL_COLOR_BITS_ARB, 32,
      WGL_DEPTH_BITS_ARB, 24,
      WGL_STENCIL_BITS_ARB, 8,
      0 // End of attributes list
    };

    int context_attributes[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, major_version,
      WGL_CONTEXT_MINOR_VERSION_ARB, minor_version,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0 // End of attributes list
    };

    int pixel_format;
    int num_format;
    wglChoosePixelFormatARB(hdc_, format_attributes, NULL, 1, &pixel_format, (UINT *) &num_format);

    // PFD seems to be only redundant parameter now
    if (!SetPixelFormat(hdc_, pixel_format, &pfd)) {
      return;
    }

    hrc_ = wglCreateContextAttribsARB(hdc_, 0, context_attributes);
    // If everything went OK
    if (hrc_) {
      wglMakeCurrent(hdc_, hrc_);
    } else {
      error = true;
    }
  } else {
    error = true;
  }
  
  if(error)
  {
    // Generate error messages
    char message[255];
    char title[255];

    sprintf_s(message, "OpenGL %d.%d is not supported! Please download latest GPU drivers!", major_version, minor_version);
    sprintf_s(title, "OpenGL %d.%d Not Supported", major_version, minor_version);

    MessageBox(hwnd_, message, title, MB_ICONINFORMATION);
    return;
  }

  return;
}

// Deinitialise the window and rendering context
void Window::deinit()
{
  if (hrc_) {
    wglMakeCurrent(NULL, NULL);
  }

  if (hdc_) {
    ReleaseDC(hwnd_, hdc_);
  }

  UnregisterClass(class_, hinstance_);
  PostQuitMessage(0);
}

void Window::setDimensions(RECT dimensions) 
{
  dimensions_ = dimensions;
}

RECT Window::dimensions()
{
  return dimensions_;
}

HDC Window::hdc() const
{
  return hdc_;
}

HINSTANCE Window::hinstance() const
{
  return hinstance_;
}

HGLRC Window::hrc() const
{
  return hrc_;
}

HWND Window::hwnd() const
{
  return hwnd_;
}
