#ifndef CAMERA_H
#define CAMERA_H

#include "gameobject.h"

#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

class Camera : public GameObject 
{
public:
  enum State {
    FREE,
    FPS,
    SIDE,
    BIRD
  };

  Camera();

  glm::vec3 position() const;
  glm::vec3 view() const;
  glm::vec3 upVector() const;
  glm::vec3 strafeVector() const;

  glm::mat4 *perspectiveMatrix();
  glm::mat4 *orthographicMatrix();
  glm::mat4 viewMatrix();

  void set(glm::vec3 &position, glm::vec3 &viewpoint, glm::vec3 &up_vector);
  void rotateViewPoint(float theta, glm::vec3 point);

  State state();
  void setState(State state);

  void strafe(double direction);
  void advance(double direction);

  void setPerspectiveMatrix(float fov, float aspect_ratio, float near, float far);
  void setOrthographicMatrix(int width, int height);

  glm::mat3 normal(const glm::mat4 &modelview);

  // GameObject methods
  void mouseHandler(double dt, GLFWwindow *window);
  void keyboardHandler(double dt, GLFWwindow *window);
  void update(glutil::MatrixStack &modelview, double dt);
  void render(glutil::MatrixStack &modelview, ShaderProgram *program);

private:
  State state_;

  glm::vec3 position_;
  glm::vec3 view_;
  glm::vec3 up_vector_;
  glm::vec3 strafe_vector_;

  float speed_;

  glm::mat4 perspective_;
  glm::mat4 orthographic_;
};

#endif
