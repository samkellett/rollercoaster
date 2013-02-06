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

#include "include/gl/glew.h"
#include <gl/gl.h>

// setup includes
#include "timer.h"
#include "window.h"

// game includes
#include "camera.h"
#include "skybox.h"
#include "plane.h"
#include "shaders.h"
#include "freetypefont.h"
#include "objmodel.h"
#include "sphere.h"
#include "matrixstack.h"


Game::Game() : 
  skybox_(NULL), camera_(NULL), terrain_(NULL), font_(NULL), barrel_(NULL), horse_(NULL), sphere_(NULL),
  window_(Window::instance()), timer_(NULL), dt_(0.0), fps_(0)
{
}

Game::~Game() 
{ 
  // game objects
  delete camera_;
  delete skybox_;
  delete terrain_;
  delete font_;
  delete barrel_;
  delete horse_;
  delete sphere_;

  while(!shader_programs_->empty()) {
    delete shader_programs_->back();
    shader_programs_->pop_back();
  }
  delete shader_programs_;

  // setup objects
  delete timer_;
}

void Game::init() 
{
  // Set the clear colour and depth
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClearDepth(1.0f);

  /// Create objects
  camera_ = new Camera;
  skybox_ = new Skybox;
  shader_programs_ = new vector<ShaderProgram *>;
  terrain_ = new Plane;
  font_ = new FreeTypeFont;
  barrel_ = new ObjModel;
  horse_ = new ObjModel;
  sphere_ = new Sphere;

  RECT dimensions = window_.dimensions();

  int width = dimensions.right - dimensions.left;
  int height = dimensions.bottom - dimensions.top;

  // Set the orthographic and perspective projection matrices based on the image size
  camera_->setOrthographicProjectionMatrix(width, height); 
  camera_->setPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

  // Load shaders
  vector<Shader> shaders;
  vector<string> shader_filenames;

  shader_filenames.push_back("perVertexLighting.vert");
  shader_filenames.push_back("perVertexLighting.frag");
  shader_filenames.push_back("ortho2D.vert");
  shader_filenames.push_back("font2D.frag");

  for (int i = 0; i < (int) shader_filenames.size(); ++i) {
    string ext = shader_filenames[i].substr((int) shader_filenames[i].size() - 4, 4);
    int shader_type = ext == "vert" ? GL_VERTEX_SHADER : (ext == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER);

    Shader shader;
    shader.load("resources\\shaders\\" + shader_filenames[i], shader_type);
    shaders.push_back(shader);
  }

  // Create the main shader program
  ShaderProgram *main = new ShaderProgram;
  main->create();
  main->addShader(&shaders[0]);
  main->addShader(&shaders[1]);
  main->link();
  shader_programs_->push_back(main);
  
  // Create a shader program for fonts
  ShaderProgram *fonts = new ShaderProgram;
  fonts->create();
  fonts->addShader(&shaders[2]);
  fonts->AddShader(&shaders[3]);
  fonts->link();
  shader_programs_->push_back(fonts);

  // Create the skybox
  // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
  skybox_->create("resources\\skyboxes\\jajdarkland1\\", "jajdarkland1_ft.jpg", "jajdarkland1_bk.jpg", "jajdarkland1_lf.jpg", "jajdarkland1_rt.jpg", "jajdarkland1_up.jpg", "jajdarkland1_dn.jpg", 2500.0f);
  
  // Create the planar terrain
  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
  terrain_->create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); 

  font_->loadSystemFont("arial.ttf", 32);
  font_->setShader(fonts);

  // Load some meshes in OBJ format
  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
  barrel_->load("resources\\models\\Barrel\\Barrel02.obj", "Barrel02.mtl");

  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
  horse_->load("resources\\models\\Horse\\Horse2.obj", "Horse2.mtl");

  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
  sphere_->create("resources\\textures\\", "dirtpile01.jpg", 25, 25);

  // Set the texture sampler in the fragment shader
  main->setUniform("gSampler", 0);

}

void Game::render() 
{  
  // Clear the buffers and enable depth testing (z-buffering)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // Set up a matrix stack
  glutil::MatrixStack modelview;
  modelview.setIdentity();

  // Use the main shader program 
  ShaderProgram *main = (*shader_programs_)[0];
  main->useProgram();
  main->setUniform("bUseTexture", true);

  // Set the projection and modelview matrix based on the current camera location  
  main->setUniform("matrices.projMatrix", camera_->perspectiveMatrix());
  modelview.lookAt(camera_->position(), camera_->view(), camera_->upVector());

  // Set light and materials in main shader program
  glm::vec4 position(-100, 100, -100, 1);
  glm::mat3 normal_matrix = camera_->normalMatrix(modelview.Top());
  
  // Convert light position to eye coordinates, since lighting is done in eye coordinates
  glm::vec4 light_eye = modelview.top() * position;

  main->setUniform("light1.position", light_eye); // Position of light source in eye coordinates
  main->setUniform("light1.La", glm::vec3(1.0f)); // Ambient colour of light
  main->setUniform("light1.Ld", glm::vec3(1.0f)); // Diffuse colour of light
  main->setUniform("light1.Ls", glm::vec3(1.0f)); // Specular colour of light
  main->setUniform("material1.Ma", glm::vec3(1.0f)); // Ambient material reflectance
  main->setUniform("material1.Md", glm::vec3(0.0f)); // Diffuse material reflectance
  main->setUniform("material1.Ms", glm::vec3(0.0f)); // Specular material reflectance
  main->setUniform("material1.shininess", 15.0f); // Shininess material property

  // Render the skybox and terrain with full ambient light 
  modelview.push();
    // Translate the modelview matrix to the camera eye point so skybox stays centred around camera
    glm::vec3 eye = camera_->position();
    modelview.translate(eye);

    main->setUniform("matrices.modelViewMatrix", modelview.top());
    main->setUniform("matrices.normalMatrix", camera_->normalMatrix(modelview.top()));

    skybox_->render();
  modelview.pop();
  
  // Render the planar terrain
  modelview.push();
    main->setUniform("matrices.modelViewMatrix", modelview.top());
    main->setUniform("matrices.normalMatrix", camera_->normalMatrix(modelview.top()));
    
    terrain_->render();
  modelview.pop();

  // Turn on diffuse + specular materials
  main->setUniform("material1.Ma", glm::vec3(0.5f)); // Ambient material reflectance
  main->setUniform("material1.Md", glm::vec3(0.5f)); // Diffuse material reflectance
  main->setUniform("material1.Ms", glm::vec3(1.0f)); // Specular material reflectance  


  // Render the horse 
  modelview.push();
    modelview.translate(glm::vec3(0.0f, 0.0f, 0.0f));
    modelview.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
    modelview.scale(2.5f);

    main->setUniform("matrices.modelViewMatrix", modelview.top());
    main->setUniform("matrices.normalMatrix", camera_->normalMatrix(modelview.top()));

    horse_->render();
  modelview.pop();

  // Render the barrel 
  modelview.push();
    modelview.translate(glm::vec3(100.0f, 0.0f, 0.0f));
    modelview.scale(5.0f);
    
    main->setUniform("matrices.modelViewMatrix", modelview.top());
    main->setUniform("matrices.normalMatrix", camera_->normalMatrix(modelview.top()));

    barrel_->render();
  modelview.pop();

  // Render the sphere
  modelview.push();
    modelview.translate(glm::vec3(0.0f, 2.5f, 150.0f));
    modelview.scale(2.0f);

    main->setUniform("matrices.modelViewMatrix", modelview.top());
    main->setUniform("matrices.normalMatrix", camera_->normalMatrix(modelview.top()));

    // To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
    //pMainProgram->SetUniform("bUseTexture", false);
    sphere_->render();
  modelview.pop();

  renderFPS();

  // Swap buffers to show the rendered image
  SwapBuffers(window_.hdc());    
}



void Game::update() 
{
  // Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
  camera_->update(dt_);
}



void Game::renderFPS()
{
  static int count = 0;
  static double elapsed = 0.0f;

  ShaderProgram *fonts = (*shader_programs_)[1];

  RECT dimensions = window_.dimensions();
  int height = dimensions.bottom - dimensions.top;

  // Increase the elapsed time and frame counter
  elapsed += dt_;
  ++count;

  // Now we want to subtract the current time by the last time that was stored
  // to see if the time elapsed has been over a second, which means we found our FPS.
  if(elapsed > 1000 ) {
    elapsed = 0;
    fps_ = count;

    // Reset the frames per second
    count = 0;
  }

  if (fps_ > 0) {
    glDisable(GL_DEPTH_TEST);

    // Use the font shader program and render the text
    fonts->useProgram();

    fonts->setUniform("matrices.modelViewMatrix", glm::mat4(1));
    fonts->setUniform("matrices.projMatrix", camera_->orthographicMatrix());
    fonts->setUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    font_->printf(20, height - 20, 20, "FPS: %d", fps_);

    glEnable(GL_DEPTH_TEST);
  }
}
    

WPARAM Game::exec() 
{
  timer_ = new Timer;
  window_.init(hinstance_);

  if(!window_.hdc()) {
    return 1;
  }

  init();

  timer_->start();
  double frame_duration = 1000.0 / (double) Game::FPS;

  MSG msg;
  while(true) {                          
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
      if(msg.message == WM_QUIT) {
        break;
      }

      TranslateMessage(&msg);  
      DispatchMessage(&msg);
    } else if (active_) {
      /*
      // Fixed timer
      dt_ = timer_->elapsed();

      if (dt_ > frame_duration) {
        timer_->start();

        update();
        render();
      }
      */

      // Variable timer
      timer_->start();

      update();
      render();

      dt_ = timer_->elapsed();
    } else {
      // Do not consume processor power if application isn't active -- Sam: really?!
      Sleep(200);
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
          active_ = true;
          timer_->start();
        break;

        case WA_INACTIVE:
          active_ = false;
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