#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <random>

#include "ComplexPosition.h"

// TODO : The gradients need to be worked on. Do we really need 64? Should it be dynamic?
#define NUM_GRADS 64


class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

	GLboolean hasGenerated(GLint x, GLint y, GLint z);
	GLboolean hasGeneratedGradients(GLint x, GLint y, GLint z);

	glm::vec2 ** getGradients();
	glm::vec3 *** getChunk(GLint x, GLint y, GLint z);
	void setChunk(GLint x, GLint y, GLint z);

	ComplexPosition * getChunkPosition(GLint x, GLint y, GLint z);
	void setChunkPosition(ComplexPosition * CoPo, GLint x, GLint y, GLint z);

	void genGradients(GLuint x, GLuint y);
	void genGradients3d(GLuint x, GLuint y, GLuint z);

private:
	const GLdouble PI = 3.141592653589793;

	const GLuint maxChunks = 100;
	const GLuint halfMaxChunks = ((maxChunks / 2) - 1);
	// Store each chunk
	struct Chunk {
		glm::vec3 *** gradients; // A 3d array within a 3d array
		 // Store pregenerated values
		ComplexPosition * positions; // Triple Dynamic Array of a Pointer to ComplexPosition
	} ***chunks;

	// TODO :  this should be replaced with a local var or chunkGrads should be used instead
	glm::vec3 *** gradients3d;
	glm::vec2 ** gradients;

	/*
	Generates random vector by generating a random angle
	Having the length of the vector we can convert these polar coordinates into
	Cartesian coordinates
	*/
	glm::vec2 randomVector(GLfloat length);
	glm::vec3 random3DVector(GLfloat length);
};

