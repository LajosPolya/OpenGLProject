#pragma once

#include <iostream>
#include <fstream>
#include <cmath>

#include "PerlinNoise.h"
#include "ComplexPosition.h"
#include "ChunkManager.h"
#include "CompositionType.h"

using namespace glm;

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
	vector<vec3> generate(GLint x, GLint z);

	ComplexPosition generateComplex(vec3 pos);

	GLboolean shouldGetNewChunks(vec3 position);

	GLint getLowerVal(GLfloat val, GLint range);

	vec3 getChunkPos(vec3 pos);

private:
	ChunkManager chunkManager;
	// Size of one chunk of terrain
	GLint x, y, z;

	PerlinNoise * perlinNoise;

	vec3 prevChunkPosition = vec3(0, 0, 0);

	// TODO: TerrainGenerator
	/* This class will generate the positions for the landscape 
	and will randomly generate grass on top of the terrain */
};

