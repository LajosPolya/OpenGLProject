#include "TerrainGenerator.h"


TerrainGenerator::TerrainGenerator(GLint x, GLint y, GLint z, GLuint terrainType) {
	this->x = x;
	this->y = y;
	this->z = z;

	if (terrainType == T_2D) {
		// TODO: Remove this NUM_GRADS. This shouldn't be in this class
		this->chunkManager.genGradients(NUM_GRADS, NUM_GRADS);
		this->perlinNoise = new PerlinNoise(x, z);
		this->perlinNoise->setChunk(this->chunkManager.getGradients());
	}
	else if (terrainType == T_3D) {
		this->perlinNoise = new PerlinNoise(x, y, z);
	}
	else {
		std::cout << "WARNING: Invalid Terrain Generator Type" << std::endl;
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

ComplexPosition TerrainGenerator::generateComplex(glm::vec3 pos) {
	ComplexPosition * CoPo = nullptr;
	GLint i, j, k;
	GLint lowerX = getLowerVal(pos.x, this->x);
	GLint lowerY = getLowerVal(pos.y, this->y);
	GLint lowerZ = getLowerVal(pos.z, this->z);
	GLint upperX = lowerX + this->x;
	GLint upperY = lowerY + this->y;
	GLint upperZ = lowerZ + this->z;
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> drawablePosition;

	if (this->chunkManager.hasGenerated(lowerX / this->x, lowerY / this->y, lowerZ / this->z) == 1) {
		return *this->chunkManager.getChunkPosition(lowerX / this->x, lowerY / this->y, lowerZ / this->z);
	}
	else {
		CoPo = new ComplexPosition();
		this->chunkManager.setChunkPosition(CoPo, lowerX / this->x, lowerY / this->y, lowerZ / this->z);
	}

	this->chunkManager.setChunk(lowerX / this->x, lowerY / this->y, lowerZ / this->z);
	// Set the gradients for PerlinNoise
	perlinNoise->setChunk(chunkManager.getChunk(lowerX / this->x, lowerY / this->y, lowerZ / this->z));
	GLfloat *** values;
	// Generate positions for the chunk the input parameters are in
	values = new GLfloat**[this->x];
	for (i = lowerX; i < upperX; i++) {
		values[i - lowerX] = new GLfloat*[this->y];
		for (j = lowerY; j < upperY; j++) {
			values[i - lowerX][j - lowerY] = new GLfloat[this->z];
			for (k = lowerZ; k < upperZ; k++) {
				values[i - lowerX][j - lowerY][k - lowerZ] = perlinNoise->generate(i - lowerX, j - lowerY, k - lowerZ);
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

	for (GLint i = 0; i < this->x; i++) {
		for (GLint j = 0; j < this->y; j++) {
			//for (GLint k = 0; k < this->z; k++) {
			delete values[i][j];
			//}
		}
		delete values[i];
	}
	delete values;

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

/// https://stackoverflow.com/questions/48897886/clamping-to-next-lowest-value-in-a-series?noredirect=1#comment84802282_48897886
GLint TerrainGenerator::getLowerVal(GLfloat val, GLint range) {
	GLint floorVal = (GLint)std::floor(val);
	if (floorVal < 0) {
		floorVal = floorVal - range + 1;
	}
	return (floorVal / range) * range;
}

glm::vec3 TerrainGenerator::getChunkPos(glm::vec3 pos) {
	pos.x = (GLfloat)getLowerVal(pos.x, this->x);
	pos.y = (GLfloat)getLowerVal(pos.y, this->y);
	pos.z = (GLfloat)getLowerVal(pos.z, this->z);
	return pos;
}
