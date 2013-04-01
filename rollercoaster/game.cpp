/* 
 OpenGL Template for INM376 / IN3005
 City University London, School of Informatics
 Source code drawn from a number of sources and examples, including contributions from
 - Drs. Greg Slabaugh, Chris Child, Dean Mohamedally
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett
 and others

 For educational use by School of Informatics, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshs, set up lighting, 
 different camera controls, etc.
 
 Template version 1.5a 31/01/2013
 Dr. Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/

#include "game.h"

#include "lighting.h"
#include "matrixstack.h"
#include "shader.h"
#include "shaderprogram.h"
#include "timer.h"
#include "window.h"

// GameObjects
#include "camera.h"
#include "skybox.h"
#include "terrain.h"
#include "hud.h"

#include "rollercoaster.h"
#include "pyramid.h"
#include "tree.h"

// Some prettifiers
#define OBJECT(Obj) objects_.push_back(new Obj)
#define GAMEOBJECT(Obj, i) (Obj *) objects_[i]
#define SHADER_PROGRAM(name) programs_.push_back(#name)

Game::Game() : 
  camera_(NULL), terrain_(NULL), dt_(0.0), fps_(0)
{
}

Game::~Game() 
{ 
  while (!objects_.empty()) {
    delete objects_.back();
    objects_.pop_back();
  }

  for (ShaderProgramMap::iterator program(shader_programs_.begin()); program != shader_programs_.end(); ++program) {
    delete program->second;
  }
}

void Game::registerObjects()
{
  // Shader Programs:
  SHADER_PROGRAM(main);
  SHADER_PROGRAM(terrain);
  SHADER_PROGRAM(fonts);
  SHADER_PROGRAM(trees);

  // GameObjects:
  OBJECT(Camera);
  OBJECT(Skybox);
  OBJECT(Terrain);
  OBJECT(HUD);

  OBJECT(Rollercoaster);

  OBJECT(Tree("obj__plamt2"));

  OBJECT(Pyramid);
  OBJECT(Pyramid(glm::vec3(2.0f, 0.0f, 4.0f), 2.5f));

  // GameObjects that are tightly coupled need to be explicitely defined:
  // (this kinda sucks, but fuck it...)
  camera_ = GAMEOBJECT(Camera, 0);
  terrain_ = GAMEOBJECT(Terrain, 2);
}

void Game::init() 
{
  // Set the clear colour and depth
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClearDepth(1.0f);

  registerObjects();

  RECT dimensions = window_.dimensions();
  int width = dimensions.right - dimensions.left;
  int height = dimensions.bottom - dimensions.top;

  // Set the orthographic and perspective projection matrices based on the image size
  camera_->setOrthographicMatrix(width, height); 
  camera_->setPerspectiveMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

  // Load shaders
  std::vector<Shader> shaders;
  std::vector<std::string> shader_filenames;

  WIN32_FIND_DATA file;
  HANDLE find = FindFirstFile("resources/shaders/*", &file);
  if (find != INVALID_HANDLE_VALUE) {
    do {
      if ((file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
        shader_filenames.push_back(file.cFileName);
      }
    } while(FindNextFile(find, &file) != 0);
    FindClose(find);
  }
 
  for (unsigned int i = 0; i < shader_filenames.size(); ++i) {
    std::string ext = shader_filenames[i].substr((int) shader_filenames[i].size() - 4, 4);
    int shader_type;
    if (ext == "vert") {
      shader_type = GL_VERTEX_SHADER;
    } else if (ext == "frag") {
      shader_type = GL_FRAGMENT_SHADER;
    } else {
      shader_type = GL_GEOMETRY_SHADER;
    }

    Shader shader;
    shader.loadShader("resources/shaders/", shader_filenames[i], shader_type);
    shaders.push_back(shader);
  }

  for (unsigned int i = 0; i < programs_.size(); ++i) {
    char *name = programs_[i];

    shader_programs_[name] = new ShaderProgram;
    shader_programs_[name]->create();

    for (unsigned int j = 0; j < shaders.size(); ++j) {
      if (shaders[j].name() == name) {
        shader_programs_[name]->addShader(&shaders[j]);
      }
    }

    shader_programs_[name]->link();
  }

  // Initialise GameObjects:
  for (unsigned int i = 0; i < objects_.size(); ++i) {
    GameObject *object = objects_[i];
    ShaderProgram *program = shader_programs_[object->program()];

    program->use();
    object->init(program);
  }
}

void Game::setHInstance(HINSTANCE hinstance) 
{
  hinstance_ = hinstance;
}

Camera *Game::camera()
{
  return camera_;
}

Window &Game::window()
{
  return window_;
}

float Game::height(glm::vec3 point)
{
  if (terrain_) {
    return terrain_->height(point);
  } else {
    return 0.0f;
  }
}

int Game::fps()
{
  return fps_;
}

void Game::loop() 
{  
  // Clear the buffers and enable depth testing (z-buffering)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // Set up a matrix stack
  glutil::MatrixStack modelview;
  modelview.setIdentity();

  // Set the projection and modelview matrix based on the current camera location
  modelview.lookAt(camera_->position(), camera_->view(), camera_->upVector());

  // Calculate normals
  for (ShaderProgramMap::iterator program(shader_programs_.begin()); program != shader_programs_.end(); ++program) {
    ShaderProgram *shader_program = program->second;

    shader_program->use();
    shader_program->setUniform("matrices.projection", camera_->perspectiveMatrix());
    shader_program->setUniform("matrices.normal", camera_->normal(modelview.top()));

    Lighting::white(modelview, shader_program);
  }

  // Update / render
  for (unsigned int i = 0; i < objects_.size(); ++i) {
    modelview.push();
      GameObject *object = objects_[i];
      ShaderProgram *program = shader_programs_[object->program()];

      object->mouseHandler(dt_);
      object->keyboardHandler(dt_);
      object->update(modelview, dt_);

      program->use();
      object->render(modelview, program);
    modelview.pop();
  }

  elapsed_ += dt_;
  ++count_;

  if(elapsed_ > 1000 ) {
    elapsed_ = 0;
    fps_ = count_;

    count_ = 0;
  }

  // Swap buffers to show the rendered image
  SwapBuffers(window_.hdc());    
}

WPARAM Game::exec() 
{
  window_.init(hinstance_);

  if(!window_.hdc()) {
    return 1;
  }

  init();

  timer_.start();
  double frame_duration = 1000.0 / (double) Game::FPS;

  MSG msg;
  while(true) {                          
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
      if(msg.message == WM_QUIT) {
        break;
      }

      TranslateMessage(&msg);  
      DispatchMessage(&msg);
    } else {
      timer_.start();
      loop();
      dt_ = timer_.elapsed();
    }
  }

  window_.deinit();

  return msg.wParam;
}

LRESULT Game::processEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
  LRESULT result = 0;

  switch (message) {
    case WM_ACTIVATE:
      switch(LOWORD(w_param)) {
        case WA_ACTIVE:
        case WA_CLICKACTIVE:
          timer_.start();
        break;
      }
    break;

    case WM_SIZE:
      RECT dimensions;
      GetClientRect(window, &dimensions);
      window_.setDimensions(dimensions);
    break;

    case WM_PAINT:
      PAINTSTRUCT ps;
      BeginPaint(window, &ps);
      EndPaint(window, &ps);
    break;

    case WM_KEYDOWN:
      switch(w_param) {
        case VK_ESCAPE:
          PostQuitMessage(0);
        break;
      }
    break;

    case WM_DESTROY:
      PostQuitMessage(0);
    break;

    default:
      result = DefWindowProc(window, message, w_param, l_param);
    break;
  }

  return result;
}

Game& Game::instance() 
{
  static Game instance;

  return instance;
}
