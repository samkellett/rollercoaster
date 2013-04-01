#include "terrain.h"

#include "game.h"
#include "window.h"
#include "vertex.h"

Terrain::Terrain() :
  dib_(NULL),  
  heightmap_filename_("resources/textures/heightmap.bmp"),
  
  origin_(0.0f, -10.0f, 0.0f),
  
  sizex_(1200.0f),
  sizez_(1200.0f),
  scaley_(20.5f),

  min_height_(1000000.0f),
  max_height_(-1000000.0f)
{
	BYTE *data;
	
	if (imageBytes(heightmap_filename_, &data, width_, height_) == false) {
    Window &window = Game::instance().window();
    MessageBox(window.hwnd(), "Couldn't find heightmap!", "Fatal Error", MB_ICONERROR);

		return;
  }

	// Allocate memory and initialize to store the image
	heightmap_ = new float[width_ * height_];
	if (heightmap_ == NULL) {
    Window &window = Game::instance().window();
    MessageBox(window.hwnd(), "Heightmap alloc error!", "Fatal Error", MB_ICONERROR);

		return;
  }

	// Clear the heightmap
	memset(heightmap_, 0, width_ * height_ * sizeof(float));

	// Form mesh
	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	for (unsigned int z = 0; z < height_; ++z) {
		for (unsigned int x = 0; x < width_; ++x) {
			int index = x + z * width_;

			// Retreive the colour from the terrain image, and set the normalized height in the range [0, 1]
			float gray = (data[index * 3] + data[index * 3 + 1] + data[index * 3 + 2]) / 3.0f;
			float height = (gray - 128.0f) / 128.0f;

			// Make a point based on this pixel position. Then, transform so that the mesh has the correct size and origin
			// This transforms a point in image coordinates to world coordinates
			glm::vec3 image = glm::vec3((float) x, height, (float) z);
			glm::vec3 world = toWorldCoordinates(image);

			// Scale the terrain and store for later
			world.y *= scaley_;
			heightmap_[index] = world.y;

      if (world.y > max_height_) {
        max_height_ = world.y;
      }

      if (world.y < min_height_) {
        min_height_ = world.y;
      }

			// Store the point in a vector
			Vertex v = Vertex(world, glm::vec2(0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
			vertices.push_back(v);
		}
	}

	FreeImage_Unload(dib_);

	// Form triangles from successive rows of the image
	for (unsigned int z = 0; z < height_-1; ++z) {
		for (unsigned int x = 0; x < width_-1; ++x) {
			int index = x + z * width_;
			triangles.push_back(index);
			triangles.push_back(index + 1 + width_);
			triangles.push_back(index + 1);

			triangles.push_back(index);
			triangles.push_back(index + width_);
			triangles.push_back(index + 1 + width_);
		}
	}

	// Create a face vertex mesh
	mesh_.createFromTriangleList(vertices, triangles);

	// Load a texture for texture mapping the mesh
  texture_images_.push_back("resources/textures/sand.jpg");
  texture_images_.push_back("resources/textures/grass.jpg");

  for (unsigned int i = 0; i < texture_images_.size(); ++i) {
    Texture texture;
    texture.load(texture_images_[i], true);

    textures_.push_back(texture);
  }
}

Terrain::~Terrain()
{
	delete[] heightmap_;
}

// Convert a point from image (pixel) coordinates to world coordinates
glm::vec3 Terrain::toWorldCoordinates(glm::vec3 point)
{
	// Normalize the image point so that it in the range [-1, 1] in x and [-1, 1] in z
	point.x = 2.0f * (point.x / width_) - 1.0f;
	point.z = 2.0f * (point.z / height_) - 1.0f;

	// Now scale the point so that the terrain has the right size in x and z
	point.x *= sizex_ / 2.0f;
	point.z *= sizez_ / 2.0f;

	// Now translate the point based on the origin passed into the function
	point += origin_;

	return point;
}

// Convert a point from world coordinates to image (pixel) coordinates
glm::vec3 Terrain::toImageCoordinates(glm::vec3 point)
{
	point -= origin_;

	// Normalize the image point so that it in the range [-1, 1] in x and [-1, 1] in z
	point.x *= 2.0f / sizex_;
	point.z *= 2.0f / sizez_;

	// Now transform the point so that it is in the range [0, 1] in x and [0, 1] in z
	point.x = (point.x + 1.0f) * (width_ / 2.0f);
	point.z = (point.z + 1.0f) * (height_ / 2.0f);

	return point;
}

bool Terrain::imageBytes(char *filename, BYTE **data, unsigned int &width, unsigned int &height)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

  // Check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);

	if(fif == FIF_UNKNOWN) {
    // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(filename);
    
    if(fif == FIF_UNKNOWN) {
      // If still unknown, return failure
		  return false;
    }
  }

	if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib_ = FreeImage_Load(fif, filename);

	if(!dib_) {
		char message[1024];
		sprintf_s(message, "Cannot load image\n%s\n", filename);
		MessageBox(NULL, message, "Error", MB_ICONERROR);
		return false;
	}

	*data = FreeImage_GetBits(dib_); // Retrieve the image data
	width = FreeImage_GetWidth(dib_);
	height = FreeImage_GetHeight(dib_);

	// If somehow one of these failed (they shouldn't), return failure
	if(data == NULL || width == 0 || height == 0) {
		return false;
  } else {
    return true;
  }
}

// For a point p in world coordinates, return the height of the terrain
float Terrain::height(glm::vec3 point)
{
	// Undo the transformation going from image coordinates to world coordinates
	glm::vec3 image = toImageCoordinates(point);

	// Bilinear interpolation. 
	int xl = (int) floor(image.x);
	int zl = (int) floor(image.z);

	// Check if the position is in the region of the heightmap
	if (xl < 0 || (unsigned int) xl >= width_ - 1 || zl < 0 || (unsigned int) zl >= height_ -1) {
		return 0.0f;
  }

	// Get the indices of four pixels around the current point 
	int indexll = xl + zl * width_;
	int indexlr = (xl + 1) + zl * width_;
	int indexul = xl + (zl + 1) * width_;
	int indexur = (xl + 1) + (zl + 1) * width_;

	// Interpolation amounts in x and z
	float dx = image.x - xl;
	float dz = image.z - zl;

	// Interpolate -- first in x and and then in z
	float a = (1 - dx) * heightmap_[indexll] + dx * heightmap_[indexlr];
	float b = (1 - dx) * heightmap_[indexul] + dx * heightmap_[indexur];
	float c = (1 - dz) * a + dz * b;
	
  return c;
}

std::string Terrain::program()
{
  return "terrain";
}

void Terrain::init(ShaderProgram *program)
{
  program->setUniform("sampler0", 0);
  program->setUniform("sampler1", 1);

  program->setUniform("min_height", min_height_);
  program->setUniform("max_height", max_height_);
}

void Terrain::update(glutil::MatrixStack &, double)
{
}

void Terrain::render(glutil::MatrixStack &modelview, ShaderProgram *program)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Lighting::diffuseSpecular(modelview, program);

	modelview.translate(glm::vec3(0.0f, 0.0f, 0.0f));
	program->setUniform("matrices.modelview", modelview.top());

	for (unsigned int i = 0; i < textures_.size(); ++i) {
    textures_[i].bind(i);
  }

	mesh_.render();

  glDisable(GL_BLEND);
}
