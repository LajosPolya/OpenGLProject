#pragma once
#include "InstancedArrayComplexShader.h"


class InstancedComplexShader : public InstancedArrayComplexShader
{
public:
	InstancedComplexShader(const Camera & camera, const LightsContainer & lightsContainer, glm::mat4 projection, const GLchar * vertexPath, const GLchar * fragmentPath);
	InstancedComplexShader(const Camera & camera, const LightsContainer & lightsContainer, glm::mat4 projection, std::string materialPath, const GLchar * vertexPath, const GLchar * fragmentPath);
	~InstancedComplexShader();

	void sendInstancesToShader(std::vector<glm::mat4> intances);
};

