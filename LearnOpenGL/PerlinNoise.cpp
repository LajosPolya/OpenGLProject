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

	values = new GLfloat*[this->x];
	for (i = 0; i < this->x; i++) {
		values[i] = new GLfloat[this->y];
		for (j = 0; j < this->y; j++) {
			values[i][j] = perlin((GLfloat)i / (GLfloat)this->x * (GLfloat)6.0, (GLfloat)j / (GLfloat)this->y * (GLfloat)6.0);
		}
	}

	return values;
}

GLfloat PerlinNoise::generate(GLint x, GLint y, GLint z) {
	if (this->gradients3d == nullptr) {
		std::cout << "ERROR::PerlinNoise:: CHUNK NOT SET BUT IT GENERATING" << std::endl;
	}
	return perlin((GLfloat)x / (GLfloat)this->x * (GLfloat)1.0, (GLfloat)y / (GLfloat)this->y * (GLfloat)1.0, (GLfloat)z / (GLfloat)this->z * (GLfloat)1.0);
}

void PerlinNoise::setChunk(glm::vec3 *** gradientVec) {
	this->gradients3d = gradientVec;
}

void PerlinNoise::setChunk(glm::vec2 ** gradientVec) {
	this->gradients = gradientVec;
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

GLfloat PerlinNoise::fade(GLfloat val) {
	return val * val * val * (val * (val * 6 - 15) + 10);
}

GLfloat PerlinNoise::lerp(GLfloat x, GLfloat y, GLfloat w) {
	return (1.0f - w)*x + w*y;
}
