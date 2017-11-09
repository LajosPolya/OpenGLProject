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


class PerlinNoise
{
public:
	PerlinNoise(GLuint x, GLuint y);
	PerlinNoise(GLuint x, GLuint y, GLuint z);
	GLfloat **  generate(GLint x, GLint y);
	GLfloat *** generate(GLint x, GLint y, GLint z);

private:
	// Size of chunck
	GLuint x, y, z;

	const GLuint maxChuncks = 100;
	const GLuint halfMaxChuncks = ((maxChuncks / 2) - 1);
	// Store each chuncks gradients
	glm::vec3 *** chunckGrads[100][100][100];
	
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

