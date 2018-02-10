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
			(*values)[i - x][j - z] = (GLfloat)(GLint)((*values)[i - x][j - z] * (GLfloat)this->y);
			heightValues.push_back(glm::vec3(i, (*values)[i - x][j - z] + (GLint)15, j));
			for (k = (GLint)(*values)[i - x][j - z] - 1; k >= -5; k--) {
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
				if (values[i - x][j - y][k - z] >(GLfloat)GRAN) {
					position.push_back(glm::vec3(i, j, k));
				}
			}
		}
	}

	if (position.size() == 0) {
		std::cout << "Terrain Generator ERROR: Zero Positions Generated" << std::endl;
	}
	return position;
}

ComplexPosition TerrainGenerator::generateComplex(GLint x, GLint y, GLint z) {
	ComplexPosition CoPo;
	GLint i, j, k;
	GLint upperX = x + this->x;
	GLint upperY = y + this->y;
	GLint upperZ = z + this->z;
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> drawablePosition;

	GLfloat *** values;
	// Generate positions for the chunk the input parameters are in
	values = perlinNoise->generate(x / this->x, y / this->y, z / this->z);
	for (i = x; i < upperX; i++) {
		for (j = y; j < upperY; j++) {
			for (k = z; k < upperZ; k++) {
				if (values[i - x][j - y][k - z] >(GLfloat)GRAN) {
					position.push_back(glm::vec3(i, j, k));

					if (i == x || i == upperX - 1 || j == y || j == upperY - 1 || k == z || k == upperZ - 1) {
						drawablePosition.push_back(glm::vec3(i, j, k));
					}
				}
			}
		}
	}

	// TODO: First check if there are cubes touching the faces of the cube
	// And if there's one of each face then you don't have to check the rest because it'll be hiddem
	for (i = x + 1; i < upperX - 1; i++) {
		for (j = y + 1; j < upperY - 1; j++) {
			for (k = z + 1; k < upperZ - 1; k++) {
				if (values[i - x][j - y][k - z] >(GLfloat)GRAN) {

					if (!(values[i - x][j - y + 1][k - z] >(GLfloat)GRAN && values[i - x][j - y - 1][k - z] >(GLfloat)GRAN && values[i - x + 1][j - y][k - z] > (GLfloat)GRAN && values[i - x - 1][j - y][k - z] > (GLfloat)GRAN && values[i - x][j - y][k - z + 1] > (GLfloat)GRAN && values[i - x][j - y][k - z - 1] > (GLfloat)GRAN)) {
						drawablePosition.push_back(glm::vec3(i, j, k));
					}
					// Top																																																																																																																	   // Bottom																																																																																																															// Middle
					// This is the rest of checks for the cube. Though these are not necessary
					/*else if ((!(values[i - x + 1][j - y + 1][k - z] >(GLfloat)GRAN && values[i - x - 1][j - y + 1][k - z] >(GLfloat)GRAN && values[i - x][j - y + 1][k - z + 1] >(GLfloat)GRAN && values[i - x][j - y + 1][k - z - 1] >(GLfloat)GRAN && values[i - x + 1][j - y + 1][k - z + 1] >(GLfloat)GRAN && values[i - x + 1][j - y + 1][k - z - 1] >(GLfloat)GRAN && values[i - x - 1][j - y + 1][k - z + 1] >(GLfloat)GRAN && values[i - x - 1][j - y + 1][k - z - 1] >(GLfloat)GRAN)) || (!(values[i - x + 1][j - y - 1][k - z] >(GLfloat)GRAN && values[i - x - 1][j - y - 1][k - z] >(GLfloat)GRAN && values[i - x][j - y - 1][k - z + 1] >(GLfloat)GRAN && values[i - x][j - y - 1][k - z - 1] >(GLfloat)GRAN && values[i - x + 1][j - y - 1][k - z + 1] >(GLfloat)GRAN && values[i - x + 1][j - y - 1][k - z - 1] >(GLfloat)GRAN && values[i - x - 1][j - y - 1][k - z + 1] >(GLfloat)GRAN && values[i - x - 1][j - y - 1][k - z - 1] >(GLfloat)GRAN)) || (!(values[i - x + 1][j - y][k - z + 1] > (GLfloat)GRAN && values[i - x + 1][j - y][k - z - 1] > (GLfloat)GRAN && values[i - x - 1][j - y][k - z + 1] > (GLfloat)GRAN && values[i - x - 1][j - y][k - z + 1] > (GLfloat)GRAN))) {
						drawablePosition.push_back(glm::vec3(i, j, k));
					}*/
				}
			}
		}
	}

	if (position.size() == 0) {
		std::cout << "Terrain Generator ERROR: Zero Positions Generated" << std::endl;
	}
	CoPo.setPositions(position);
	CoPo.setDrawablePositions(drawablePosition);
	return CoPo;
}
