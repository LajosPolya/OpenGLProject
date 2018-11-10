#pragma once
#include "InstancedArrayComplexShader.h"

using namespace std;

class ComplexShader : public InstancedArrayComplexShader
{
public:
	ComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string & vertexPath, const string & fragmentPath);
	ComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string materialPath, const string & vertexPath, const string & fragmentPath);
	~ComplexShader();

	// TODO: Should ComplexShader have a list of all GameObjects | Transform it draws so model isn't sent every frame via method below
	/*
		If the Transform was stored then the GameObject won't need to store the Material and the ComplexShader can draw any Transform's
		Model with that Material
	*/
	void sendModelToShader(mat4 model);
};

