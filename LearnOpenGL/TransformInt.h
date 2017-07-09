#pragma once


#ifndef TRANSFORM_INT_H
#define TRANSFORM_INT_H

#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

class TransformInt {
public:

	virtual glm::vec3 getPosition() = 0;

	virtual void setPosition() = 0;
	virtual void setPosition(glm::vec3 pos) = 0;

	virtual void setRotation() = 0;
	virtual void setYRotation(GLfloat radians) = 0;

	virtual void setScale() = 0;

	virtual glm::mat4 getModel() = 0;

	// Testing for Instancing
	virtual std::vector<glm::mat4> getModels() = 0;

	virtual void refreshModel() = 0;

	virtual void Draw() = 0;
};

#endif // !TRANSFORM_INT_H
