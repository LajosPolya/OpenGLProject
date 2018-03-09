#pragma once

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

class TransformImpl : public Transform {

public:

	TransformImpl();
	TransformImpl(const TransformImpl & toCopy);

	TransformImpl(std::string fileLocation);
	TransformImpl(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale);

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();

	void setPosition();
	void setPosition(glm::vec3 pos);

	void setRotation();
	void setYRotation(GLfloat radians);

	void setScale();

	glm::mat4 getModel();

	void refreshModel();

	void Draw();

private:

	/* Only the file should set this */
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	/* These get updated which update the model */
	glm::vec3 newPosition;
	glm::vec3 newRotation;
	glm::vec3 newScale;

	glm::mat4 model;

	void readFile(std::string filename);
};
