#include "ComplexPosition.h"


ComplexPosition::ComplexPosition() {}
ComplexPosition::~ComplexPosition() {}

std::vector<glm::vec3> ComplexPosition::getPositions() {
	return this->positions;
}

void ComplexPosition::setPositions(std::vector<glm::vec3> positions) {
	this->positions = positions;
}

std::vector<glm::vec3> ComplexPosition::getDrawablePositions() {
	return this->drawablePositions;
}

void ComplexPosition::setDrawablePositions(std::vector<glm::vec3> drawablePositions) {
	this->drawablePositions = drawablePositions;
}
