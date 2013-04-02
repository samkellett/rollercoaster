#version 330

// Structure for matrices
uniform struct Matrices {
	mat4 projection;
	mat4 modelview;
	mat3 normal;
} matrices;

// Structure holding light information:  its position as well as ambient, diffuse, and specular colours
struct LightInfo {
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
struct MaterialInfo {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform LightInfo sun;
uniform LightInfo light0, light1, light2, light3, light4;
uniform bool nighttime;

uniform MaterialInfo material;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 coord;
layout (location = 2) in vec3 normal;

out float height;
out vec3 colour;
out vec2 texture_coord;

vec3 phongModel(vec4 eye_position, vec3 eye_normal, LightInfo light)
{
	// This function implements the Phong shading model
	// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 2, pp. 62 - 63, with a few tweaks. 
	// Please see Chapter 2 of the book for a detailed discussion.
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

vec3 pointLight(vec3 position, vec3 normal, LightInfo light)
{
	// Adapted from: http://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
	float radius = 50.0;
	vec3 len = light.position.xyz - position;
	float distance = length(len);

	float d = max(distance - radius, 0);
	len /= distance;
	
	float attenuation = 1 / pow(d/radius + 1, 2);
	
	float cutoff = 0.005;
	attenuation = (attenuation - cutoff) / (1 - cutoff);
	attenuation = max(attenuation, 0);

	return light.ambient * max(dot(len, normal), 0) * attenuation;
}

void main()
{	
	gl_Position = matrices.projection * matrices.modelview * vec4(position, 1.0f);
	
	vec3 eye_normal = normalize(matrices.normal * normal);
	vec4 eye_position = matrices.modelview * vec4(position, 1.0f);
	
	height = position.y;

	vec3 night_colour = vec3(0.0);
	if (nighttime) {
		night_colour += pointLight(eye_position, eye_normal, light0);
		night_colour += pointLight(eye_position, eye_normal, light1);
		night_colour += pointLight(eye_position, eye_normal, light2);
		night_colour += pointLight(eye_position, eye_normal, light3);
		night_colour += pointLight(eye_position, eye_normal, light4);
	}

	colour = night_colour + phongModel(eye_position, eye_normal, sun);
	texture_coord = coord;
} 
