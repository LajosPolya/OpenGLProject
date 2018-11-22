#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(GLuint x, GLuint y) {
	this->x = x;
	this->y = y;
}

// The parameters are the size of the chunk it should produce on each axis
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
			// TODO: THIS GRANULARITY (2.0) SHOULD NOT BE HERE AND WILL BREAK CODE;
			values[i][j] = perlin((GLfloat)i / (GLfloat)this->x * (GLfloat)2.0, (GLfloat)j / (GLfloat)this->y * (GLfloat)2.0);
		}
	}

	return values;
}

GLfloat PerlinNoise::generate(GLfloat x, GLfloat y, GLfloat z) {
	if (this->gradients3d == nullptr) {
		cout << "ERROR::PerlinNoise:: CHUNK NOT SET BUT IT GENERATING" << endl;
	}
	return perlin(x / (GLfloat)this->x, y / (GLfloat)this->y, z / (GLfloat)this->z);
}

void PerlinNoise::setChunk(vec3 *** gradientVec) {
	this->gradients3d = gradientVec;
}

void PerlinNoise::setChunk(vec2 ** gradientVec) {
	this->gradients = gradientVec;
}

GLfloat PerlinNoise::perlin(GLfloat x, GLfloat y) {
	// Turn param into vector
	vec2 xy{ x, y };

	// Grid Cell Coordinates
	vec2 bottomLeft{ floor(x), floor(y) };
	vec2 topRight{ bottomLeft.x + 1, bottomLeft.y + 1 };

	// Determine Interpolation Weight
	// (or the decimal value of the point)
	GLfloat sx = x - bottomLeft.x;
	GLfloat sy = y - bottomLeft.y;

	GLfloat bottomLeftDot = dot(this->gradients[(GLint)bottomLeft.x][(GLint)bottomLeft.y], xy - bottomLeft);
	GLfloat bottomRightDot = dot(this->gradients[(GLint)topRight.x][(GLint)bottomLeft.y], xy - vec2(topRight.x, bottomLeft.y));

	GLfloat smooth1 = lerp(bottomLeftDot, bottomRightDot, smoothstep((GLfloat)0.0, (GLfloat)1.0, sx));

	GLfloat topLeftDot = dot(this->gradients[(GLint)bottomLeft.x][(GLint)topRight.y], xy - vec2(bottomLeft.x, topRight.y));
	GLfloat topRightDot = dot(this->gradients[(GLint)topRight.x][(GLint)topRight.y], xy - topRight);

	GLfloat smooth2 = lerp(topLeftDot, topRightDot, smoothstep((GLfloat)0.0, (GLfloat)1.0, sx));

	GLfloat value = lerp(smooth1, smooth2, smoothstep((GLfloat)0.0, (GLfloat)1.0, sy));
	return value;
}

GLfloat PerlinNoise::perlin(GLfloat x, GLfloat y, GLfloat z) {
	// Turn param into vector
	vec3 xyz{ x, y, z };

	// Grid Cell Coordinates
	vec3 lowerLattice{ floor(x), floor(y), floor(z) };
	vec3 upperLattice{ lowerLattice.x + 1.0, lowerLattice.y + 1.0, lowerLattice.z + 1.0 };

	vec3 diff = (xyz - lowerLattice) / (upperLattice - lowerLattice);

	// Bottom Left Front Dot Product and Bottom Right Front Dot
	GLfloat blfDot = dot(this->gradients3d[(GLint)lowerLattice.x][(GLint)lowerLattice.y][(GLint)lowerLattice.z], xyz - lowerLattice);
	GLfloat brfDot = dot(this->gradients3d[(GLint)upperLattice.x][(GLint)lowerLattice.y][(GLint)lowerLattice.z], xyz - vec3(upperLattice.x, lowerLattice.y, lowerLattice.z));
	GLfloat c00 = lerp(blfDot, brfDot, smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat blbDot = dot(this->gradients3d[(GLint)lowerLattice.x][(GLint)lowerLattice.y][(GLint)upperLattice.z], xyz - vec3(lowerLattice.x, lowerLattice.y, upperLattice.z));
	GLfloat brbDot = dot(this->gradients3d[(GLint)upperLattice.x][(GLint)lowerLattice.y][(GLint)upperLattice.z], xyz - vec3(upperLattice.x, lowerLattice.y, upperLattice.z));
	GLfloat c01 = lerp(blbDot, brbDot, smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat tlfDot = dot(this->gradients3d[(GLint)lowerLattice.x][(GLint)upperLattice.y][(GLint)lowerLattice.z], xyz - vec3(lowerLattice.x, upperLattice.y, lowerLattice.z));
	GLfloat trfDot = dot(this->gradients3d[(GLint)upperLattice.x][(GLint)upperLattice.y][(GLint)lowerLattice.z], xyz - vec3(upperLattice.x, upperLattice.y, lowerLattice.z));
	GLfloat c10 = lerp(tlfDot, trfDot, smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat tlbDot = dot(this->gradients3d[(GLint)lowerLattice.x][(GLint)upperLattice.y][(GLint)upperLattice.z], xyz - vec3(lowerLattice.x, upperLattice.y, upperLattice.z));
	GLfloat trbDot = dot(this->gradients3d[(GLint)upperLattice.x][(GLint)upperLattice.y][(GLint)upperLattice.z], xyz - vec3(upperLattice.x, upperLattice.y, upperLattice.z));
	GLfloat c11 = lerp(tlbDot, trbDot, smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat c0 = lerp(c00, c10, smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.y));
	GLfloat c1 = lerp(c01, c11, smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.y));

	GLfloat c = lerp(c0, c1, smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.z));

	return c;
}

GLfloat PerlinNoise::fade(GLfloat val) {
	return val * val * val * (val * (val * 6 - 15) + 10);
}

GLfloat PerlinNoise::lerp(GLfloat x, GLfloat y, GLfloat w) {
	return (1.0f - w)*x + w*y;
}
