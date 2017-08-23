#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <random>
#include <iostream>


class PerlinNoise
{
public:
	PerlinNoise();
	PerlinNoise(GLuint x, GLuint y);
	~PerlinNoise();

private:
	glm::vec2 ** gradients;

	void genGradients(GLuint x, GLuint y);
	GLfloat perlin(GLfloat x, GLfloat y);
	GLfloat smooth(GLfloat dot1, GLfloat dot2, GLfloat weight);
};

