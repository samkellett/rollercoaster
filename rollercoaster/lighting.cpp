#include "lighting.h"

#include "common.h"
#include "game.h"
#include "camera.h"
#include "shaderprogram.h"

void Lighting::white(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  Camera *camera = Game::instance().camera();

  // Set light and materials in main shader program
  glm::vec4 position(-100, -150, -100, 1);
  glm::mat3 normal_matrix = camera->normal(modelview.top());
  
  // Convert light position to eye coordinates, since lighting is done in eye coordinates
  glm::vec4 light_eye = modelview.top() * position;

  // Position of light source in eye coordinates
  program->setUniform("light.position", light_eye);

  // Ambient colour of light
  program->setUniform("light.ambient", glm::vec3(1.0f));

  // Diffuse colour of light
  program->setUniform("light.diffuse", glm::vec3(1.0f));

  // Specular colour of light
  program->setUniform("light.specular", glm::vec3(1.0f));

  // Ambient material reflectance
  program->setUniform("material.ambient", glm::vec3(1.0f));

  // Diffuse material reflectance
  program->setUniform("material.diffuse", glm::vec3(0.0f));

  // Specular material reflectance
  program->setUniform("material.specular", glm::vec3(0.0f));

  // Shininess material property
  program->setUniform("material.shininess", 15.0f);
}

void Lighting::diffuseSpecular(glutil::MatrixStack &, ShaderProgram *program)
{
  // Ambient material reflectance
 	program->setUniform("material.ambient", glm::vec3(0.5f));

  // Diffuse material reflectance
	program->setUniform("material.diffuse", glm::vec3(1.0f));

  // Specular material reflectance	
	program->setUniform("material.specular", glm::vec3(0.0f));
}