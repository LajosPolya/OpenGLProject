#pragma once

#include <iostream>
#include <fstream>

#include "PerlinNoise.h"

class TerrainGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator();

	static void generate(GLuint x, GLuint y, std::vector<glm::vec3> &heightValues);
	static void generate(GLuint x, GLuint y, GLuint z, std::vector<glm::vec3> &position);

	static void generate(GLuint x, GLuint y, GLchar * filename);
	static void generate(GLuint x, GLuint y, GLuint z, GLchar * filename);

	// TODO: TerrainGenerator
	/* This class will generate the positions for the landscape 
	and will randomly generate grass on top of the terrain */
};

