#include "rollercoaster.h"

#include "camera.h"
#include "catmullrom.h"
#include "point.h"
#include "vertex.h"

// Data
#include "resources/data/track.h"

const int Rollercoaster::FACES = 8;

Rollercoaster::Rollercoaster() :
  t_(0.0)
{
  for (int i = 0; i < sizeof(data::track) / sizeof(data::track_point); ++i) {
    data::track_point point = data::track[i];
    addDerivative(glm::vec3(point.x, point.y, point.z));
  }
}

Rollercoaster::~Rollercoaster()
{
  while (!dirivatives_.empty()) {
    delete dirivatives_.back();
    dirivatives_.pop_back();
  }
}

double Rollercoaster::t()
{
  return t_;
}

glm::vec3 Rollercoaster::point()
{
  return point(t_);
}

glm::vec3 Rollercoaster::point(double d)
{
  float total_length = distances_[distances_.size()-1];
  float length = (float) (d - (int) (d / total_length) * total_length);

  int segment;
  for (unsigned int i = 0; i < distances_.size(); i++) {
    if (length >= distances_[i] && length < distances_[i+1]) {
      segment = i;
      break;
    }
  }

  float segment_length =  distances_[segment + 1] - distances_[segment];  
  float t = (length - distances_[segment]) / segment_length;      
  
  int p0 = ((segment - 1) + dirivatives_.size()) % dirivatives_.size();
  int p1 = segment;
  int p2 = (segment + 1) % dirivatives_.size();
  int p3 = (segment + 2) % dirivatives_.size();

  return CatmullRom::interpolate(dirivatives_[p0], dirivatives_[p1], dirivatives_[p2], dirivatives_[p3], t);
}

void Rollercoaster::addDerivative(glm::vec3 position)
{
  dirivatives_.push_back(new Point(position, glm::vec3(1.0f, 0.0f, 0.0f)));
}

void Rollercoaster::init(ShaderProgram *program)
{
  GameObject::init(program);

  float length = 0.0f;
  distances_.push_back(length);

  for (unsigned int i = 1; i < dirivatives_.size(); i++) {
    length += glm::distance(dirivatives_[i-1]->position(), dirivatives_[i]->position());
    distances_.push_back(length);
  }

  length += glm::distance(dirivatives_[dirivatives_.size()-1]->position(), dirivatives_[0]->position());
  distances_.push_back(length);
  float total_length = distances_[distances_.size() - 1];

  float spacing = 6;
  float n = total_length / spacing;

  for (int i = 0; i < n; i++) {
    frames_.push_back(Frame(point(i * spacing), point((i + 1) * spacing)));
    Frame &frame = frames_.back();
    frame.generatePoints(FACES);

    if (i % 10 == 0) {
      supports_.push_back(new Support(point(i * spacing)));
    }
  }

  std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	for (unsigned int i = 0; i < frames_.size(); ++i) {
		for (unsigned int j = 0; j < FACES; ++j) {
			vertices.push_back(Vertex(frames_[i].points[j], glm::vec2(0), glm::vec3(0)));
    }
    vertices.push_back(Vertex(frames_[i].points[0], glm::vec2(0), glm::vec3(0)));
  }

  unsigned int height = frames_.size();
  unsigned int width = FACES + 1;
	for (unsigned int i = 0; i < height - 1; ++i) {
		for (unsigned int j = 0; j < width - 1; ++j) {
			int index = j + i * width;
			triangles.push_back(index);
			triangles.push_back(index + 1 + width);
			triangles.push_back(index + 1);

			triangles.push_back(index);
			triangles.push_back(index + width);
			triangles.push_back(index + 1 + width);
		}
	}

  mesh_.createFromTriangleList(vertices, triangles);
}

void Rollercoaster::update(glutil::MatrixStack &modelview, double dt)
{
  ++t_;
}

void Rollercoaster::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  Lighting::diffuseSpecular(program);

	modelview.translate(glm::vec3(0.0f, 0.0f, 0.0f));
	program->setUniform("matrices.modelview", modelview.top());
  program->setUniform("materials.ambient", glm::vec3(1, 1, 1));

	mesh_.render();

  for (unsigned int i = 0; i < supports_.size(); ++i) {
    modelview.push();
      program->setUniform("matrices.modelview", modelview.top());
      supports_[i]->render(modelview, program);
    modelview.pop();
  }
}