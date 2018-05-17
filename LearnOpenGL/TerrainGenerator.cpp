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
	glm::vec3 lower(getLowerVal(pos.x, this->x), getLowerVal(pos.y, this->y), getLowerVal(pos.z, this->z));
	glm::vec3 upper((int)lower.x + this->x, (int)lower.y + this->y, (int)lower.z + this->z);
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> drawablePosition;

	if (this->chunkManager.hasGenerated((int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z) == 1) {
		return *this->chunkManager.getChunkPosition((int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z);
	}
	else {
		CoPo = new ComplexPosition();
		this->chunkManager.setChunkPosition(CoPo, (int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z);
	}

	this->chunkManager.setChunk((int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z);
	// Set the gradients for PerlinNoise
	perlinNoise->setChunk(chunkManager.getChunk((int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z));
	GLfloat *** values;
	// Generate positions for the chunk the input parameters are in
	values = new GLfloat**[this->x];
	for (i = (int)lower.x; i < (int)upper.x; i++) {
		values[i - (int)lower.x] = new GLfloat*[this->y];
		for (j = (int)lower.y; j < (int)upper.y; j++) {
			values[i - (int)lower.x][j - (int)lower.y] = new GLfloat[this->z];
			for (k = (int)lower.z; k < (int)upper.z; k++) {
				values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z] = perlinNoise->generate(i - (int)lower.x, j - (int)lower.y, k - (int)lower.z);
				if (values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z] >(GLfloat)GRAN) {
					position.push_back(glm::vec3(i, j, k));

					if (i == (int)lower.x || i == (int)upper.x - 1 || j == (int)lower.y || j == (int)upper.y - 1 || k == (int)lower.z || k == (int)upper.z - 1) {
						drawablePosition.push_back(glm::vec3(i, j, k));
					}
				}
			}
		}
	}

	for (i = (int)lower.x + 1; i < (int)upper.x - 1; i++) {
		for (j = (int)lower.y + 1; j < (int)upper.y - 1; j++) {
			for (k = (int)lower.z + 1; k < (int)upper.z - 1; k++) {
				if (values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z] >(GLfloat)GRAN) {

					if (!(values[i - (int)lower.x][j - (int)lower.y + 1][k - (int)lower.z] >(GLfloat)GRAN && values[i - (int)lower.x][j - (int)lower.y - 1][k - (int)lower.z] >(GLfloat)GRAN && values[i - (int)lower.x + 1][j - (int)lower.y][k - (int)lower.z] > (GLfloat)GRAN && values[i - (int)lower.x - 1][j - (int)lower.y][k - (int)lower.z] > (GLfloat)GRAN && values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z + 1] > (GLfloat)GRAN && values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z - 1] > (GLfloat)GRAN)) {
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
