#include "PerlinNoise.h"

glm::vec2 ** PerlinNoise::gradients;
glm::vec3 *** PerlinNoise::gradients3d;

const double PerlinNoise::PI = 3.141592653589793;

GLfloat **  PerlinNoise::generate(GLuint x, GLuint y)
{
	GLuint i, j;
	GLfloat ** values;

	genGradients(64, 64);

	values = new GLfloat*[x];
	for (i = 0; i < x; i++) {
		values[i] = new GLfloat[y];
		for (j = 0; j < y; j++) {
			values[i][j] = perlin((GLfloat)i / (GLfloat)x * (GLfloat)6.0, (GLfloat)j / (GLfloat)y * (GLfloat)6.0);
		}
	}

	return values;
}

GLfloat *** PerlinNoise::generate(GLuint x, GLuint y, GLuint z)
{
	GLuint i, j, k;
	GLfloat *** values3d;

	genGradients3d(64, 64, 64);

	values3d = new GLfloat**[x];
	for (i = 0; i < x; i++) {
		values3d[i] = new GLfloat*[y];
		for (j = 0; j < y; j++) {
			values3d[i][j] = new GLfloat[z];
			for (k = 0; k < z; k++) {
				values3d[i][j][k] = perlin((GLfloat)i / (GLfloat)x * (GLfloat)4.0, (GLfloat)j / (GLfloat)y * (GLfloat)4.0, (GLfloat)k / (GLfloat)z * (GLfloat)4.0);
			}
		}
	}

	return values3d;
}

void PerlinNoise::genGradients(GLuint x, GLuint y)
{
	GLuint i, j;

	gradients = new glm::vec2*[x];
	for (i = 0; i < x; i++) {
		gradients[i] = new glm::vec2[y];
		for (j = 0; j < y; j++) {
			gradients[i][j] = randomVector((GLfloat)1.0);
		}
	}

	gradients[0][0] = glm::vec2(1.0, 0.0);
	gradients[1][0] = glm::vec2(0.0, 1.0);
	gradients[0][1] = glm::vec2(-1.0, 0.0);
	gradients[1][1] = glm::vec2(0.0, -1.0);
}

void PerlinNoise::genGradients3d(GLuint x, GLuint y, GLuint z)
{
	gradients3d = new glm::vec3**[x];
	for (GLuint i = 0; i < x; i++) {
		gradients3d[i] = new glm::vec3*[y];
		for (GLuint j = 0; j < y; j++) {
			gradients3d[i][j] = new glm::vec3[z];
			for (GLuint k = 0; k < z; k++) {
				gradients3d[i][j][k] = random3DVector((GLfloat)1.0);
			}
		}
	}
}

GLfloat PerlinNoise::perlin(GLfloat x, GLfloat y)
{
	// Turn param into vector
	glm::vec2 xy = glm::vec2(x, y);

	// Grid Cell Coordinates
	glm::vec2 bottomLeft = glm::vec2(std::floor(x), std::floor(y));
	glm::vec2 topRight = glm::vec2(bottomLeft.x + 1, bottomLeft.y + 1);

	// Determine Interpolation Weight
	// (or the decimal value of the point)
	GLfloat sx = x - bottomLeft.x;
	GLfloat sy = y - bottomLeft.y;

	GLfloat bottomLeftDot = glm::dot(gradients[(GLint)bottomLeft.x][(GLint)bottomLeft.y], xy - bottomLeft);
	GLfloat bottomRightDot = glm::dot(gradients[(GLint)topRight.x][(GLint)bottomLeft.y], xy - glm::vec2(topRight.x, bottomLeft.y));

	GLfloat smooth1 = lerp(bottomLeftDot, bottomRightDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, sx));

	GLfloat topLeftDot = glm::dot(gradients[(GLint)bottomLeft.x][(GLint)topRight.y], xy - glm::vec2(bottomLeft.x, topRight.y));
	GLfloat topRightDot = glm::dot(gradients[(GLint)topRight.x][(GLint)topRight.y], xy - topRight);

	GLfloat smooth2 = lerp(topLeftDot, topRightDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, sx));

	GLfloat value = lerp(smooth1, smooth2, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, sy));
	return value;
}

GLfloat PerlinNoise::perlin(GLfloat x, GLfloat y, GLfloat z)
{
	// Turn param into vector
	glm::vec3 xyz = glm::vec3(x, y, z);

	// Grid Cell Coordinates
	glm::vec3 lowerLattice = glm::vec3(std::floor(x), std::floor(y), std::floor(z));
	glm::vec3 upperLattice = glm::vec3(lowerLattice.x + 1.0, lowerLattice.y + 1.0, lowerLattice.z + 1);

	glm::vec3 diff = (xyz - lowerLattice) / (upperLattice - lowerLattice);

	// Bottom Left Front Dot Product and Bottom Right Front Dot
	GLfloat blfDot = glm::dot(gradients3d[(GLint)lowerLattice.x][(GLint)lowerLattice.y][(GLint)lowerLattice.z], xyz - lowerLattice);
	GLfloat brfDot = glm::dot(gradients3d[(GLint)upperLattice.x][(GLint)lowerLattice.y][(GLint)lowerLattice.z], xyz - glm::vec3(upperLattice.x, lowerLattice.y, lowerLattice.z));
	GLfloat c00 = lerp(blfDot, brfDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat blbDot = glm::dot(gradients3d[(GLint)lowerLattice.x][(GLint)lowerLattice.y][(GLint)upperLattice.z], xyz - glm::vec3(lowerLattice.x, lowerLattice.y, upperLattice.z));
	GLfloat brbDot = glm::dot(gradients3d[(GLint)upperLattice.x][(GLint)lowerLattice.y][(GLint)upperLattice.z], xyz - glm::vec3(upperLattice.x, lowerLattice.y, upperLattice.z));
	GLfloat c01 = lerp(blbDot, brbDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat tlfDot = glm::dot(gradients3d[(GLint)lowerLattice.x][(GLint)upperLattice.y][(GLint)lowerLattice.z], xyz - glm::vec3(lowerLattice.x, upperLattice.y, lowerLattice.z));
	GLfloat trfDot = glm::dot(gradients3d[(GLint)upperLattice.x][(GLint)upperLattice.y][(GLint)lowerLattice.z], xyz - glm::vec3(upperLattice.x, upperLattice.y, lowerLattice.z));
	GLfloat c10 = lerp(tlfDot, trfDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, diff.x));

	GLfloat tlbDot = glm::dot(gradients3d[(GLint)lowerLattice.x][(GLint)upperLattice.y][(GLint)upperLattice.z], xyz - glm::vec3(lowerLattice.x, upperLattice.y, upperLattice.z));
	GLfloat trbDot = glm::dot(gradients3d[(GLint)upperLattice.x][(GLint)upperLattice.y][(GLint)upperLattice.z], xyz - glm::vec3(upperLattice.x, upperLattice.y, upperLattice.z));
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
glm::vec2 PerlinNoise::randomVector(GLfloat length)
{
	std::random_device rd;
	std::uniform_real_distribution<GLdouble> dist(0.0, 2.0 * PI);
	GLfloat angle = (GLfloat)dist(rd);

	// Turn angle and length into vector
	// The length of the vector is implicitly 1
	GLfloat x = length * std::cos(angle);
	GLfloat y = length * std::sin(angle);
	return glm::vec2(x, y);
}

glm::vec3 PerlinNoise::random3DVector(GLfloat length)
{
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

GLfloat PerlinNoise::fade(GLfloat val)
{
	return val * val * val * (val * (val * 6 - 15) + 10);
}

GLfloat PerlinNoise::lerp(GLfloat x, GLfloat y, GLfloat w)
{
	return (1.0f - w)*x + w*y;
}
