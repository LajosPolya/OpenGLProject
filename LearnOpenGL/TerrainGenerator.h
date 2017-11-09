#pragma once

#include <iostream>
#include <fstream>

#include "PerlinNoise.h"

#define T_2D 2
#define T_3D 3

class TerrainGenerator
{
public:
	TerrainGenerator(GLint x, GLint y, GLint z, GLuint terrainType);
	~TerrainGenerator();

	std::vector<glm::vec3> generate(GLint x, GLint z);
	std::vector<glm::vec3> generate(GLint x, GLint y, GLint z);

private:
	GLint x, y, z;
	PerlinNoise * perlinNoise;

	// TODO: TerrainGenerator
	/* This class will generate the positions for the landscape 
	and will randomly generate grass on top of the terrain */
};

