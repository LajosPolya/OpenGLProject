#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "LightsContainer.h"
#include "Camera.h"

class ComplexShader
{
public:
	ComplexShader(Camera * camera, LightsContainer * lightsContainer, glm::mat4 projection, const GLchar* vertexPath, const GLchar* fragmentPath);
	~ComplexShader();

private:
	GLuint shaderId;
	LightsContainer * lighstContainer;
	Camera * camera;
	glm::mat4 projection;

	void buildShaders(const GLchar* vertexPath, const GLchar* fragmentPath);
	void readShaderFile(const GLchar * path, std::string * code);
	GLuint createShader(GLint type, const GLchar * code);
};

