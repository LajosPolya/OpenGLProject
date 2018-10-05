#pragma once
#include "InstancedArrayComplexShader.h"


class ComplexShader : public InstancedArrayComplexShader
{
public:
	ComplexShader(const Camera & camera, LightsContainer * lightsContainer, glm::mat4 projection, const GLchar * vertexPath, const GLchar * fragmentPath);
	ComplexShader(const Camera & camera, LightsContainer * lightsContainer, glm::mat4 projection, std::string materialPath, GLchar * vertexPath, const GLchar * fragmentPath);
	~ComplexShader();

	// TODO: Should ComplexShader have a list of all GameObjects | Transform it draws so model isn't sent every frame via method below
	/*
		If the Transform was stored then the GameObject won't need to store the Material and the ComplexShader can draw any Transform's
		Model with that Material
	*/
	void sendModelToShader(glm::mat4 model);
};

