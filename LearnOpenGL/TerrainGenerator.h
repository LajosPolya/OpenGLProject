#pragma once

#include <iostream>
#include <fstream>

#include "PerlinNoise.h"
#include "ComplexPosition.h"

#define T_2D 2
#define T_3D 3
#define GRAN 0.01 // Granularity

class TerrainGenerator
{
public:
	TerrainGenerator(GLint x, GLint y, GLint z, GLuint terrainType);
	~TerrainGenerator();

	/* Params: a position in 2D or 3D space

	   The chunk of which the position is in will be generated
	*/
	std::vector<glm::vec3> generate(GLint x, GLint z);
	std::vector<glm::vec3> generate(GLint x, GLint y, GLint z);
	ComplexPosition generateComplex(GLint x, GLint y, GLint z);

	GLboolean hasGenerated(GLint x, GLint y, GLint z);

	GLboolean shouldGetNewChunks(glm::vec3 position);

	GLint getLowerVal(GLint val, GLint range);

private:
	// Size of one chunk of terrain
	GLint x, y, z;

	PerlinNoise * perlinNoise;

	glm::vec3 prevChunkPosition = glm::vec3(0, 0, 0);

	// TODO: TerrainGenerator
	/* This class will generate the positions for the landscape 
	and will randomly generate grass on top of the terrain */
};

