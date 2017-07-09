#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// SOIL
#include <SOIL/SOIL.h>


class Texture {
public:
	GLuint id;
	GLchar* name;
	std::string type;

	Texture();

	Texture(GLchar * path, GLboolean alpha);

	/* MAKE THE MESH/MATERIALS USE THIS INSTEAD OF THE TEXTURE STRUCT IN MESH CLASS*/
	Texture(GLchar * path, GLboolean alpha, GLchar * name);

	GLuint getTextureID();


private:
	

};

#endif // !TEXTURE_H