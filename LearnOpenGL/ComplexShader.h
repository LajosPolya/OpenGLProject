#pragma once
#include "InstancedArrayComplexShader.h"


class ComplexShader : public InstancedArrayComplexShader
{
public:
	ComplexShader(Camera * camera, LightsContainer * lightsContainer, glm::mat4 projection, const GLchar * vertexPath, const GLchar * fragmentPath);
	~ComplexShader();

	void sendModelToShader(glm::mat4 model);
};

