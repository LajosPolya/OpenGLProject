#include "PerlinNoise.h"

glm::vec2 ** PerlinNoise::gradients;
GLfloat ** PerlinNoise::values;

GLint PerlinNoise::maxX;
GLint PerlinNoise::maxY;

const double PerlinNoise::PI = 3.141592653589793;

GLfloat **  PerlinNoise::generate(GLuint x, GLuint y)
{
	GLuint i, j;
	std::random_device rd;
	std::uniform_real_distribution<GLfloat> dist(0, 1);
	
	maxX = x;
	maxY = y;

	genGradients(32, 32);

	values = new GLfloat*[x];
	for (i = 0; i < x; i++) {
		values[i] = new GLfloat[y];
		for (j = 0; j < y; j++) {
			//values[i][j] = perlin(i, j, (GLfloat)i * 0.005, (GLfloat)j * 0.005);
			values[i][j] = perlin(0.01 * (GLfloat)i * (GLfloat)8.0, 0.01 * (GLfloat)j * (GLfloat)8.0);
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
			// TODO: Createa a function to rotate the vectors (maybe glm has one)
			if (j != 0) {
				// x * Math.Cos(degrees) - y * Math.Sin(degrees);
				gradients[i][j].x = gradients[i][j-1].x * std::cos(-PI / (GLfloat)32.0) - gradients[i][j-1].y * sin(-PI / (GLfloat)32.0);
				// result[1] = x * Math.Sin(degrees) + y * Math.Cos(degrees);
				gradients[i][j].y = gradients[i][j-1].x * std::sin(-PI / (GLfloat)32.0) + gradients[i][j-1].y * cos(-PI / (GLfloat)32.0);
			}
			else if (j == 0 && i != 0) {
				gradients[i][j].x = gradients[i-1][j].x * std::cos(-PI / (GLfloat)32.0) - gradients[i-1][j].y * sin(-PI / (GLfloat)32.0);
				// result[1] = x * Math.Sin(degrees) + y * Math.Cos(degrees);
				gradients[i][j].y = gradients[i-1][j - 1].x * std::sin(-PI / (GLfloat)32.0) + gradients[i-1][j].y * cos(-PI / (GLfloat)32.0);
			}
		}
	}

	/*gradients[0][0] = glm::vec2(1.0, 0.0);
	gradients[1][0] = glm::vec2(0.0, 1.0);
	gradients[0][1] = glm::vec2(-1.0, 0.0);
	gradients[1][1] = glm::vec2(0.0, -1.0);*/
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
	//GLfloat bottomLeftDot = glm::dot(gradients[0][0], bottomLeft - xy);
	//GLfloat bottomRightDot = glm::dot(gradients[1][0], glm::vec2(topRight.x, bottomLeft.y) - xy);

	GLfloat smooth1 = glm::smoothstep(bottomLeftDot, bottomRightDot, sx);

	GLfloat topLeftDot = glm::dot(gradients[(GLint)bottomLeft.x][(GLint)topRight.y], xy - glm::vec2(bottomLeft.x, topRight.y));
	GLfloat topRightDot = glm::dot(gradients[(GLint)topRight.x][(GLint)topRight.y], xy - topRight);
	//GLfloat topLeftDot = glm::dot(gradients[0][1], glm::vec2(bottomLeft.x, topRight.y) - xy);
	//GLfloat topRightDot = glm::dot(gradients[1][1], topRight - xy);

	GLfloat smooth2 = glm::smoothstep(topLeftDot, topRightDot, sx);

	GLfloat value = glm::smoothstep(smooth1, smooth2, sy);
	// std::cout << value << std::endl;
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

	GLfloat smooth2 = glm::smoothstep(topLeftDot, topRightDot, xVal);

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
	std::uniform_real_distribution<GLfloat> dist(0, 2 * PI);
	GLfloat angle = dist(rd);

	// Turn angle and length into vector
	// The length of the vector is implicitly 1
	GLfloat x = std::cos(angle);
	GLfloat y = std::sin(angle);
	return glm::vec2(x, y);
}
