#include "ChunkManager.h"



ChunkManager::ChunkManager() {
	this->chunks = new Chunk**[maxChunks];
	for (GLuint i = 0; i < maxChunks; i++) {
		this->chunks[i] = new Chunk*[maxChunks];
		for (GLuint j = 0; j < maxChunks; j++) {
			this->chunks[i][j] = new Chunk[maxChunks];
			for (GLuint k = 0; k < maxChunks; k++) {
				this->chunks[i][j][k].positions = nullptr;
				this->chunks[i][j][k].gradients = nullptr;
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
		std::cout << "ERROR::ChunkManager:: INCONSISTENCY BETWEEN chunks && chunkPositions" << std::endl;
	}

	return 0;
}

GLboolean ChunkManager::hasGeneratedGradients(GLint x, GLint y, GLint z) {
	if (this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks].gradients!= nullptr) {
		return 1;
	}
	return 0;
}

glm::vec2 ** ChunkManager::getGradients() {
	return this->gradients;
}

glm::vec3 *** ChunkManager::getChunk(GLint x, GLint y, GLint z) {
	return this->chunks[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks].gradients;
}

void ChunkManager::setChunk(GLint x, GLint y, GLint z) {
	GLuint i, j, k;

	if (hasGeneratedGradients(x, y, z) == 0) {
		genGradients3d(NUM_GRADS, NUM_GRADS, NUM_GRADS);
	}

	if (hasGeneratedGradients(x, y, z - 1) == 1) {
		glm::vec3 *** gradients = getChunk(x, y, z - 1);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[i][j][0] = gradients[i][j][1 * GRANULARITY];
			}
		}
	}

	if (hasGeneratedGradients(x, y, z + 1) == 1) {
		glm::vec3 *** gradients = getChunk(x, y, z + 1);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[i][j][1 * GRANULARITY] = gradients[i][j][0];
			}
		}
	}

	if (hasGeneratedGradients(x - 1, y, z) == 1) {
		glm::vec3 *** gradients = getChunk(x - 1, y, z);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[0][i][j] = gradients[1 * GRANULARITY][i][j];
			}
		}
	}

	if (hasGeneratedGradients(x + 1, y, z) == 1) {
		glm::vec3 *** gradients = getChunk(x + 1, y, z);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[1 * GRANULARITY][i][j] = gradients[0][i][j];
			}
		}
	}

	if (hasGeneratedGradients(x, y - 1, z) == 1) {
		glm::vec3 *** gradients = getChunk(x, y - 1, z);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[i][0][j] = gradients[i][1 * GRANULARITY][j];
			}
		}
	}

	if (hasGeneratedGradients(x, y + 1, z) == 1) {
		glm::vec3 *** gradients = getChunk(x, y + 1, z);
		for (i = 0; i < NUM_GRADS; i++) {
			for (j = 0; j < NUM_GRADS; j++) {
				this->gradients3d[i][1 * GRANULARITY][j] = gradients[i][0][j];
			}
		}
	}

	// Positions relative to full world
	GLint rx = x + halfMaxChunks;
	GLint ry = y + halfMaxChunks;
	GLint rz = z + halfMaxChunks;
	this->chunks[rx][ry][rz].gradients= new glm::vec3**[NUM_GRADS];
	for (i = 0; i < NUM_GRADS; i++) {
		this->chunks[rx][ry][rz].gradients[i] = new glm::vec3*[NUM_GRADS];
		for (j = 0; j < NUM_GRADS; j++) {
			this->chunks[rx][ry][rz].gradients[i][j] = new glm::vec3[NUM_GRADS];
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

	this->gradients = new glm::vec2*[x];
	for (i = 0; i < x; i++) {
		this->gradients[i] = new glm::vec2[y];
		for (j = 0; j < y; j++) {
			this->gradients[i][j] = randomVector((GLfloat)1.0);
		}
	}

	this->gradients[0][0] = glm::vec2(1.0, 0.0);
	this->gradients[1][0] = glm::vec2(0.0, 1.0);
	this->gradients[0][1] = glm::vec2(-1.0, 0.0);
	this->gradients[1][1] = glm::vec2(0.0, -1.0);
}

void ChunkManager::genGradients3d(GLuint x, GLuint y, GLuint z) {
	this->gradients3d = new glm::vec3**[x];
	for (GLuint i = 0; i < x; i++) {
		this->gradients3d[i] = new glm::vec3*[y];
		for (GLuint j = 0; j < y; j++) {
			this->gradients3d[i][j] = new glm::vec3[z];
			for (GLuint k = 0; k < z; k++) {
				this->gradients3d[i][j][k] = random3DVector((GLfloat)1.0);
			}
		}
	}
}

glm::vec2 ChunkManager::randomVector(GLfloat length) {
	std::random_device rd;
	std::uniform_real_distribution<GLdouble> dist(0.0, 2.0 * PI);
	GLfloat angle = (GLfloat)dist(rd);

	// Turn angle and length into vector
	// The length of the vector is implicitly 1
	GLfloat x = length * std::cos(angle);
	GLfloat y = length * std::sin(angle);
	return glm::vec2(x, y);
}

glm::vec3 ChunkManager::random3DVector(GLfloat length) {
	std::random_device rd;
	std::uniform_real_distribution<GLdouble> dist(0.0, 2.0 * PI);
	std::uniform_real_distribution<GLfloat> randomCostheta(-1.0, 1.0);
	std::uniform_real_distribution<GLfloat> randomU(0.0, 1.0);
	GLfloat phi = (GLfloat)dist(rd);
	GLfloat costheta = randomCostheta(rd);

	GLfloat theta = std::acos(costheta);

	length = length * std::cbrt(randomU(rd));

	// Turn angle and length into vector
	// The length of the vector is implicitly 1
	GLfloat x = length * std::sin(theta) * std::cos(phi);
	GLfloat y = length * std::sin(theta) * std::sin(phi);
	GLfloat z = length * std::cos(theta);
	return glm::vec3(x, y, z);
}
