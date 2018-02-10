#include "InstancedTransformImpl.h"

InstancedTransformImpl::InstancedTransformImpl() {}

InstancedTransformImpl::~InstancedTransformImpl() {}

InstancedTransformImpl::InstancedTransformImpl(std::string fileLocation) {
	this->readFile(fileLocation);
}

std::vector<glm::vec3> * InstancedTransformImpl::getPositions() {
	return &this->Position;
}

std::vector<glm::vec3>* InstancedTransformImpl::getRotations() {
	return &this->Rotation;
}

std::vector<glm::vec3>* InstancedTransformImpl::getScales() {
	return &this->Scale;
}

std::vector<glm::mat4> InstancedTransformImpl::getModels() {
	return this->model;
}

void InstancedTransformImpl::setModels(std::vector<glm::mat4> models) {
	this->model = models;
}

void InstancedTransformImpl::setPositions(std::vector<glm::vec3> positions) {
	this->Position = positions;
}

void InstancedTransformImpl::setRotations(std::vector<glm::vec3> rotations) {
	this->Rotation = rotations;
}

void InstancedTransformImpl::setScales(std::vector<glm::vec3> scales) {
	this->Scale = scales;
}

void InstancedTransformImpl::refreshModel() {
	// this->model = glm::mat4();
}

void InstancedTransformImpl::Draw() {
	/* This resets to the original values from file */
	// this->newRotation = this->Rotation;
	// this->newPosition = this->Position;
	// this->newScale = this->Scale;
}

// TODO: Create a Base Class which will have this method (implemented by this and InstancedTransformImpl)
void InstancedTransformImpl::readFile(std::string filename) {
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;

		while (std::getline(file, line)) {
			GLchar * token;
			GLchar* context = NULL;
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
			glm::mat4 model;

			token = strtok_s(&line[0], ",", &context);
			position.x = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			position.y = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			position.z = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			rotation.x = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			rotation.y = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			rotation.z = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			scale.x = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			scale.y = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			scale.z = std::stof(token);

			model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::translate(model, position);

			model = glm::scale(model, scale);

			this->Position.push_back(position);
			this->Rotation.push_back(rotation);
			this->Scale.push_back(scale);

			this->model.push_back(model);
		}
	}
	else {
		std::cout << "ERROR::BAD TRANSFORM FILENAME" << std::endl;
	}
}
