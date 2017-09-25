#include "TerrainGenerator.h"


TerrainGenerator::TerrainGenerator() {}
TerrainGenerator::~TerrainGenerator() {}

void TerrainGenerator::generate(GLuint x, GLuint y, std::vector<glm::vec3> &heightValues)
{
	GLuint i, j;
	GLint k; // Can't be unsigned because I'm subtracting 1 in the loop

	GLfloat *** values = new GLfloat**();
	*values = PerlinNoise::generate(x, y);

	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			(*values)[i][j] = (GLfloat)(GLint)((*values)[i][j] * (GLfloat)10.0f);
			heightValues.push_back(glm::vec3(i, (*values)[i][j] + (GLint)10, j));
			for (k = (GLint)(*values)[i][j] - 1; k >= -5; k--) {
				heightValues.push_back(glm::vec3(i, k + 15, j));
			}
		}
	}
}

void TerrainGenerator::generate(GLuint x, GLuint y, GLuint z, std::vector<glm::vec3> &position)
{
	GLuint i, j, k;

	GLfloat *** values;
	values = PerlinNoise::generate(x, y, z);
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			for (k = 0; k < z; k++) {
				if (values[i][j][k] >(GLfloat)0.1) {
					position.push_back(glm::vec3(i, j, k + 50));
				}
			}
		}
	}
}


void TerrainGenerator::generate(GLuint x, GLuint y, GLchar * filename)
{
	GLuint i, j;
	GLint k; // Can't be unsigned because I'm subtracting 1 in the loop
	std::ofstream file;
	file.open(filename);

	GLfloat *** values = new GLfloat**();
	*values = PerlinNoise::generate(x, y);

	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			(*values)[i][j] = (GLfloat)(GLint)((*values)[i][j] * (GLfloat)10.0f);
			file << i << "," << (*values)[i][j] + (GLint)10 << "," << j << "," << 0.0 << "," << 0.0 << "," << 0.0 << "," << 1.0 << "," << 1.0 << "," << 1.0 << std::endl;
			for (k = (GLint)(*values)[i][j] - 1; k >= -5; k--) {
				file << i << "," << k + (GLint)15 << "," << j << "," << 0.0 << "," << 0.0 << "," << 0.0 << "," << 1.0 << "," << 1.0 << "," << 1.0 << std::endl;
			}
		}
	}

	file.close();


}

void TerrainGenerator::generate(GLuint x, GLuint y, GLuint z, GLchar * filename)
{
	GLuint i, j, k;
	std::ofstream file;
	file.open(filename);

	GLfloat *** values;
	values = PerlinNoise::generate(x, y, z);
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			for (k = 0; k < z; k++) {
				if (values[i][j][k] >(GLfloat)0.1) {
					file << i << "," << j << "," << k + 50 << "," << 0.0 << "," << 0.0 << "," << 0.0 << "," << 1.0 << "," << 1.0 << "," << 1.0 << std::endl;
				}
			}
		}
	}

	file.close();
}
