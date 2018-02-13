#include "PositionRelativeCamera.h"


PositionRelativeCamera::PositionRelativeCamera(InstancedArrayTransformImpl transform, GLuint index) {
	this->transform = transform;
	this->index = index;
}
PositionRelativeCamera::~PositionRelativeCamera() {}

InstancedArrayTransformImpl PositionRelativeCamera::getTransform() {
	return this->transform;
}

GLuint PositionRelativeCamera::getIndex() {
	return this->index;
}
