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

// TODO: Why are these Forward Declared?
class GameObjectImpl;
class InstancedArrayGameObjectImpl;
class InstancedGameObjectImpl;
class TransparentGameObjectImpl;
class InstancedGameObject;

class Shader
{
public:
	// Program ID
	GLuint Program;

	Shader();
	Shader(const Shader & toCopy);

	Shader(const GLchar * vertexPath, const GLchar * fragmentPath);
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath, GLchar * geometryPath);
	void readShaderFile(const GLchar * path, std::string * code);
	GLuint createShader(GLint type, const GLchar * code);

	void Use();

	void sendToShader(LightsContainer * lightsContainer);
	void sendToShader(GameObjectImpl * gameObject);
	void sendToShader(InstancedArrayGameObjectImpl * gameObject);
	void sendToShader(InstancedGameObjectImpl * gameObject);
	void sendToShader(TransparentGameObjectImpl * gameObject);
	void sendToShader(Material * material);

	void setProjectionMatrix(glm::mat4 projection);

private:
	void sendCommonToShader(TransparentGameObjectImpl * gameObject);
	void sendCommonToShader(InstancedGameObject * gameObject);

};
