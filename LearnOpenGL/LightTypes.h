#pragma once

// GML
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// These Structs are Direct Copies of the Structs in the Fragment Shader
struct DirLight {
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight {
	glm::vec3 position;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct SpotLight {
	glm::vec3 position;
	glm::vec3 direction;
	GLfloat cutOff;
	GLfloat outerCutOff;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct LightsPropertiesContainer {
	DirLight * dirLight = nullptr;
	std::vector<PointLight> pointLights;
	SpotLight * spotLight = nullptr;
};
