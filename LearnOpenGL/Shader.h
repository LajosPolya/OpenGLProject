#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>


class Shader
{
public:
	// Program ID
	GLuint Program;

	Shader();

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void Use();
};

#endif // !SHADER_H
