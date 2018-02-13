#include "PositionRelativeCamera.h"


PositionRelativeCamera::PositionRelativeCamera() {}
PositionRelativeCamera::~PositionRelativeCamera() {}

InstancedArrayTransformImpl PositionRelativeCamera::getTransform() {
	return this->transform;
}

GLuint PositionRelativeCamera::getIndex() {
	return this->index;
}
