#include "TerrainGenerator.h"


TerrainGenerator::TerrainGenerator(GLint x, GLint y, GLint z, GLuint terrainType) {
	this->x = x;
	this->y = y;
	this->z = z;

	if (terrainType == T_2D) {
		this->perlinNoise = new PerlinNoise(x, z);
	}
	else if (terrainType == T_3D) {
		this->chunkPositions = new ComplexPosition***[maxChunks];
		for (GLuint i = 0; i < maxChunks; i++) {
			this->chunkPositions[i] = new ComplexPosition**[maxChunks];
			for (GLuint j = 0; j < maxChunks; j++) {
				this->chunkPositions[i][j] = new ComplexPosition*[maxChunks];
				for (GLuint k = 0; k < maxChunks; k++) {
					this->chunkPositions[i][j][k] = nullptr;
				}
			}
		}
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
	GLint lowerX = getLowerVal(x, this->x);
	GLint lowerY = getLowerVal(y, this->y);
	GLint lowerZ = getLowerVal(z, this->z);
	GLint upperX = lowerX + this->x;
	GLint upperY = lowerY + this->y;
	GLint upperZ = lowerZ + this->z;
	std::vector<glm::vec3> position;

	GLfloat *** values;
	// Generate positions for the chunk the input parameters are in
	values = perlinNoise->generate(lowerX / this->x, lowerY / this->y, lowerZ / this->z);
	for (i = lowerX; i < upperX; i++) {
		for (j = lowerY; j < upperY; j++) {
			for (k = lowerZ; k < upperZ; k++) {
				if (values[i - lowerX][j - lowerY][k - lowerZ] >(GLfloat)GRAN) {
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
	ComplexPosition * CoPo = nullptr;
	GLint i, j, k;
	GLint lowerX = getLowerVal(x, this->x);
	GLint lowerY = getLowerVal(y, this->y);
	GLint lowerZ = getLowerVal(z, this->z);
	GLint upperX = lowerX + this->x;
	GLint upperY = lowerY + this->y;
	GLint upperZ = lowerZ + this->z;
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> drawablePosition;

	if (this->chunkPositions[lowerX / this->x + halfMaxChunks][lowerY / this->y + halfMaxChunks][lowerZ / this->z + halfMaxChunks] != nullptr) {
		return *this->chunkPositions[lowerX / this->x + halfMaxChunks][lowerY / this->y + halfMaxChunks][lowerZ / this->z + halfMaxChunks];
	}
	else {
		this->chunkPositions[lowerX / this->x + halfMaxChunks][lowerY / this->y + halfMaxChunks][lowerZ / this->z + halfMaxChunks] = new ComplexPosition();
		CoPo = this->chunkPositions[lowerX / this->x + halfMaxChunks][lowerY / this->y + halfMaxChunks][lowerZ / this->z + halfMaxChunks];
	}

	/*if (this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] != nullptr) {
		return *this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks];
	}*/

	GLfloat *** values;
	// Generate positions for the chunk the input parameters are in
	values = perlinNoise->generate(lowerX / this->x, lowerY / this->y, lowerZ / this->z);
	for (i = lowerX; i < upperX; i++) {
		for (j = lowerY; j < upperY; j++) {
			for (k = lowerZ; k < upperZ; k++) {
				if (values[i - lowerX][j - lowerY][k - lowerZ] >(GLfloat)GRAN) {
					position.push_back(glm::vec3(i, j, k));

					if (i == lowerX || i == upperX - 1 || j == lowerY || j == upperY - 1 || k == lowerZ || k == upperZ - 1) {
						drawablePosition.push_back(glm::vec3(i, j, k));
					}
				}
			}
		}
	}

	for (i = lowerX + 1; i < upperX - 1; i++) {
		for (j = lowerY + 1; j < upperY - 1; j++) {
			for (k = lowerZ + 1; k < upperZ - 1; k++) {
				if (values[i - lowerX][j - lowerY][k - lowerZ] >(GLfloat)GRAN) {

					if (!(values[i - lowerX][j - lowerY + 1][k - lowerZ] >(GLfloat)GRAN && values[i - lowerX][j - lowerY - 1][k - lowerZ] >(GLfloat)GRAN && values[i - lowerX + 1][j - lowerY][k - lowerZ] > (GLfloat)GRAN && values[i - lowerX - 1][j - lowerY][k - lowerZ] > (GLfloat)GRAN && values[i - lowerX][j - lowerY][k - lowerZ + 1] > (GLfloat)GRAN && values[i - lowerX][j - lowerY][k - lowerZ - 1] > (GLfloat)GRAN)) {
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
	(*CoPo).setPositions(position);
	(*CoPo).setDrawablePositions(drawablePosition);
	return *CoPo;
}

GLboolean TerrainGenerator::shouldGetNewChunks(glm::vec3 position) {
	glm::vec3 chunkPos = glm::vec3(getLowerVal(position.x, this->x), getLowerVal(position.y, this->y), getLowerVal(position.z, this->z));

	if (chunkPos.x == this->prevChunkPosition.x && chunkPos.y == this->prevChunkPosition.y && chunkPos.z == this->prevChunkPosition.z) {
		return 0;
	}
	this->prevChunkPosition = chunkPos;
	return 1;
}

GLint TerrainGenerator::getLowerVal(GLint val, GLint range) {
	if (val < 0 && val % range != 0) {
		val -= range;
	}
	return (val / range) * range;
}
