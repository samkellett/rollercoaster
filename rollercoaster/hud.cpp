#include "hud.h"

#include "camera.h"
#include "window.h"

#include <sstream>

HUD::HUD()
{
  font_.loadSystemFont("arial.ttf", 32);
}

void HUD::update(glutil::MatrixStack &modelview, double dt)
{
}

void HUD::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{ 
  RECT dimensions = Game::instance().window().dimensions();
  int height = dimensions.bottom - dimensions.top;

  int fps = Game::instance().fps();
  if (fps > 0) {
    glDisable(GL_DEPTH_TEST);

    program->setUniform("matrices.modelview", glm::mat4(1));
    program->setUniform("matrices.projection", Game::instance().camera()->orthographicMatrix());
    program->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    font_.setShaderProgram(program);

    std::stringstream ss;
    ss << "FPS: " << fps;
    std::string msg = ss.str();
    font_.print(msg, height - 20, 20, msg.size());
    // font_.printf(20, height - 20, 20, "FPS: %d", fps);

    glEnable(GL_DEPTH_TEST);
  }
}

std::string HUD::program()
{
  return "fonts";
}
