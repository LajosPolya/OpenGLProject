#pragma once

#include <iostream>
#include <fstream>

#include "PerlinNoise.h"

#define T_2D 2
#define T_3D 3

class TerrainGenerator
{
public:
	TerrainGenerator(GLuint x, GLuint y, GLuint z, GLuint terrainType);
	~TerrainGenerator();

	std::vector<glm::vec3> generate(GLuint x, GLuint z);
	std::vector<glm::vec3> generate(GLuint x, GLuint y, GLuint z);

private:
	GLuint x, y, z;
	PerlinNoise * perlinNoise;

	// TODO: TerrainGenerator
	/* This class will generate the positions for the landscape 
	and will randomly generate grass on top of the terrain */
};

