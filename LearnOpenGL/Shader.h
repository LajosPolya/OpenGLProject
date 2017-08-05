#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "LightsContainer.h"
#include "Camera.h"

#include <GL/glew.h>

class GameObjectImpl;
class InstancedArrayGameObjectImpl;
class InstancedGameObjectImpl;
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

	void setProjectionMatrix(glm::mat4 projection);
};
