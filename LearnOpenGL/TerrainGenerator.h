#pragma once

#include <iostream>
#include <fstream>

#include "PerlinNoise.h"

class TerrainGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator();

	TerrainGenerator(GLuint x, GLuint y);
	TerrainGenerator(GLuint x, GLuint y, GLuint z);

	// TODO: TerrainGenerator
	/* This class will generate the positions for the landscape 
	and will randomly generate grass on top of the terrain */
};

