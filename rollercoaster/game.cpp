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

#include "matrixstack.h"
#include "shader.h"
#include "shaderprogram.h"
#include "timer.h"
#include "window.h"

// GameObject's
#include "camera.h"
#include "skybox.h"
#include "terrain.h"
#include "hud.h"

// tbc
#include "builder.h"

#define OBJECT(Obj) objects_.push_back(new Obj)

Game::Game() : 
  camera_(NULL), dt_(0.0), fps_(0)
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
  OBJECT(Camera);
  OBJECT(Skybox);
  OBJECT(Terrain);
  OBJECT(Builder);
  OBJECT(HUD);

  camera_ = (Camera *) objects_[0];
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
  std::string shader_filenames[] = {
    "perVertexLighting.vert",
    "perVertexLighting.frag",
    "ortho2D.vert",
    "font2D.frag"
  };
 
  int amount = sizeof(shader_filenames) / sizeof(shader_filenames[0]);
  for (int i = 0; i < amount; ++i) {
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
    shader.loadShader("resources/shaders/" + shader_filenames[i], shader_type);
    shaders.push_back(shader);
  }

  // Create shader programs
  char *programs[] = {
    "main",
    "fonts"
  };

  for (int i = 0; i < sizeof(programs) / sizeof(programs[0]); ++i) {
    char *name = programs[i];

    shader_programs_[name] = new ShaderProgram;
    shader_programs_[name]->create();
    shader_programs_[name]->addShader(&shaders[i * 2]);
    shader_programs_[name]->addShader(&shaders[i * 2 + 1]);
    shader_programs_[name]->link();
  }

  // Set the texture sampler in the fragment shader
  shader_programs_["main"]->setUniform("gSampler", 0);
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

  // Use the main shader program 
  ShaderProgram *main = shader_programs_["main"];
  main->use();
  main->setUniform("textured", true);

  // Set the projection and modelview matrix based on the current camera location  
  main->setUniform("matrices.projection", camera_->perspectiveMatrix());
  modelview.lookAt(camera_->position(), camera_->view(), camera_->upVector());

  // Set light and materials in main shader program
  glm::vec4 position(-100, 100, -100, 1);
  glm::mat3 normal_matrix = camera_->normal(modelview.top());
  
  // Convert light position to eye coordinates, since lighting is done in eye coordinates
  glm::vec4 light_eye = modelview.top() * position;

  // Position of light source in eye coordinates
  main->setUniform("light.position", light_eye);

  // Ambient colour of light
  main->setUniform("light.ambient", glm::vec3(1.0f));

  // Diffuse colour of light
  main->setUniform("light.diffuse", glm::vec3(1.0f));

  // Specular colour of light
  main->setUniform("light.specular", glm::vec3(1.0f));

  // Ambient material reflectance
  main->setUniform("material.ambient", glm::vec3(1.0f));

  // Diffuse material reflectance
  main->setUniform("material.diffuse", glm::vec3(0.0f));

  // Specular material reflectance
  main->setUniform("material.specular", glm::vec3(0.0f));

  // Shininess material property
  main->setUniform("material.shininess", 15.0f);

  // Calculate normals
  for (ShaderProgramMap::iterator program(shader_programs_.begin()); program != shader_programs_.end(); ++program) {
    ShaderProgram *shader_program = program->second;
    shader_program->setUniform("matrices.normal", camera_->normal(modelview.top()));
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

int Game::fps()
{
  return fps_;
}
