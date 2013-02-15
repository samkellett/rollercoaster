#define _USE_MATH_DEFINES

#include "camera.h"

#include "include/gl/glew.h"
#include <gl/gl.h>

#include <math.h>

#include "window.h"

// Constructor for camera -- initialise with some default values
Camera::Camera() :
  position_(glm::vec3(0.0f, 10.0f, 100.0f)), view_(glm::vec3(0.0f, 0.0f, 0.0f)),
  up_vector_(glm::vec3(0.0f, 1.0f, 0.0f)), speed_(0.025f)
{
}
 
// Set the camera at a specific position, looking at the view point, with a given up vector
void Camera::set(glm::vec3 &position, glm::vec3 &viewpoint, glm::vec3 &up_vector)
{
  position_ = position;
  view_ = viewpoint;
  up_vector_ = up_vector;
}

// Respond to mouse movement
void Camera::setViewByMouse()
{  
  int middle_x = Window::WIDTH >> 1;
  int middle_y = Window::HEIGHT >> 1;

  float angle_y = 0.0f;
  float angle_z = 0.0f;
  static float rotation_x = 0.0f;

  POINT mouse;
  GetCursorPos(&mouse);

  if (mouse.x == middle_x && mouse.y == middle_y) {
    return;
  }

  SetCursorPos(middle_x, middle_y);

  angle_y = (float) (middle_x - mouse.x) / 1000.0f;
  angle_z = (float) (middle_y - mouse.y) / 1000.0f;

  rotation_x -= angle_z;

  // Just a little bit below PI / 2 -- Sam: should be a #define / const / enum?
  float max_angle = 1.56f;

  if (rotation_x > max_angle) {
    rotation_x = max_angle;
  } else if (rotation_x < -max_angle) {
    rotation_x = -max_angle;
  } else {
    glm::vec3 cross = glm::cross(view_ - position_, up_vector_);
    glm::vec3 axis = glm::normalize(cross);

    rotateViewPoint(angle_z, axis);
  }

  rotateViewPoint(angle_y, glm::vec3(0, 1, 0));
}

// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
void Camera::rotateViewPoint(float angle, glm::vec3 &point)
{
  glm::vec3 view = view_ - position_;
  
  glm::mat4 rotated = glm::rotate(glm::mat4(1), angle * 180.0f / (float) M_PI, point);
  glm::vec4 new_view = rotated * glm::vec4(view, 1);

  view_ = position_ + glm::vec3(new_view);
}

// Strafe the camera (side to side motion)
void Camera::strafe(double direction)
{
  float speed = (float) (speed_ * direction);

  position_.x = position_.x + strafe_vector_.x * speed;
  position_.z = position_.z + strafe_vector_.z * speed;

  view_.x = view_.x + strafe_vector_.x * speed;
  view_.z = view_.z + strafe_vector_.z * speed;
}

// Advance the camera (forward / backward motion)
void Camera::advance(double direction)
{
  float speed = (float) (speed_ * direction);

  glm::vec3 view = glm::normalize(view_ - position_);
  position_ = position_ + view * speed;
  view_ = view_ + view * speed;
}

// Update the camera to respond to mouse motion for rotations and keyboard for translation
void Camera::update(double dt)
{
  glm::vec3 cross = glm::cross(view_ - position_, up_vector_);
  strafe_vector_ = glm::normalize(cross);

  setViewByMouse();
  translateByKeyboard(dt);
}

// Update the camera to respond to key presses for translation
void Camera::translateByKeyboard(double dt)
{
  if(GetKeyState(VK_UP) & 0x80) {    
    advance(1.0 * dt);  
  }

  if(GetKeyState(VK_DOWN) & 0x80) {  
    advance(-1.0 * dt);    
  }

  if(GetKeyState(VK_LEFT) & 0x80) {  
    strafe(-1.0 * dt);
  }

  if(GetKeyState(VK_RIGHT) & 0x80 ) {      
    strafe(1.0 * dt);
  }  
}

// Return the camera position
glm::vec3 Camera::position() const
{
  return position_;
}

// Return the camera view point
glm::vec3 Camera::view() const
{
  return view_;
}

// Return the camera up vector
glm::vec3 Camera::upVector() const
{
  return up_vector_;
}

// Return the camera strafe vector
glm::vec3 Camera::strafeVector() const
{
  return strafe_vector_;
}

// Return the camera perspective projection matrix
glm::mat4 *Camera::perspectiveMatrix()
{
  return &perspective_;
}

// Return the camera orthographic projection matrix
glm::mat4 *Camera::orthographicMatrix()
{
  return &orthographic_;
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio, 
// and near / far clipping planes
void Camera::setPerspectiveMatrix(float fov, float ratio, float close, float end)
{
	perspective_ = glm::perspective(fov, ratio, close, end);
}

// The the camera orthographic projection matrix to match the width and height passed in
void Camera::setOrthographicMatrix(int width, int height)
{
  orthographic_ = glm::ortho(0.0f, (float) width, 0.0f, (float) height);
}

// Get the camera view matrix
glm::mat4 Camera::viewMatrix()
{
  return glm::lookAt(position_, view_, up_vector_);
}

// The normal matrix is used to transform normals to eye coordinates -- part of lighting calculations
glm::mat3 Camera::normalMatrix(const glm::mat4 &modelview)
{
  return glm::transpose(glm::inverse(glm::mat3(modelview)));
}

