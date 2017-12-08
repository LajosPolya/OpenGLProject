#include "Texture.h"

GLuint id;
GLchar * name;
std::string type;

Texture::Texture() {
	this->name = "";
	this->id = -1;
	this->type = std::string();
}

Texture::Texture(const Texture & toCopy) {
	this->id = toCopy.id;
	this->name = toCopy.name;
	this->type = toCopy.type;
}

Texture::Texture(GLchar * path, GLboolean alpha) {
	if (path != nullptr) {
		// Gen texture ID and load texture data
		glGenTextures(1, &this->id);
		int width, height;
		GLubyte * image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, this->id);
		glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		// Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
	}
}

Texture::Texture(GLuint id, std::string name, std::string type)
{
	this->id = id;
	this->name = name;
	this->type = type;
}

/* MAKE THE MESH/MATERIALS USE THIS INSTEAD OF THE TEXTURE STRUCT IN MESH CLASS*/
Texture::Texture(GLchar * path, GLboolean alpha, std::string name) {
	this->name = name;
	// Gen texture ID and load texture data
	glGenTextures(1, &this->id);
	int width, height;
	/* This was an unsigned cahr*/
	GLubyte * image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	// Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
}

GLuint Texture::getTextureID() {
	return this->id;
}

std::string Texture::getName()
{
	return this->name;
}

std::string Texture::getType()
{
	return this->type;
}

void Texture::setName(std::string name)
{
	this->name = name;
}
