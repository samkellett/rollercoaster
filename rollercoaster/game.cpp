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
#include "cart.h"
#include "pyramid.h"
#include "tree.h"
#include "lamp.h"

// Data
#include "resources/data/trees.h"

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

  // Base GameObjects:
  OBJECT(Camera);
  OBJECT(Skybox);
  OBJECT(Terrain);

  // The main attraction
  OBJECT(Rollercoaster);
  OBJECT(Cart);

  // Static GameObjects
  for (int i = 0; i < sizeof(data::trees) / sizeof(data::trees[0]); ++i) {
    OBJECT(Tree(data::trees[i]));
  }

  OBJECT(Pyramid(glm::vec2(55.92f, 148.56f), 4.0f));
  OBJECT(Pyramid(glm::vec2(91.23f, 75.13f), 2.5f));
  OBJECT(Pyramid(glm::vec2(-94.84f, -229.97f), 4.5f));
  OBJECT(Pyramid(glm::vec2(-148.02f, -262.82f), 3.0f));
  OBJECT(Pyramid(glm::vec2(-122.47f, -292.54f), 2.5f));

  // Light source GameObjects
  OBJECT(Lamp(glm::vec2(117.18f, -100.39f), glm::vec3(0, 0, 1)));
  OBJECT(Lamp(glm::vec2(-187.27f, -301.06f), glm::vec3(0, 1, 0)));
  OBJECT(Lamp(glm::vec2(-79.36f, 156.53f), glm::vec3(1, 0, 0)));
  OBJECT(Lamp(glm::vec2(357.99f, -92.96f), glm::vec3(1, 1, 0)));
  OBJECT(Lamp(glm::vec2(83.16f, -385.89f), glm::vec3(1, 1, 1)));

  // And HUD last so it's on top.
  OBJECT(HUD);

  // GameObjects that are tightly coupled need to be explicitely defined:
  // (this kinda sucks, but fuck it...)
  camera_ = GAMEOBJECT(Camera, 0);
  terrain_ = GAMEOBJECT(Terrain, 2);
  rollercoaster_ = GAMEOBJECT(Rollercoaster, 3);
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

    Shader shader("resources/shaders/", shader_filenames[i], shader_type);
    shaders.push_back(shader);
  }

  for (unsigned int i = 0; i < programs_.size(); ++i) {
    char *name = programs_[i];

    shader_programs_[name] = new ShaderProgram;
    shader_programs_[name]->create();

    bool vert = false;
    bool frag = false;
    for (unsigned int j = 0; j < shaders.size(); ++j) {
      Shader &shader = shaders[j];
      if (shader.name() == name) {
        switch(shader.type()) {
        case GL_VERTEX_SHADER:
          vert = true;
          break;
        case GL_FRAGMENT_SHADER:
          frag = true;
          break;
        }
        shader_programs_[name]->addShader(&shaders[j]);
      }
    }

    // Fall back to the default shaders if there isn't one that shares a name with the shader program
    if (!vert) {
      Shader *shader = new Shader("resources/shaders/", Shader::DEFAULT_VERTEX_SHADER, GL_VERTEX_SHADER);
      shader_programs_[name]->addShader(shader);
    }

    if (!frag) {
      Shader *shader = new Shader("resources/shaders/", Shader::DEFAULT_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
      shader_programs_[name]->addShader(shader);
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

bool Game::nighttime()
{
  return nighttime_;
}

void Game::switchTime()
{
  nighttime_ = !nighttime_;
}

float Game::height(glm::vec3 point)
{
  if (terrain_) {
    return terrain_->height(point);
  } else {
    return 0.0f;
  }
}

glm::vec3 Game::point()
{
  return rollercoaster_->point();
}

glm::vec3 Game::futurePoint(double t)
{
  return rollercoaster_->point(rollercoaster_->t() + t);
}

int Game::fps()
{
  return fps_;
}

int Game::count()
{
  return count_;
}

void Game::loop() 
{  
  // Clear the buffers and enable depth testing (z-buffering)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

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
    shader_program->setUniform("nighttime", nighttime_);

    Lighting::global(modelview, shader_program);
  }

  // Update / render
  for (unsigned int i = 0; i < objects_.size(); ++i) {
    modelview.push();
      GameObject *object = objects_[i];

      object->mouseHandler(dt_);
      object->keyboardHandler(dt_);
      object->update(modelview, dt_);

      ShaderProgram *program = shader_programs_[object->program()];
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
        case 32: // space
          char pos[128];
          sprintf_s(pos, "  { %.2ff, %.2ff, %.2ff },\n", camera_->position().x, camera_->position().y, camera_->position().z);
          OutputDebugString(pos);

          rollercoaster_->addDerivative(camera_->position());
        break;

        case 49: // 1
          camera_->setState(Camera::FREE);
        break;

        case 50: // 2
          camera_->setState(Camera::FPS);
        break;

        case 51: // 3
          camera_->setState(Camera::SIDE);
        break;

        case 52: // 4
          camera_->setState(Camera::BIRD);
        break;

        case 53: // 5
          switchTime();
        break;

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
