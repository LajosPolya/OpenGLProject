#pragma once

#include <iostream>
#include <fstream>
#include <cmath>

#include "PerlinNoise.h"
#include "ComplexPosition.h"

#define T_2D 2
#define T_3D 3
#define GRAN 0.09 // Granularity

class TerrainGenerator
{
public:
	TerrainGenerator(GLint x, GLint y, GLint z, GLuint terrainType);
	~TerrainGenerator();

	/* Params: a position in 2D or 3D space

	   The chunk of which the position is in will be generated
	*/
	std::vector<glm::vec3> generate(GLint x, GLint z);
	// TODO: This should take a glm::vec3 (will reduce programmer error and looks nicer)
	ComplexPosition generateComplex(glm::vec3 pos);

	GLboolean shouldGetNewChunks(glm::vec3 position);

	GLint getLowerVal(GLfloat val, GLint range);

	glm::vec3 getChunkPos(glm::vec3 pos);

private:
	// Size of one chunk of terrain
	GLint x, y, z;

	PerlinNoise * perlinNoise;

	glm::vec3 prevChunkPosition = glm::vec3(0, 0, 0);

	// TODO: This should take care of storing all of the chunks while PerlinNoise should take care of storing the gradients and generating values
	// TODO: This is a copy of PerlinNoise.h, this logic should be one
	const GLuint maxChunks = 100;
	const GLuint halfMaxChunks = ((maxChunks / 2) - 1);
	// Store pregenerated values
	ComplexPosition **** chunkPositions; // Triple Dynamic Array of a Pointer to ComplexPosition

	// TODO: TerrainGenerator
	/* This class will generate the positions for the landscape 
	and will randomly generate grass on top of the terrain */
};

