#include "TerrainGenerator.h"


TerrainGenerator::TerrainGenerator(GLint x, GLint y, GLint z, GLuint terrainType) {
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

std::vector<glm::vec3> TerrainGenerator::generate(GLint x, GLint z) {
	GLint i, j;
	GLint k; // Can't be unsigned because I'm subtracting 1 in the loop
	GLint upperX = x + this->x;
	GLint upperZ = z + this->z;
	std::vector<glm::vec3> heightValues;

	GLfloat *** values = new GLfloat**();
	*values = perlinNoise->generate(this->x, this->z);

	for (i = x; i < upperX; i++) {
		for (j = z; j < upperZ; j++) {
			(*values)[i-x][j-z] = (GLfloat)(GLint)((*values)[i-x][j-z] * (GLfloat)this->y);
			heightValues.push_back(glm::vec3(i, (*values)[i-x][j-z] + (GLint)15, j));
			for (k = (GLint)(*values)[i-x][j-z] - 1; k >= -5; k--) {
				heightValues.push_back(glm::vec3(i, k + 15, j));
			}
		}
	}

	return heightValues;
}

std::vector<glm::vec3> TerrainGenerator::generate(GLint x, GLint y, GLint z) {
	GLint i, j, k;
	GLint upperX = x + this->x;
	GLint upperY = y + this->y;
	GLint upperZ = z + this->z;
	std::vector<glm::vec3> position;

	GLfloat *** values;
	// Generate positions for the chunk the input parameters are in
	values = perlinNoise->generate(x / this->x, y / this->y, z / this->z);
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
