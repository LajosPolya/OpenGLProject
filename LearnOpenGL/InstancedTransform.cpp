#include "InstancedTransform.h"



InstancedTransform::InstancedTransform()
{
}


InstancedTransform::~InstancedTransform()
{
}

InstancedTransform::InstancedTransform(GLchar* fileLocation, GameObject* gameObject) {

}

glm::vec3 InstancedTransform::getPosition() {
	return glm::vec3();
}

void InstancedTransform::setPosition() {
	// this->model = glm::translate(this->model, this->newPosition);
}

void InstancedTransform::setPosition(glm::vec3 pos) {
	// this->newPosition = pos;
}

void InstancedTransform::setYRotation(GLfloat radians)
{
	// this->newRotation.y = radians;
}


/* Only on x-axis currently */
void InstancedTransform::setRotation() {
	// this->model = glm::rotate(this->model, this->newRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 InstancedTransform::getModel() {
	// return this->model;
	return glm::mat4();
}

void InstancedTransform::refreshModel() {
	// this->model = glm::mat4();
}

void InstancedTransform::setScale() {
	// this->model = glm::scale(model, this->Scale);
}

void InstancedTransform::Draw() {
	/* This resets to the original values from file */
	// this->newRotation = this->Rotation;
	// this->newPosition = this->Position;
	// this->newScale = this->Scale;
}