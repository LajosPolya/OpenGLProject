#include "ComplexPosition.h"


ComplexPosition::ComplexPosition() {
	this->positions = vector<vector<vec3>>(2);
	this->drawablePositions = vector<vector<vec3>>(2);
}
ComplexPosition::~ComplexPosition() {}

vector<vec3> ComplexPosition::getPositions(CompositionType  i) {
	return this->positions[i];
}

void ComplexPosition::setPositions(vector<vec3> positions, CompositionType i) {
	this->positions[i] = positions;
}

vector<vec3> ComplexPosition::getDrawablePositions(CompositionType i) {
	return this->drawablePositions[i];
}

void ComplexPosition::setDrawablePositions(vector<vec3> drawablePositions, CompositionType i) {
	this->drawablePositions[i] = drawablePositions;
}
