#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Material.h"

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

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void Use();

	void sendToShader(DirLight * dirLight, SpotLight * spotLight, std::vector<PointLight> * pointLights);
	void sendToShader(GameObjectImpl * gameObject);
	void sendToShader(InstancedArrayGameObjectImpl * gameObject);
	void sendToShader(InstancedGameObjectImpl * gameObject);
	void sendToShader(TransparentGameObjectImpl * gameObject);
	void sendToShader(Material * material);

	void setProjectionMatrix(glm::mat4 projection);

private:
	void sendCommonToShader(InstancedGameObject * gameObject);

};
