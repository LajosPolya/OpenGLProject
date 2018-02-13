#pragma once

#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "InstancedArrayTransformImpl.h"


class PositionRelativeCamera
{
public:
	PositionRelativeCamera();
	~PositionRelativeCamera();

	InstancedArrayTransformImpl getTransform();

	GLuint getIndex();

private:
	InstancedArrayTransformImpl transform;
	GLuint index;
};

