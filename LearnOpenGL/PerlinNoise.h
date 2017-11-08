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
	PerlinNoise(GLuint x, GLuint y);
	PerlinNoise(GLuint x, GLuint y, GLuint z);
	GLfloat **  generate(GLuint x, GLuint y);
	GLfloat *** generate(GLuint x, GLuint y, GLuint z);

private:
	// Size of chunck
	GLuint x, y, z;
	
	const GLdouble PI = 3.141592653589793;
	glm::vec2 ** gradients;
	glm::vec3 *** gradients3d;

	void genGradients(GLuint x, GLuint y);
	void genGradients3d(GLuint x, GLuint y, GLuint z);

	GLfloat perlin(GLfloat x, GLfloat y);
	GLfloat perlin(GLfloat x, GLfloat y, GLfloat z);

	glm::vec2 randomVector(GLfloat length);
	glm::vec3 random3DVector(GLfloat length);

	GLfloat fade(GLfloat val);

	GLfloat lerp(GLfloat x, GLfloat y, GLfloat w);
};

