#pragma once

// GML
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

using namespace std;
using namespace glm;

// These Structs are Direct Copies of the Structs in the Fragment Shader
struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	GLfloat cutOff;
	GLfloat outerCutOff;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct LightsPropertiesContainer {
	DirLight * dirLight = nullptr;
	vector<PointLight> pointLights;
	SpotLight * spotLight = nullptr;
};
