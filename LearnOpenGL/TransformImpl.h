#pragma once

#ifndef TRANSFORMIMPL_H
#define TRANSFORMIMPL_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#include "Transform.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <map>

class GameObjectImpl;

class TransformImpl : public Transform {

public:

	TransformImpl();

	TransformImpl(GLchar* fileLocation, GameObjectImpl* gameObject);

	glm::vec3 getPosition();

	void setPosition();
	void setPosition(glm::vec3 pos);

	void setRotation();
	void setYRotation(GLfloat radians);

	void setScale();

	glm::mat4 getModel();
	std::vector<glm::mat4> getModels();

	void refreshModel();

	void Draw();

private:
	/* Reference to own GameObject instance*/
	GameObjectImpl* gameObject;


	/* Only the file should set this */
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	/* These get updated which update the model */
	glm::vec3 newPosition;
	glm::vec3 newRotation;
	glm::vec3 newScale;

	glm::mat4 model;

	void readFile(GLchar* filename);
};

#endif // !TRANSFORMIMPL_H