#include "TerrainGenerator.h"


TerrainGenerator::TerrainGenerator(){}
TerrainGenerator::~TerrainGenerator(){}

TerrainGenerator::TerrainGenerator(GLuint x, GLuint y)
{
	GLuint i, j;
	GLint k; // Can't be unsigned because I'm subtracting 1 in the loop
	std::ofstream file;
	file.open("Perlin/perlin.txt");

	GLfloat ** values;
	values = PerlinNoise::generate(x, y);

	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			values[i][j] = (GLint)(values[i][j] * (GLfloat)10.0);
			file << i << "," << values[i][j] + (GLint)10 << "," << j << "," << 0.0 << "," << 0.0 << "," << 0.0 << "," << 1.0 << "," << 1.0 << "," << 1.0 << std::endl;
			for (k = (GLint)values[i][j] - 1; k >= -5; k--) {
				file << i << "," << k + (GLint)10 << "," << j << "," << 0.0 << "," << 0.0 << "," << 0.0 << "," << 1.0 << "," << 1.0 << "," << 1.0 << std::endl;
			}
		}
	}

	file.close();


}

TerrainGenerator::TerrainGenerator(GLuint x, GLuint y, GLuint z)
{
	GLuint i, j, k;
	std::ofstream file;
	file.open("Perlin/perlin3d.txt");

	GLfloat *** values;
	values = PerlinNoise::generate(x, y, z);
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			for (k = 0; k < z; k++) {
				if (values[i][j][k] >(GLfloat)0.1) {
					file << i << "," << j + (GLint)10 << "," << k + 50 << "," << 0.0 << "," << 0.0 << "," << 0.0 << "," << 1.0 << "," << 1.0 << "," << 1.0 << std::endl;
				}
			}
		}
	}

	file.close();
}
