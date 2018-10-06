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
	InstancedArrayComplexShader(const Camera & camera, const LightsContainer & lightsContainer, glm::mat4 projection, const GLchar * vertexPath, const GLchar * fragmentPath);
	InstancedArrayComplexShader(const Camera & camera, const LightsContainer & lightsContainer, glm::mat4 projection, std::string materialPath, const GLchar * vertexPath, const GLchar * fragmentPath);
	~InstancedArrayComplexShader();

	void sendToShader(Material * material);

	void setSamplers();
	void sendLightsContainerToShader();
	void sendProjectionMatrixToShader();
	void sendCameraToShader();
	void sendMaterialToShader();

	const LightsContainer & getLightsContainer();

protected:
	GLuint shaderId;

	void use();

private:
	const LightsContainer & lightsContainer;
	const Camera & camera;
	glm::mat4 projection;
	Material * material;

	void buildShaders(const GLchar* vertexPath, const GLchar* fragmentPath);
	void readShaderFile(const GLchar * path, std::string * code);
	GLuint createShader(GLint type, const GLchar * code);
};

