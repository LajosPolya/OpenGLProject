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
	InstancedArrayComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const glm::mat4 & projection, const std::string & vertexPath, const std::string & fragmentPath);
	InstancedArrayComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const glm::mat4 & projection, const std::string & materialPath, const std::string & vertexPath, const std::string & fragmentPath);
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

	void buildShaders(const std::string & vertexPath, const std::string & fragmentPath);
	std::string readShaderFile(const std::string & path);
	GLuint createShader(GLint type, const std::string & code);
};

