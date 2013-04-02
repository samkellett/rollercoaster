#include "lighting.h"

#include <sstream>

#include "common.h"
#include "game.h"
#include "camera.h"
#include "shaderprogram.h"

std::vector<glm::vec3> Lighting::sources_;
std::vector<glm::vec3> Lighting::colours_;

void Lighting::global(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  glm::vec3 light(1.0f);
  if (Game::instance().nighttime()) {
    light = glm::vec3(0.20f);

    // Point lights:
    for(unsigned int i = 0; i < Lighting::sources_.size(); ++i) {
      glm::vec4 position = glm::vec4(Lighting::sources_[i], 1.0f);

      std::ostringstream oss;
      oss << "light" << i;
      std::string name = oss.str();

      program->setUniform(name + ".position", modelview.top() * position);
      program->setUniform(name + ".ambient", Lighting::colours_[i]);
    }
  }

  Camera *camera = Game::instance().camera();

  glm::vec4 position(0, -150, 0, 1);
  glm::vec4 light_eye = modelview.top() * position;

  program->setUniform("sun.position", light_eye);
  program->setUniform("sun.ambient", light);
  program->setUniform("sun.diffuse", light);
  program->setUniform("sun.specular", light);

  program->setUniform("material.ambient", light);
  program->setUniform("material.diffuse", glm::vec3(0.0f));
  program->setUniform("material.specular", glm::vec3(0.0f));
  program->setUniform("material.shininess", 15.0f);
}

void Lighting::diffuseSpecular(ShaderProgram *program, float specular, float diffuse, float ambient)
{
 	program->setUniform("material.ambient", glm::vec3(ambient));
	program->setUniform("material.diffuse", glm::vec3(diffuse));
	program->setUniform("material.specular", glm::vec3(specular));
}

void Lighting::registerSource(glm::vec3 position, glm::vec3 colour)
{
  Lighting::sources_.push_back(position);
  Lighting::colours_.push_back(colour);
}