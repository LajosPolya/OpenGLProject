#include "PerlinNoise.h"



PerlinNoise::PerlinNoise() {}
PerlinNoise::~PerlinNoise() {}

PerlinNoise::PerlinNoise(GLuint x, GLuint y)
{
	GLuint i, j;
	std::random_device rd;
	std::uniform_real_distribution<GLfloat> dist(0, 1);
	std::ofstream file;
	file.open("Perlin/perlin.txt");
	this->maxX = x;
	this->maxY = y;

	// Might only need to add 1
	this->genGradients(2, 2);

	this->values = new GLfloat*[x];
	for (i = 0; i < x; i++) {
		this->values[i] = new GLfloat[y];
		for (j = 0; j < y; j++) {
			this->values[i][j] = perlin(i, j, (GLfloat)(i + 1) * 0.04, (GLfloat)(j + 1) * 0.04);
			// std::cout << dist(rd) << " " << dist(rd) << std::endl;
			file << i << "," << this->values[i][j] * (GLfloat)4 + (GLfloat)10 << "," << j << "," << 0.0 << "," << 0.0 << "," << 0.0 << "," << 1.0 << "," << 1.0 << "," << 1.0 << std::endl;
		}
	}

	file.close();
}

void PerlinNoise::genGradients(GLuint x, GLuint y)
{
	std::random_device rd;
	std::uniform_real_distribution<GLfloat> dist(0, 1000);
	GLuint i, j;

	this->gradients = new glm::vec2*[x];
	for (i = 0; i < x; i++) {
		this->gradients[i] = new glm::vec2[y];
		for (j = 0; j < y; j++) {
			glm::vec2 nonUnitVec2 = glm::vec2(dist(rd), dist(rd));
			GLfloat len = glm::length(nonUnitVec2);
			this->gradients[i][j] = glm::vec2(nonUnitVec2.x / len, nonUnitVec2.y / len);
			// std::cout << gradients[i][j].x << " " << gradients[i][j].y << " " << glm::length(gradients[i][j]) << std::endl;
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

	GLfloat bottomLeftDot = glm::dot(gradients[(GLint)bottomLeft.x / (this->maxX / 2)][(GLint)bottomLeft.y / (this->maxY / 2)], bottomLeft - glm::vec2(0,0));
	GLfloat bottomRightDot = glm::dot(gradients[(GLint)bottomLeft.x / (this->maxX / 2)][(GLint)bottomLeft.y / (this->maxX / 2)], bottomLeft - glm::vec2(this->maxX, 0));
	//GLfloat bottomLeftDot = glm::dot(gradients[0][0], bottomLeft - xy);
	//GLfloat bottomRightDot = glm::dot(gradients[1][0], glm::vec2(topRight.x, bottomLeft.y) - xy);

	GLfloat smooth1 = glm::smoothstep(bottomLeftDot, bottomRightDot, sx);

	GLfloat topLeftDot = glm::dot(gradients[(GLint)bottomLeft.x / (this->maxX / 2)][(GLint)bottomLeft.y / (this->maxY / 2)], bottomLeft - glm::vec2(0, this->maxY));
	GLfloat topRightDot = glm::dot(gradients[(GLint)bottomLeft.x / (this->maxX / 2)][(GLint)bottomLeft.y / (this->maxY / 2)], bottomLeft - glm::vec2(this->maxX, this->maxY));
	//GLfloat topLeftDot = glm::dot(gradients[0][1], glm::vec2(bottomLeft.x, topRight.y) - xy);
	//GLfloat topRightDot = glm::dot(gradients[1][1], topRight - xy);

	GLfloat smooth2 = glm::smoothstep(topLeftDot, topRightDot, sx);

	GLfloat value = glm::smoothstep(smooth1, smooth2, sy);
	// std::cout << value << std::endl;
	return value;
}

GLfloat PerlinNoise::perlin(GLint x, GLint y, GLfloat xVal, GLfloat yVal)
{
	// Turn param into vector
	glm::vec2 pos = glm::vec2(x, y);

	GLfloat bottomLeftDot = glm::dot(gradients[0][0], pos - glm::vec2(0, 0));
	GLfloat bottomRightDot = glm::dot(gradients[1][0], pos - glm::vec2(this->maxX, 0));

	GLfloat smooth1 = glm::smoothstep(bottomLeftDot, bottomRightDot, xVal);

	GLfloat topLeftDot = glm::dot(gradients[0][1], pos - glm::vec2(0, this->maxY));
	GLfloat topRightDot = glm::dot(gradients[1][1], pos - glm::vec2(this->maxX, this->maxY));

	GLfloat smooth2 = glm::smoothstep(topLeftDot, topRightDot, xVal);

	GLfloat value = glm::smoothstep(smooth1, smooth2, yVal);
	// std::cout << value << std::endl;
	return value;
}
