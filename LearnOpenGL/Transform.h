#pragma once

#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

class Transform {
public:

	virtual glm::mat4 getModel() = 0;

	virtual void refreshModel() = 0;

	virtual void Draw() = 0;
};
