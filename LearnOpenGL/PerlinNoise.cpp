#include "PerlinNoise.h"

glm::vec2 ** PerlinNoise::gradients;
GLfloat ** PerlinNoise::values;

GLint PerlinNoise::maxX;
GLint PerlinNoise::maxY;

const double PerlinNoise::PI = 3.141592653589793;

GLfloat **  PerlinNoise::generate(GLuint x, GLuint y)
{
	GLuint i, j;
	
	maxX = x;
	maxY = y;

	genGradients(64, 64);

	values = new GLfloat*[x];
	for (i = 0; i < x; i++) {
		values[i] = new GLfloat[y];
		for (j = 0; j < y; j++) {
			//values[i][j] = perlin(i, j, (GLfloat)i * 0.005, (GLfloat)j * 0.005);
			values[i][j] = perlin((GLfloat)i / (GLfloat)x * (GLfloat)8.0, (GLfloat)j / (GLfloat)y * (GLfloat)8.0);
			// std::cout << dist(rd) << " " << dist(rd) << std::endl;
		}
	}

	return values;
}

void PerlinNoise::genGradients(GLuint x, GLuint y)
{
	GLuint i, j;

	gradients = new glm::vec2*[x];
	for (i = 0; i < x; i++) {
		gradients[i] = new glm::vec2[y];
		for (j = 0; j < y; j++) {
			gradients[i][j] = randomVector((GLfloat)1.0);
			// TODO: Createa a function to generate vectors to avoid tall mountains
		}
	}

	gradients[0][0] = glm::vec2(1.0, 0.0);
	gradients[1][0] = glm::vec2(0.0, 1.0);
	gradients[0][1] = glm::vec2(-1.0, 0.0);
	gradients[1][1] = glm::vec2(0.0, -1.0);
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

	//sx = fade(sx);
	//sy = fade(sy);

	GLfloat bottomLeftDot = glm::dot(gradients[(GLint)bottomLeft.x][(GLint)bottomLeft.y], xy - bottomLeft);
	GLfloat bottomRightDot = glm::dot(gradients[(GLint)topRight.x][(GLint)bottomLeft.y], xy - glm::vec2(topRight.x, bottomLeft.y));

	GLfloat smooth1 = lerp(bottomLeftDot, bottomRightDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, sx));

	GLfloat topLeftDot = glm::dot(gradients[(GLint)bottomLeft.x][(GLint)topRight.y], xy - glm::vec2(bottomLeft.x, topRight.y));
	GLfloat topRightDot = glm::dot(gradients[(GLint)topRight.x][(GLint)topRight.y], xy - topRight);

	GLfloat smooth2 = lerp(topLeftDot, topRightDot, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, sx));

	GLfloat value = lerp(smooth1, smooth2, glm::smoothstep((GLfloat)0.0, (GLfloat)1.0, sy));
	return value;
}


// TODO: youtube video 13.30 says the distance coordinate should not exceed 1
GLfloat PerlinNoise::perlin(GLint x, GLint y, GLfloat xVal, GLfloat yVal)
{
	// Turn param into vector
	glm::vec2 pos = glm::vec2(x, y);
	glm::vec2 posVal = glm::vec2(xVal, yVal);

	GLfloat bottomLeftDot = glm::dot(gradients[0][0], posVal - glm::vec2(0, 0));
	GLfloat bottomRightDot = glm::dot(gradients[1][0], posVal - glm::vec2(maxX * 0.01 * 2.0, 0));

	GLfloat smooth1 = glm::smoothstep(bottomLeftDot, bottomRightDot, xVal);

	GLfloat topLeftDot = glm::dot(gradients[0][1], posVal - glm::vec2(0, maxY * 0.01 * 2.0));
	GLfloat topRightDot = glm::dot(gradients[1][1], posVal - glm::vec2(maxX * 0.01 * 2.0, maxY * 0.01 * 2.0));
	
	//topLeftDot = fade(topLeftDot);
	//topRightDot = fade(topRightDot);
	GLfloat smooth2 = glm::smoothstep(topLeftDot, topRightDot, xVal);

	//smooth1 = fade(smooth1);
	//smooth2 = fade(smooth2);
	GLfloat value = glm::smoothstep(smooth1, smooth2, yVal);

	return value;
}


/*
Generates random vector by generating a random angle
Having the length of the vector we can convert these polar coordinates into
Cartesian coordinates
*/
glm::vec2 PerlinNoise::randomVector(GLfloat length)
{
	std::random_device rd;
	std::uniform_real_distribution<GLfloat> dist(0.0, 2.0 * PI);
	GLfloat angle = dist(rd);

	// Turn angle and length into vector
	// The length of the vector is implicitly 1
	GLfloat x = length * std::cos(angle);
	GLfloat y = length * std::sin(angle);
	return glm::vec2(x, y);
}

glm::vec3 PerlinNoise::random3DVector(GLfloat length)
{
	std::random_device rd;
	std::uniform_real_distribution<GLfloat> dist(0.0, 2.0 * PI);
	std::uniform_real_distribution<GLfloat> randomCostheta(-1.0, 1.0);
	std::uniform_real_distribution<GLfloat> randomU(0.0, 1.0);
	GLfloat phi = dist(rd);
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
	return (1.0 - w)*x + w*y;
}
