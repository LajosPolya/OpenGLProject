#include "TransparentTransformImpl.h"
#include "InstancedGameObjectImpl.h"

TransparentTransformImpl::TransparentTransformImpl() {}

TransparentTransformImpl::~TransparentTransformImpl() {}

void TransparentTransformImpl::refreshModel()
{

	// Second Answer
	// https://stackoverflow.com/questions/2758080/how-to-sort-stl-vector
	// props insntance variable may need to be a std::vector instead of its attributes being one
	std::map<GLfloat, GLint> sortedWindowPosition;
	for (GLuint i = 0; i < (*(this->getPositions())).size(); i++) {
		GLfloat distance = glm::length(InstancedTransformImpl::getGameObject()->getCamera()->Position - (*(this->getPositions()))[i]);
		sortedWindowPosition[distance] = i;
	}

	// Find a way to sort this efficiently
	for (std::map<GLfloat, GLint>::reverse_iterator it = sortedWindowPosition.rbegin(); it != sortedWindowPosition.rend(); ++it)
	{

	}
}
