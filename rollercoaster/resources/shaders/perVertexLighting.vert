#version 330

// Structure for matrices
uniform struct Matrices
{
	mat4 projection;
	mat4 modelview;
	mat3 normal;
} matrices;

// Structure holding light information:  its position as well as ambient, diffuse, and specular colours
struct LightInfo
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
struct MaterialInfo
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

// Lights and materials passed in as uniform variables from client programme
uniform LightInfo light;
uniform MaterialInfo material;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 coord;
layout (location = 2) in vec3 normal;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
// Colour computed using reflectance model
out vec3 colour;

// Texture coordinate
out vec2 texture_coord;

// This function implements the Phong shading model
// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 2, pp. 62 - 63, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
vec3 phongModel(vec4 eye_position, vec3 eye_normal)
{
	vec3 s = normalize(vec3(light.position - eye_position));
	vec3 v = normalize(-eye_position.xyz);
	vec3 r = reflect(-s, eye_normal);

	vec3 ambient = light.ambient * material.ambient;
	float dot_product = max(dot(s, eye_normal), 0.0f);
	vec3 diffuse = light.diffuse * material.diffuse * dot_product;
	vec3 specular = vec3(0.0f);

  // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	float eps = 0.000001f;
	if (dot_product > 0.0f) {
		specular = light.specular * material.specular * pow(max(dot(r, v), 0.0f), material.shininess + eps);
  }
	
	return ambient + diffuse + specular;
}

// This is the entry point into the vertex shader
void main()
{	
	// Transform the vertex spatial position using 
	gl_Position = matrices.projection * matrices.modelview * vec4(position, 1.0f);
	
	// Get the vertex normal and vertex position in eye coordinates
	vec3 eye_normal = normalize(matrices.normal * normal);
	vec4 eye_position = matrices.modelview * vec4(position, 1.0f);
		
	// Apply the Phong model to compute the vertex colour
	colour = phongModel(eye_position, eye_normal);
	
	// Pass through the texture coordinate
	texture_coord = coord;
} 
