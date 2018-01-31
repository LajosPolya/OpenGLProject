#pragma once

#include "LightsContainer.h"
#include "Camera.h"

class ComplexShader
{
public:
	ComplexShader(Camera * camera);
	~ComplexShader();

private:
	LightsContainer * lightContainer;
	Camera * camera;
};

