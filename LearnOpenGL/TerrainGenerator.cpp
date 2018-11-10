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
		cout << "WARNING: Invalid Terrain Generator Type" << endl;
	}
}

TerrainGenerator::~TerrainGenerator() {
	delete this->perlinNoise;
}

vector<vec3> TerrainGenerator::generate(GLint x, GLint z) {
	GLint i, j;
	GLint k; // Can't be unsigned because I'm subtracting 1 in the loop
	GLint upperX = x + this->x;
	GLint upperZ = z + this->z;
	vector<vec3> heightValues;

	GLfloat *** values = new GLfloat**();
	*values = perlinNoise->generate(this->x, this->z);

	for (i = x; i < upperX; i++) {
		for (j = z; j < upperZ; j++) {
			(*values)[i - x][j - z] = (GLfloat)(GLint)((*values)[i - x][j - z] * (GLfloat)this->y);
			heightValues.push_back(vec3(i, (*values)[i - x][j - z] + (GLint)15, j));
			for (k = (GLint)(*values)[i - x][j - z] - 1; k >= -5; k--) {
				heightValues.push_back(vec3(i, k + 15, j));
			}
		}
	}

	return heightValues;
}

ComplexPosition TerrainGenerator::generateComplex(vec3 pos) {
	// TODO: This should be managed completely in ChunkManaget (perhaps unique pointer)
	ComplexPosition * CoPo = nullptr; 
	GLint i, j, k;
	vec3 lower{ getLowerVal(pos.x, this->x), getLowerVal(pos.y, this->y), getLowerVal(pos.z, this->z) };
	vec3 upper{ (int)lower.x + this->x, (int)lower.y + this->y, (int)lower.z + this->z };
	vector<vector<vec3>> position(2);
	vector<vector<vec3>> drawablePosition(2);

	if (this->chunkManager.hasGenerated((int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z) == 1) {
		return *this->chunkManager.getChunkPosition((int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z);
	}

	this->chunkManager.setChunk((int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z); // Rename to generateGradients()
	Chunk chunk = chunkManager.getChunk((int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z);
	// Set the gradients for PerlinNoise
	perlinNoise->setChunk(chunk.gradients); // Rename to setGradients() or setChunkGradients()
	vector<vector<vector<GLfloat>>> values(this->x);
	// Generate positions for the chunk the input parameters are in
	for (i = (int)lower.x; i < (int)upper.x; i++) {
		values[i - (int)lower.x] = vector<vector<GLfloat>>(this->y);
		for (j = (int)lower.y; j < (int)upper.y; j++) {
			values[i - (int)lower.x][j - (int)lower.y] = vector<GLfloat>(this->z);
			for (k = (int)lower.z; k < (int)upper.z; k++) {
				values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z] = perlinNoise->generate(((GLfloat)i - lower.x) * (GLfloat)chunk.granularity, ((GLfloat)j - lower.y) * (GLfloat)chunk.granularity, ((GLfloat)k - lower.z) * (GLfloat)chunk.granularity);
				if (values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z] > (GLfloat)GRAN) {
					if (values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z] < GRAN * 1.3f) {
						position[GRASS].push_back(vec3(i, j, k));
					}
					else {
						position[COAL].push_back(vec3(i, j, k));
					}

					if (i == (int)lower.x || i == (int)upper.x - 1 || j == (int)lower.y || j == (int)upper.y - 1 || k == (int)lower.z || k == (int)upper.z - 1) {
						if (values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z] < GRAN * 1.3f) {
							drawablePosition[GRASS].push_back(vec3(i, j, k));
						}
						else {
							drawablePosition[COAL].push_back(vec3(i, j, k));
						}
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
						if (values[i - (int)lower.x][j - (int)lower.y][k - (int)lower.z] < GRAN * 1.3f) {
							drawablePosition[GRASS].push_back(vec3(i, j, k));
						}
						else {
							drawablePosition[COAL].push_back(vec3(i, j, k));
						}
					}
					// Top																																																																																																																	   // Bottom																																																																																																															// Middle
					// This is the rest of checks for the cube. Though these are not necessary
					/*else if ((!(values[i - x + 1][j - y + 1][k - z] >(GLfloat)GRAN && values[i - x - 1][j - y + 1][k - z] >(GLfloat)GRAN && values[i - x][j - y + 1][k - z + 1] >(GLfloat)GRAN && values[i - x][j - y + 1][k - z - 1] >(GLfloat)GRAN && values[i - x + 1][j - y + 1][k - z + 1] >(GLfloat)GRAN && values[i - x + 1][j - y + 1][k - z - 1] >(GLfloat)GRAN && values[i - x - 1][j - y + 1][k - z + 1] >(GLfloat)GRAN && values[i - x - 1][j - y + 1][k - z - 1] >(GLfloat)GRAN)) || (!(values[i - x + 1][j - y - 1][k - z] >(GLfloat)GRAN && values[i - x - 1][j - y - 1][k - z] >(GLfloat)GRAN && values[i - x][j - y - 1][k - z + 1] >(GLfloat)GRAN && values[i - x][j - y - 1][k - z - 1] >(GLfloat)GRAN && values[i - x + 1][j - y - 1][k - z + 1] >(GLfloat)GRAN && values[i - x + 1][j - y - 1][k - z - 1] >(GLfloat)GRAN && values[i - x - 1][j - y - 1][k - z + 1] >(GLfloat)GRAN && values[i - x - 1][j - y - 1][k - z - 1] >(GLfloat)GRAN)) || (!(values[i - x + 1][j - y][k - z + 1] > (GLfloat)GRAN && values[i - x + 1][j - y][k - z - 1] > (GLfloat)GRAN && values[i - x - 1][j - y][k - z + 1] > (GLfloat)GRAN && values[i - x - 1][j - y][k - z + 1] > (GLfloat)GRAN))) {
					drawablePosition.push_back(vec3(i, j, k));
					}*/
				}
			}
		}
	}

	if (position[GRASS].size() == 0) {
		cout << "Terrain Generator WARN: Zero Grass Positions Generated" << endl;
	}
	else if (position[COAL].size() == 0) {
		cout << "Terrain Generator WARN: Zero Coal Positions Generated" << endl;
	}

	CoPo = new ComplexPosition();
	CoPo->setPositions(position[GRASS], GRASS);
	CoPo->setDrawablePositions(drawablePosition[GRASS], GRASS);
	CoPo->setPositions(position[COAL], COAL);
	CoPo->setDrawablePositions(drawablePosition[COAL], COAL);
	this->chunkManager.setChunkPosition(CoPo, (int)lower.x / this->x, (int)lower.y / this->y, (int)lower.z / this->z);
	return *CoPo;
}

GLboolean TerrainGenerator::shouldGetNewChunks(vec3 position) {
	vec3 chunkPos{ getLowerVal(position.x, this->x), getLowerVal(position.y, this->y), getLowerVal(position.z, this->z) };

	if (chunkPos.x == this->prevChunkPosition.x && chunkPos.y == this->prevChunkPosition.y && chunkPos.z == this->prevChunkPosition.z) {
		return 0;
	}
	this->prevChunkPosition = chunkPos;
	return 1;
}

/// https://stackoverflow.com/questions/48897886/clamping-to-next-lowest-value-in-a-series?noredirect=1#comment84802282_48897886
GLint TerrainGenerator::getLowerVal(GLfloat val, GLint range) {
	GLint floorVal = (GLint)floor(val);
	if (floorVal < 0) {
		floorVal = floorVal - range + 1;
	}
	return (floorVal / range) * range;
}

vec3 TerrainGenerator::getChunkPos(vec3 pos) {
	pos.x = (GLfloat)getLowerVal(pos.x, this->x);
	pos.y = (GLfloat)getLowerVal(pos.y, this->y);
	pos.z = (GLfloat)getLowerVal(pos.z, this->z);
	return pos;
}
