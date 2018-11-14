#pragma once
#include "InstancedArrayComplexShader.h"

using namespace std;
using namespace glm;


class InstancedComplexShader : public InstancedArrayComplexShader
{
public:
	InstancedComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string & vertexPath, const string & fragmentPath);
	InstancedComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string & materialPath, const string & vertexPath, const string & fragmentPath);
	~InstancedComplexShader();

	void sendInstancesToShader(vector<mat4> intances);
};

