#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "LightsContainer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"

class InstancedArrayComplexShader
{
public:
	InstancedArrayComplexShader(Camera * camera, LightsContainer * lightsContainer, glm::mat4 projection, const GLchar * vertexPath, const GLchar * fragmentPath);
	~InstancedArrayComplexShader();

	void sendToShader(Material * material);

	void setSamplers();
	void sendLightsContainerToShader();
	void sendProjectionMatrixToShader();
	void sendCameraToShader();

protected:
	GLuint shaderId;

private:
	LightsContainer * lightsContainer;
	Camera * camera;
	glm::mat4 projection;

	void buildShaders(const GLchar* vertexPath, const GLchar* fragmentPath);
	void readShaderFile(const GLchar * path, std::string * code);
	GLuint createShader(GLint type, const GLchar * code);
	
	void use();
};

