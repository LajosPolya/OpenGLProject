#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// My Code
#include "Material.h"
#include "GameObject.h"
#include "GameObjectUtils.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class GameObjectImpl : public GameObject {
public:

	/* In the future have a Constructor which doesn't take the lightsLoc and allow the LightsContainer to be initialized independenty */
	GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * specularMapLoc, std::string meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);
	GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, std::string meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection);
	GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection);
	
	void Draw();

	Shader * getShader();

	Camera * getCamera();

	Transform * getTransform();

	Texture * getDiffuseMap(GLuint i);
	Texture * getSpecularMap(GLuint i);

	LightsContainer * getLightsContainer();

private:
	std::vector<Mesh*> mesh;

	Material * material;

	Shader * shader;

	Camera * camera;

	Transform * transform;

	LightsContainer * lightsContainer;

	glm::mat4 projection;
};
