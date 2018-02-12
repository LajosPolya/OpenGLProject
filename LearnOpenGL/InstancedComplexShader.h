#pragma once
#include "InstancedArrayComplexShader.h"


class InstancedComplexShader : public InstancedArrayComplexShader
{
public:
	InstancedComplexShader(Camera * camera, LightsContainer * lightsContainer, glm::mat4 projection, const GLchar * vertexPath, const GLchar * fragmentPath);
	~InstancedComplexShader();

	void sendInstancesToShader(std::vector<glm::mat4> intances);
};

