#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "LightsContainer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"

using namespace std;
using namespace glm;

class InstancedArrayComplexShader
{
public:
	InstancedArrayComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string & vertexPath, const string & fragmentPath);
	InstancedArrayComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string & materialPath, const string & vertexPath, const string & fragmentPath);
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
	mat4 projection;
	Material * material;

	void buildShaders(const string & vertexPath, const string & fragmentPath);
	string readShaderFile(const string & path);
	GLuint createShader(GLint type, const string & code);
};

