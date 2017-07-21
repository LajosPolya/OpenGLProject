#pragma once

#ifndef INSTANCED_TRANSFORM_IMPL_H
#define INSTANCED_TRANSFORM_IMPL_H

#include "Transform.h"


#include <fstream>
#include <string>
#include <iostream>
#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

class GameObject;

class InstancedTransformImpl : public Transform
{
public:
	InstancedTransformImpl();
	~InstancedTransformImpl();


	InstancedTransformImpl(GLchar* fileLocation, GameObject* gameObject);

	glm::mat4 getModel();

	std::vector<glm::mat4> getModels();

	void refreshModel();

	void Draw();

private:
	/* Reference to own GameObject instance*/
	GameObject* gameObject;


	/* Only the file should set this */
	std::vector<glm::vec3> Position;
	std::vector<glm::vec3> Rotation;
	std::vector<glm::vec3> Scale;

	std::vector<glm::mat4> model;

	void readFile(GLchar* filename);
};

#endif // !INSTANCED_TRANSFORM_IMPL_H

