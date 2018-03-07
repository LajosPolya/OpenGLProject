#include "ChunkManager.h"



ChunkManager::ChunkManager() {
	this->chunkPositions = new ComplexPosition***[maxChunks];
	this->chunkGrads = new glm::vec3*****[maxChunks];
	for (GLuint i = 0; i < maxChunks; i++) {
		this->chunkPositions[i] = new ComplexPosition**[maxChunks];
		this->chunkGrads[i] = new glm::vec3****[maxChunks];
		for (GLuint j = 0; j < maxChunks; j++) {
			this->chunkPositions[i][j] = new ComplexPosition*[maxChunks];
			this->chunkGrads[i][j] = new glm::vec3***[maxChunks];
			for (GLuint k = 0; k < maxChunks; k++) {
				this->chunkPositions[i][j][k] = nullptr;
				this->chunkGrads[i][j][k] = nullptr;
			}
		}
	}
}
ChunkManager::~ChunkManager() {}

GLboolean ChunkManager::hasGenerated(GLint x, GLint y, GLint z) {
	if (this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] != nullptr && this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] != nullptr) {
		return 1;
	}
	else if (this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] == nullptr && this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] == nullptr) {
		return 0;
	}
	else {
		std::cout << "ERROR::ChunkManager:: INCONSISTENCY BETWEEN chunkGrads && chunkPositions" << std::endl;
	}

	return 0;
}

GLboolean ChunkManager::hasGeneratedGradients(GLint x, GLint y, GLint z) {
	if (this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] != nullptr) {
		return 1;
	}
	return 0;
}

glm::vec2 ** ChunkManager::getGradients() {
	return this->gradients;
}

glm::vec3 *** ChunkManager::getChunk(GLint x, GLint y, GLint z) {
	return this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks];
}

void ChunkManager::setChunk(GLint x, GLint y, GLint z) {
	GLuint i, j, k;

	// Positions relative to full world
	GLint rx = x + halfMaxChunks;
	GLint ry = y + halfMaxChunks;
	GLint rz = z + halfMaxChunks;
	if (hasGeneratedGradients(x, y, z) == 0) {
		genGradients3d(64, 64, 64);
	}

	if (hasGeneratedGradients(x, y, z - 1) == 1) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				this->gradients3d[i][j][0] = this->chunkGrads[rx][ry][rz - 1][i][j][1 * 1];
			}
		}
	}

	if (hasGeneratedGradients(x, y, z + 1) == 1) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				this->gradients3d[i][j][1 * 1] = this->chunkGrads[rx][ry][rz + 1][i][j][0];
			}
		}
	}

	if (hasGeneratedGradients(x - 1, y, z) == 1) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				this->gradients3d[0][i][j] = this->chunkGrads[rx - 1][ry][rz][1 * 1][i][j];
			}
		}
	}

	if (hasGeneratedGradients(x + 1, y, z) == 1) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				this->gradients3d[1 * 1][i][j] = this->chunkGrads[rx + 1][ry][rz][0][i][j];
			}
		}
	}

	//this->chunkGrads[rx][ry][rz] = this->gradients3d;
	this->chunkGrads[rx][ry][rz] = new glm::vec3**[64];
	for (i = 0; i < 64; i++) {
		this->chunkGrads[rx][ry][rz][i] = new glm::vec3*[64];
		for (j = 0; j < 64; j++) {
			this->chunkGrads[rx][ry][rz][i][j] = new glm::vec3[64];
			for (k = 0; k < 64; k++) {
				this->chunkGrads[rx][ry][rz][i][j][k] = this->gradients3d[i][j][k];
			}
		}
	}
}

ComplexPosition * ChunkManager::getChunkPosition(GLint x, GLint y, GLint z) {
	return this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks];
}

void ChunkManager::setChunkPosition(ComplexPosition * CoPo, GLint x, GLint y, GLint z) {
	this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] = CoPo;
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
