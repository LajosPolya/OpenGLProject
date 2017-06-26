#pragma once

// STD
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Shader.h"

// GML
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// These Structs are Direct Copies of the Structs in the Fragment Shader

typedef struct DirLight {
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
} DirLight;

typedef struct PointLight {
	glm::vec3 position;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	GLboolean isInit;
} PointLight;

typedef struct SpotLight {
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
} SpotLight;
