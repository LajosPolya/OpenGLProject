#pragma once


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

class InstancedGameObjectImpl;

class InstancedTransformImpl
{
public:
	InstancedTransformImpl();
	~InstancedTransformImpl();


	InstancedTransformImpl(GLchar* fileLocation, InstancedGameObjectImpl * gameObject);

	std::vector<glm::mat4> getModels();

	void refreshModel();

	void Draw();

private:
	/* Reference to own GameObject instance*/
	InstancedGameObjectImpl* gameObject;


	/* Only the file should set this */
	std::vector<glm::vec3> Position;
	std::vector<glm::vec3> Rotation;
	std::vector<glm::vec3> Scale;

	std::vector<glm::mat4> model;

	void readFile(GLchar* filename);
};

