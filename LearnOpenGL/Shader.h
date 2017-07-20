#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "LightsContainer.h"
#include "Camera.h"

#include <GL/glew.h>

class GameObjectImpl;
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

	void setProjectionMatrix(glm::mat4 projection);
};

#endif // !SHADER_H
