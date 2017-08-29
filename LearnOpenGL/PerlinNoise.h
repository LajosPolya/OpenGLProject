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
#include <fstream>
#include <math.h>


class PerlinNoise
{
public:
	PerlinNoise();
	PerlinNoise(GLuint x, GLuint y);
	~PerlinNoise();

private:
	const double PI = 3.141592653589793;
	glm::vec2 ** gradients;
	GLfloat ** values;

	GLint maxX, maxY;

	void genGradients(GLuint x, GLuint y);
	GLfloat perlin(GLfloat x, GLfloat y);

	GLfloat perlin(GLint x, GLint y, GLfloat xVal, GLfloat yVal);

	glm::vec2 randomVector(GLfloat length);
};

