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
	// TODO: Should only return one GLfloat
	/*
		This way we cut a looping through all the data once in TerrainGenerator
		We'd need better Chunk handling so it doesn't check the gradients everytime
	*/
	GLfloat *** generate(GLint x, GLint y, GLint z);

	GLboolean hasGenerated(GLint x, GLint y, GLint z);

private:
	// Size of chunk
	GLuint x, y, z;

	// TODO: This should take care of  storing the gradients and generating PerlinNoise values while TerrainGenerator should take care of storing all of the chunks and Positions
	// TODO: This was copied to TerrainGenerator.h. This should only be in one place
	const GLuint maxChunks = 100;
	const GLuint halfMaxChunks = ((maxChunks / 2) - 1);
	// Store each chunk's gradients
	glm::vec3 *** chunkGrads[100][100][100];
	
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

