#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

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
#include "Transform.h"
#include "Renderer.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class GameObject {
public:

	GameObject(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* specularMapLoc, GLchar* meshLoc, GLchar* materialLoc, GLchar* transformLoc, Camera * camera, glm::mat4 projection);
	
	void Draw();

	Shader* getShader();

	Camera* getCamera();

	Transform* getTransform();

	Texture* getDiffuseMap();
	Texture* getSpecularMap();

private:
	Mesh* mesh;

	Material* material;

	Texture* diffuseMap;
	Texture* specularMap;

	Shader* shader;

	Camera* camera;

	Transform* transform;

	Renderer* renderer;

	glm::mat4 projection;

};

#endif // !GAMEOBJECT_H
