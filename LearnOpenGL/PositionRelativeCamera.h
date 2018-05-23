#pragma once

#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "InstancedArrayTransformImpl.h"
#include "ChunkObjectTransformStore.h"


class PositionRelativeCamera
{
public:
	PositionRelativeCamera(ChunkObjectTransformStore transform, GLuint index);
	~PositionRelativeCamera();

	InstancedArrayTransformImpl getTransform(CompositionType i);

	GLuint getIndex();

private:
	ChunkObjectTransformStore transform;
	GLuint index;
};

