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

#include "InstancedTransform.h"

class InstancedArrayGameObjectImpl;

class InstancedArrayTransformImpl : public InstancedTransform
{
public:
	InstancedArrayTransformImpl();
	~InstancedArrayTransformImpl();


	InstancedArrayTransformImpl(GLchar* fileLocation, InstancedArrayGameObjectImpl * gameObject);

	std::vector<glm::vec3>* getPositions();

	std::vector<glm::mat4> getModels();

	void refreshModel();

	void Draw();

private:
	/* Reference to own GameObject instance*/
	InstancedArrayGameObjectImpl* gameObject;

	/* Only the file should set this */
	InstancedTransformProps props;

	void readFile(GLchar* filename);
};
