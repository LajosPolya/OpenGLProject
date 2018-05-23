#include "PositionRelativeCamera.h"


PositionRelativeCamera::PositionRelativeCamera(ChunkObjectTransformStore transform, GLuint index) {
	this->transform = transform;
	this->index = index;
}
PositionRelativeCamera::~PositionRelativeCamera() {}

InstancedArrayTransformImpl PositionRelativeCamera::getTransform(CompositionType i) {
	return this->transform.getTransform(i);
}

GLuint PositionRelativeCamera::getIndex() {
	return this->index;
}
