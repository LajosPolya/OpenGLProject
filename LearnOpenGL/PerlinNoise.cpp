#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(GLuint x, GLuint y) {
	this->x = x;
	this->y = y;
}

// The parameters are the size of the chunck it should produce on each axis
PerlinNoise::PerlinNoise(GLuint x, GLuint y, GLuint z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

GLfloat **  PerlinNoise::generate(GLint x, GLint y) {
	GLuint i, j;
	GLfloat ** values;

	genGradients(64, 64);

	values = new GLfloat*[this->x];
	for (i = 0; i < this->x; i++) {
		values[i] = new GLfloat[this->y];
		for (j = 0; j < this->y; j++) {
			values[i][j] = perlin((GLfloat)i / (GLfloat)this->x * (GLfloat)6.0, (GLfloat)j / (GLfloat)this->y * (GLfloat)6.0);
		}
	}

	return values;
}

GLfloat *** PerlinNoise::generate(GLint x, GLint y, GLint z) {
	GLuint i, j, k;
	GLfloat *** values3d;

	if (this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] != nullptr) {
		return this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks];
	}

	if (this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] == nullptr) {
		genGradients3d(64, 64, 64);
	}

	if (this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks - 1] != nullptr) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				this->gradients3d[i][j][0] = this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks - 1][i][j][1 * 1];
			}
		}
	}

	if (this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks + 1] != nullptr) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				this->gradients3d[i][j][1 * 1] = this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks + 1][i][j][0];
			}
		}
	}

	if (this->chunkGrads[x + halfMaxChunks - 1][y + halfMaxChunks][z + halfMaxChunks] != nullptr) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				this->gradients3d[0][i][j] = this->chunkGrads[x + halfMaxChunks - 1][y + halfMaxChunks][z + halfMaxChunks][1 * 1][i][j];
			}
		}
	}

	if (this->chunkGrads[x + halfMaxChunks + 1][y + halfMaxChunks][z + halfMaxChunks] != nullptr) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				this->gradients3d[1 * 1][i][j] = this->chunkGrads[x + halfMaxChunks + 1][y + halfMaxChunks][z + halfMaxChunks][0][i][j];
			}
		}
	}

	values3d = new GLfloat**[this->x];
	this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] = new GLfloat**[this->x];
	for (i = 0; i < this->x; i++) {
		values3d[i] = new GLfloat*[this->y];
		this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks][i] = new GLfloat*[this->y];
		for (j = 0; j < this->y; j++) {
			values3d[i][j] = new GLfloat[this->z];
			this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks][i][j] = new GLfloat[this->z];
			for (k = 0; k < this->z; k++) {
				values3d[i][j][k] = perlin((GLfloat)i / (GLfloat)this->x * (GLfloat)1.0, (GLfloat)j / (GLfloat)this->y * (GLfloat)1.0, (GLfloat)k / (GLfloat)this->z * (GLfloat)1.0);
				this->chunkPositions[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks][i][j][k] = values3d[i][j][k];
			}
		}
	}

	//this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] = this->gradients3d;

	this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks] = new glm::vec3**[64];
	for (GLuint i = 0; i < 64; i++) {
		this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks][i] = new glm::vec3*[64];
		for (GLuint j = 0; j < 64; j++) {
			this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks][i][j] = new glm::vec3[64];
			for (GLuint k = 0; k < 64; k++) {
				this->chunkGrads[x + halfMaxChunks][y + halfMaxChunks][z + halfMaxChunks][i][j][k] = this->gradients3d[i][j][k];
			}
		}
	}

	return values3d;
}

void PerlinNoise::genGradients(GLuint x, GLuint y) {
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

void PerlinNoise::genGradients3d(GLuint x, GLuint y, GLuint z) {
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

GLfloat PerlinNoise::perlin(GLfloat x, GLfloat y) {
	// Turn param into vector
	glm::vec2 xy = glm::vec2(x, y);

	// Grid Cell Coordinates
	glm::vec2 bottomLeft = glm::vec2(std::floor(x), std::floor(y));
	glm::vec2 topRight = glm::vec2(bottomLeft.x + 1, bottomLeft.y + 1);

	// Determine Interpolation Weight
	// (or the decimal value of the point)
	GLfloat sx = x - bottomLeft.x;
	GLfloat sy = y - bottomLeft.y;

	GLfloat bottomLeftDot = glm::dot(this->gradients[(GLint)bottomLeft.x][(GLint)bottomLeft.y], xy - bottomLeft);
	GLfloat bottomRightDot = glm::dot(this->gradients[(GLint)topRight.x][(GLint)bottomLeft.y], xy - glm::vec2(topRight.x, bottomLeft.y));

	GLfloat smooth1 = lerp(bottomLeftDot, bottomRightDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, sx));

	GLfloat topLeftDot = glm::dot(this->gradients[(GLint)bottomLeft.x][(GLint)topRight.y], xy - glm::vec2(bottomLeft.x, topRight.y));
	GLfloat topRightDot = glm::dot(this->gradients[(GLint)topRight.x][(GLint)topRight.y], xy - topRight);

	GLfloat smooth2 = lerp(topLeftDot, topRightDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, sx));

	GLfloat value = lerp(smooth1, smooth2, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, sy));
	return value;
}

GLfloat PerlinNoise::perlin(GLfloat x, GLfloat y, GLfloat z) {
	// Turn param into vector
	glm::vec3 xyz = glm::vec3(x, y, z);

	// Grid Cell Coordinates
	glm::vec3 lowerLattice = glm::vec3(std::floor(x), std::floor(y), std::floor(z));
	glm::vec3 upperLattice = glm::vec3(lowerLattice.x + 1.0, lowerLattice.y + 1.0, lowerLattice.z + 1);

	glm::vec3 diff = (xyz - lowerLattice) / (upperLattice - lowerLattice);

	// Bottom Left Front Dot Product and Bottom Right Front Dot
	GLfloat blfDot = glm::dot(this->gradients3d[(GLint)lowerLattice.x][(GLint)lowerLattice.y][(GLint)lowerLattice.z], xyz - lowerLattice);
	GLfloat brfDot = glm::dot(this->gradients3d[(GLint)upperLattice.x][(GLint)lowerLattice.y][(GLint)lowerLattice.z], xyz - glm::vec3(upperLattice.x, lowerLattice.y, lowerLattice.z));
	GLfloat c00 = lerp(blfDot, brfDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat blbDot = glm::dot(this->gradients3d[(GLint)lowerLattice.x][(GLint)lowerLattice.y][(GLint)upperLattice.z], xyz - glm::vec3(lowerLattice.x, lowerLattice.y, upperLattice.z));
	GLfloat brbDot = glm::dot(this->gradients3d[(GLint)upperLattice.x][(GLint)lowerLattice.y][(GLint)upperLattice.z], xyz - glm::vec3(upperLattice.x, lowerLattice.y, upperLattice.z));
	GLfloat c01 = lerp(blbDot, brbDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat tlfDot = glm::dot(this->gradients3d[(GLint)lowerLattice.x][(GLint)upperLattice.y][(GLint)lowerLattice.z], xyz - glm::vec3(lowerLattice.x, upperLattice.y, lowerLattice.z));
	GLfloat trfDot = glm::dot(this->gradients3d[(GLint)upperLattice.x][(GLint)upperLattice.y][(GLint)lowerLattice.z], xyz - glm::vec3(upperLattice.x, upperLattice.y, lowerLattice.z));
	GLfloat c10 = lerp(tlfDot, trfDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat tlbDot = glm::dot(this->gradients3d[(GLint)lowerLattice.x][(GLint)upperLattice.y][(GLint)upperLattice.z], xyz - glm::vec3(lowerLattice.x, upperLattice.y, upperLattice.z));
	GLfloat trbDot = glm::dot(this->gradients3d[(GLint)upperLattice.x][(GLint)upperLattice.y][(GLint)upperLattice.z], xyz - glm::vec3(upperLattice.x, upperLattice.y, upperLattice.z));
	GLfloat c11 = lerp(tlbDot, trbDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat c0 = lerp(c00, c10, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.y));
	GLfloat c1 = lerp(c01, c11, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.y));

	GLfloat c = lerp(c0, c1, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.z));

	return c;
}


/*
Generates random vector by generating a random angle
Having the length of the vector we can convert these polar coordinates into
Cartesian coordinates
*/
glm::vec2 PerlinNoise::randomVector(GLfloat length) {
	std::random_device rd;
	std::uniform_real_distribution<GLdouble> dist(0.0, 2.0 * PI);
	GLfloat angle = (GLfloat)dist(rd);

	// Turn angle and length into vector
	// The length of the vector is implicitly 1
	GLfloat x = length * std::cos(angle);
	GLfloat y = length * std::sin(angle);
	return glm::vec2(x, y);
}

glm::vec3 PerlinNoise::random3DVector(GLfloat length) {
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

GLfloat PerlinNoise::fade(GLfloat val) {
	return val * val * val * (val * (val * 6 - 15) + 10);
}

GLfloat PerlinNoise::lerp(GLfloat x, GLfloat y, GLfloat w) {
	return (1.0f - w)*x + w*y;
}
