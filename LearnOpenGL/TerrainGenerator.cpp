#include "TerrainGenerator.h"



TerrainGenerator::TerrainGenerator()
{
}


TerrainGenerator::~TerrainGenerator()
{
}

TerrainGenerator::TerrainGenerator(GLuint x, GLuint y)
{
	PerlinNoise::generate(x, y);
}
