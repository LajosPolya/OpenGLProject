#include "ComplexPosition.h"


ComplexPosition::ComplexPosition() {
	this->positions = std::vector<std::vector<glm::vec3>>(2);
	this->drawablePositions = std::vector<std::vector<glm::vec3>>(2);
}
ComplexPosition::~ComplexPosition() {}

std::vector<glm::vec3> ComplexPosition::getPositions(CompositionType  i) {
	return this->positions[i];
}

void ComplexPosition::setPositions(std::vector<glm::vec3> positions, CompositionType i) {
	this->positions[i] = positions;
}

std::vector<glm::vec3> ComplexPosition::getDrawablePositions(CompositionType i) {
	return this->drawablePositions[i];
}

void ComplexPosition::setDrawablePositions(std::vector<glm::vec3> drawablePositions, CompositionType i) {
	this->drawablePositions[i] = drawablePositions;
}
