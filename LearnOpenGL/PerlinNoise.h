#pragma once

#include <iostream>

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


class PerlinNoise
{
public:
	PerlinNoise(GLuint x, GLuint y);
	PerlinNoise(GLuint x, GLuint y, GLuint z);
	GLfloat **  generate(GLint x, GLint y);

	// Starting GLfloat impl
	GLfloat generate(GLint x, GLint y, GLint z);

	void setChunk(glm::vec3 *** gradientVec);
	void setChunk(glm::vec2 ** gradientVec);

private:
	// Size of chunk
	GLuint x, y, z;
	
	glm::vec2 ** gradients;
	glm::vec3 *** gradients3d;

	GLfloat perlin(GLfloat x, GLfloat y);
	GLfloat perlin(GLfloat x, GLfloat y, GLfloat z);

	GLfloat fade(GLfloat val);
	GLfloat lerp(GLfloat x, GLfloat y, GLfloat w);
};

