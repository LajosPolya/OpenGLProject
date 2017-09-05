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
#include <math.h>

/* No need to make class static since static keyword doesn't do anything on classes */
/*static*/class PerlinNoise
{
public:
	static GLfloat **  generate(GLuint x, GLuint y);

private:
	static const double PI;
	static glm::vec2 ** gradients;
	static GLfloat ** values;

	static GLint maxX, maxY;

	static void genGradients(GLuint x, GLuint y);

	static GLfloat perlin(GLfloat x, GLfloat y);

	static GLfloat perlin(GLfloat x, GLfloat y, GLfloat z);

	static 	GLfloat perlin(GLint x, GLint y, GLfloat xVal, GLfloat yVal);

	static glm::vec2 randomVector(GLfloat length);

	static glm::vec3 random3DVector(GLfloat length);

	static GLfloat fade(GLfloat val);

	static GLfloat lerp(GLfloat x, GLfloat y, GLfloat w);
};

