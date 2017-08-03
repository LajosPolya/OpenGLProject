#pragma once

#ifndef GAMEOBJECT_IMPL_H
#define GAMEOBJECT_IMPL_H

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// My Code
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "TransformImpl.h"
#include "InstancedArrayTransformImpl.h"
#include "LightsContainer.h"
#include "GameObject.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class GameObjectImpl : public GameObject {
public:

	/* In the future have a Constructor which doesn't take the lightsLoc and allow the LightsContainer to be initialized independenty */
	GameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* specularMapLoc, GLchar* meshLoc, GLchar* materialLoc, GLchar* transformLoc, GLchar* lightsLoc, Camera * camera, glm::mat4 projection);
	GameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* meshLoc, GLchar* transformLoc, Camera * camera, glm::mat4 projection);
	GameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* meshLoc, GLchar* transformLoc, Camera * camera, glm::mat4 projection);
	
	void Draw();

	Shader* getShader();

	Camera* getCamera();

	TransformImpl* getTransform();

	Texture* getDiffuseMap();
	Texture* getSpecularMap();

	LightsContainer* getLightsContainer();

private:
	Mesh* mesh;

	Material* material;

	Texture* diffuseMap;
	Texture* specularMap;

	Shader* shader;

	Camera* camera;

	TransformImpl* transform;

	LightsContainer* lightsContainer;

	glm::mat4 projection;

};

#endif // !GAMEOBJECT_IMPL_H
