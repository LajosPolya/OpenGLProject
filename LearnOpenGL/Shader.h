#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Material.h"
#include "Mesh.h"
#include "LightsContainer.h"
#include "Camera.h"

using namespace std;
using namespace glm;

class TransparentGameObjectImpl;

class Shader
{
public:
	// Program ID
	GLuint Program;

	Shader();
	Shader(const Shader & toCopy);

	Shader(const string vertexPath, const string fragmentPath);
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath, GLchar * geometryPath);
	void readShaderFile(const string path, string * code);
	GLuint createShader(GLint type, const GLchar * code);

	void Use();

	void sendToShader(LightsContainer * lightsContainer);
	void sendToShader(TransparentGameObjectImpl * gameObject);
	void sendToShader(Material * material);

	void setProjectionMatrix(mat4 projection);

private:
	void sendCommonToShader(TransparentGameObjectImpl * gameObject);

};
