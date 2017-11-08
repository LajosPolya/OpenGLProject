#include "TerrainGenerator.h"


TerrainGenerator::TerrainGenerator(GLuint x, GLuint y, GLuint z, GLuint terrainType)
{
	this->x = x;
	this->y = y;
	this->z = z;

	if (terrainType == T_2D) {
		this->perlinNoise = new PerlinNoise(x, z);
	}
	else if (terrainType == T_3D) {
		this->perlinNoise = new PerlinNoise(x, y, z);
	}
	else {
		std::cout << "ERROR: Invalid Terrain Generator Type" << std::endl;
	}
}

TerrainGenerator::~TerrainGenerator() {
	delete this->perlinNoise;
}

std::vector<glm::vec3> TerrainGenerator::generate(GLuint x, GLuint z)
{
	GLuint i, j;
	GLint k; // Can't be unsigned because I'm subtracting 1 in the loop
	std::vector<glm::vec3> heightValues;

	GLfloat *** values = new GLfloat**();
	*values = perlinNoise->generate(this->x, this->z);

	for (i = 0; i < this->x; i++) {
		for (j = 0; j < this->z; j++) {
			(*values)[i][j] = (GLfloat)(GLint)((*values)[i][j] * (GLfloat)this->y);
			heightValues.push_back(glm::vec3(i, (*values)[i][j] + (GLint)15, j));
			for (k = (GLint)(*values)[i][j] - 1; k >= -5; k--) {
				heightValues.push_back(glm::vec3(i, k + 15, j));
			}
		}
	}

	return heightValues;
}

std::vector<glm::vec3> TerrainGenerator::generate(GLuint x, GLuint y, GLuint z)
{
	GLuint i, j, k;
	GLuint upperX = x + this->x;
	GLuint upperY = y + this->y;
	GLuint upperZ = z + this->z;
	std::vector<glm::vec3> position;

	GLfloat *** values;
	values = perlinNoise->generate(this->x, this->y, this->z);
	for (i = x; i < upperX; i++) {
		for (j = y; j < upperY; j++) {
			for (k = z; k < upperZ; k++) {
				if (values[i-x][j-y][k-z] >(GLfloat)0.1) {
					position.push_back(glm::vec3(i, j, k));
				}
			}
		}
	}

	return position;
}
