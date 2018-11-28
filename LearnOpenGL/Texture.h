#pragma once

#include <string>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// SOIL
#include <SOIL/SOIL.h>

using namespace std;

class Texture {
public:
	Texture();
	Texture(const Texture & toCopy);

	Texture(GLchar * path, GLboolean alpha);

	/* MAKE THE MESH/MATERIALS USE THIS INSTEAD OF THE TEXTURE STRUCT IN MESH CLASS*/
	Texture(GLchar * path, GLboolean alpha, string name);

	GLuint getTextureID();
	string getName();
	string getType();

	void setName(string name);

private:
	GLuint id;
	string name;
	string type;
};
