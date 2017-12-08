#pragma once

#include <string>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// SOIL
#include <SOIL/SOIL.h>

class Texture {
public:
	Texture();
	Texture(const Texture & toCopy);

	Texture(GLchar * path, GLboolean alpha);
	Texture(GLuint id, std::string name, std::string type);

	/* MAKE THE MESH/MATERIALS USE THIS INSTEAD OF THE TEXTURE STRUCT IN MESH CLASS*/
	Texture(GLchar * path, GLboolean alpha, std::string name);

	GLuint getTextureID();
	std::string getName();
	std::string getType();

	void setName(std::string name);

private:
	GLuint id;
	std::string name;
	std::string type;
};
