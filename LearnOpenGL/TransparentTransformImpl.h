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

#include <algorithm>

class TransparentGameObjectImpl;

class TransparentTransformImpl
{
public:
	TransparentTransformImpl();
	~TransparentTransformImpl();

	TransparentTransformImpl(GLchar* fileLocation, TransparentGameObjectImpl * gameObject);

	glm::vec3* getPositions(GLuint i);

	glm::mat4 getModel(GLuint i);

	std::vector<glm::mat4> getModels();

	void refreshModel();

	void Draw();

	TransparentGameObjectImpl* getGameObject();

	GLuint getSize();

private:
	struct InstancedTransformProps {
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;

		glm::mat4 model;
	};

	/* This allows me to return models which are sorted,
	   the above data structure allows me to get a model by index
	*/
	std::vector<glm::mat4> models;

	/* Reference to own GameObject instance*/
	TransparentGameObjectImpl* gameObject;

	/* Only the file should set this */
	std::vector<InstancedTransformProps> props;

	void readFile(GLchar* filename);

	GLboolean sortFunction(InstancedTransformProps const &x, InstancedTransformProps const &y);
};

