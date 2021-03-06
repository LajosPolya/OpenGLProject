#include "ChunkManager.h"



ChunkManager::ChunkManager() {
	if (NUM_GRADS < GRANULARITY) {
		cout << "ERROR:: NUM_GRADS CANNOT BE LESS THAN GRANULARITY" << endl;
	}
	this->chunks = new Chunk**[maxChunks];
	for (GLuint i = 0; i < maxChunks; i++) {
		this->chunks[i] = new Chunk*[maxChunks];
		for (GLuint j = 0; j < maxChunks; j++) {
			this->chunks[i][j] = new Chunk[maxChunks];
			for (GLuint k = 0; k < maxChunks; k++) {
				this->chunks[i][j][k].positions = nullptr;
				this->chunks[i][j][k].gradients = nullptr;
				this->chunks[i][j][k].granularity = 2;
			}
		}
	}
}
ChunkManager::~ChunkManager() {}

GLboolean ChunkManager::hasGenerated(GLint x, GLint y, GLint z) {
	if (this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks].gradients != nullptr && this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks].positions != nullptr) {
		return 1;
	}
	else if (this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks].gradients == nullptr && this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks].positions == nullptr) {
		return 0;
	}
	else {
		cout << "ERROR::ChunkManager:: INCONSISTENCY BETWEEN chunks && chunkPositions" << endl;
	}

	return 0;
}

GLboolean ChunkManager::hasGeneratedGradients(GLint x, GLint y, GLint z) {
	if (this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks].gradients != nullptr) {
		return 1;
	}
	return 0;
}

vec2 ** ChunkManager::getGradients() {
	return this->gradients;
}

Chunk ChunkManager::getChunk(GLint x, GLint y, GLint z) {
	return this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks];
}

void ChunkManager::setChunk(GLint x, GLint y, GLint z) {
	GLuint i, j, k;

	if (hasGeneratedGradients(x, y, z) == false) {
		genGradients3d(NUM_GRADS, NUM_GRADS, NUM_GRADS);
	}

	// TODO: Do all of these granularities need to be copied over?
	if (hasGeneratedGradients(x, y, z - 1) == true) {
		Chunk chunk = getChunk(x, y, z - 1);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[i][j][0] = chunk.gradients[i][j][1 * chunk.granularity];
			}
		}
	}

	if (hasGeneratedGradients(x, y, z + 1) == true) {
		Chunk chunk = getChunk(x, y, z + 1);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[i][j][1 * chunk.granularity] = chunk.gradients[i][j][0];
			}
		}
	}

	if (hasGeneratedGradients(x - 1, y, z) == true) {
		Chunk chunk = getChunk(x - 1, y, z);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[0][i][j] = chunk.gradients[1 * chunk.granularity][i][j];
			}
		}
	}

	if (hasGeneratedGradients(x + 1, y, z) == true) {
		Chunk chunk = getChunk(x + 1, y, z);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[1 * chunk.granularity][i][j] = chunk.gradients[0][i][j];
			}
		}
	}

	if (hasGeneratedGradients(x, y - 1, z) == true) {
		Chunk chunk = getChunk(x, y - 1, z);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[i][0][j] = chunk.gradients[i][1 * chunk.granularity][j];
			}
		}
	}

	if (hasGeneratedGradients(x, y + 1, z) == true) {
		Chunk chunk = getChunk(x, y + 1, z);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[i][1 * chunk.granularity][j] = chunk.gradients[i][0][j];
			}
		}
	}

	// Positions relative to full world
	GLint rx = x + halfMaxChunks;
	GLint ry = y + halfMaxChunks;
	GLint rz = z + halfMaxChunks;
	this->chunks[rx][ry][rz].gradients= new vec3**[NUM_GRADS];
	for (i = 0; i < NUM_GRADS; i++) {
		this->chunks[rx][ry][rz].gradients[i] = new vec3*[NUM_GRADS];
		for (j = 0; j < NUM_GRADS; j++) {
			this->chunks[rx][ry][rz].gradients[i][j] = new vec3[NUM_GRADS];
			for (k = 0; k < NUM_GRADS; k++) {
				this->chunks[rx][ry][rz].gradients[i][j][k] = this->gradients3d[i][j][k];
			}
		}
	}
}

ComplexPosition * ChunkManager::getChunkPosition(GLint x, GLint y, GLint z) {
	return this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks].positions;
}

void ChunkManager::setChunkPosition(ComplexPosition * CoPo, GLint x, GLint y, GLint z) {
	this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks].positions = CoPo;
}

void ChunkManager::genGradients(GLuint x, GLuint y) {
	GLuint i, j;

	this->gradients = new vec2*[x];
	for (i = 0; i < x; i++) {
		this->gradients[i] = new vec2[y];
		for (j = 0; j < y; j++) {
			this->gradients[i][j] = randomVector((GLfloat)1.0);
		}
	}

	this->gradients[0][0] = { 1.0, 0.0 };
	this->gradients[1][0] = { 0.0, 1.0 };
	this->gradients[0][1] = { -1.0, 0.0 };
	this->gradients[1][1] = { 0.0, -1.0 };
}

void ChunkManager::genGradients3d(GLuint x, GLuint y, GLuint z) {
	this->gradients3d = new vec3**[x];
	for (GLuint i = 0; i < x; i++) {
		this->gradients3d[i] = new vec3*[y];
		for (GLuint j = 0; j < y; j++) {
			this->gradients3d[i][j] = new vec3[z];
			for (GLuint k = 0; k < z; k++) {
				this->gradients3d[i][j][k] = random3DVector((GLfloat)1.0);
			}
		}
	}
}

vec2 ChunkManager::randomVector(GLfloat length) {
	random_device rd;
	uniform_real_distribution<GLdouble> dist{ 0.0, 2.0 * PI };
	GLfloat angle = (GLfloat)dist(rd);

	// Turn angle and length into vector
	// The length of the vector is implicitly 1
	GLfloat x = length * cos(angle);
	GLfloat y = length * sin(angle);
	return{ x, y };
}


// TODO: Consider Grabbing random vectors from pregenerated vectors;
/*
I used to generate 64^3 gradient vectors/chunk, this was extremely slow.
Now I only generate 8^3, which is now way faster, but grabing vectors from 
a pregenerated map of 2^8 would greatly increase speed (if it's necessary)
*/
vec3 ChunkManager::random3DVector(GLfloat length) {

	random_device rd;
	uniform_real_distribution<GLdouble> dist{ 0.0, 2.0 * PI };
	uniform_real_distribution<GLfloat> randomCostheta{ -1.0, 1.0 };
	uniform_real_distribution<GLfloat> randomU{ 0.0, 1.0 };
	GLfloat phi = (GLfloat)dist(rd);
	GLfloat costheta = randomCostheta(rd);

	GLfloat theta = acos(costheta);

	length = length * cbrt(randomU(rd));

	// Turn angle and length into vector
	// The length of the vector is implicitly 1
	GLfloat x = length * sin(theta) * cos(phi);
	GLfloat y = length * sin(theta) * sin(phi);
	GLfloat z = length * cos(theta);
	return{ x, y, z };
}
