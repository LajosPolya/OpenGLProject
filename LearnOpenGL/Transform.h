#pragma once

#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

using namespace glm;

class Transform {
public:

	virtual vec3 getPosition() = 0;

	virtual void setPosition() = 0;
	virtual void setPosition(vec3 pos) = 0;

	virtual void setRotation() = 0;
	virtual void setYRotation(GLfloat radians) = 0;

	virtual void setScale() = 0;

	virtual mat4 getModel() = 0;

	virtual void refreshModel() = 0;

	virtual void Draw() = 0;
};
