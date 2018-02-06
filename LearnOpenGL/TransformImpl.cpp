#include "TransformImpl.h"

TransformImpl::TransformImpl() {}

TransformImpl::TransformImpl(std::string fileLocation) {
	readFile(fileLocation);
}

TransformImpl::TransformImpl(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale) {
	this->Position = Position;
	this->Rotation = Rotation;
	this->Scale = Scale;

	this->model = glm::rotate(this->model, this->Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	this->model = glm::rotate(this->model, this->Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	this->model = glm::rotate(this->model, this->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	this->model = glm::translate(this->model, this->Position);

	this->model = glm::scale(this->model, this->Scale);
}

glm::vec3 TransformImpl::getPosition() {
	return this->Position;
}

glm::vec3 TransformImpl::getRotation() {
	return this->Rotation;
}

glm::vec3 TransformImpl::getScale() {
	return this->Scale;
}

void TransformImpl::readFile(std::string filename) {
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;

		while (std::getline(file, line)) {
			// std::cout << line << std::endl;
			GLchar * token;
			GLchar * context = NULL;

			token = strtok_s(&line[0], ",", &context);
			this->Position.x = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Position.y = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Position.z = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Rotation.x = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Rotation.y = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Rotation.z = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Scale.x = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Scale.y = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Scale.z = std::stof(token);

		}
	}
	else {
		std::cout << "ERROR::BAD TRANSFORM FILENAME" << std::endl;
	}
}

void TransformImpl::setPosition() {
	this->model = glm::translate(this->model, this->newPosition);
}

void TransformImpl::setPosition(glm::vec3 pos) {
	this->newPosition = pos;
}

void TransformImpl::setYRotation(GLfloat radians) {
	this->newRotation.y = radians;
}


/* Only on x-axis currently */
void TransformImpl::setRotation() {
	this->model = glm::rotate(this->model, this->newRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 TransformImpl::getModel() {
	return this->model;
}

void TransformImpl::refreshModel() {
	this->model = glm::mat4();
}

void TransformImpl::setScale() {
	this->model = glm::scale(model, this->Scale);
}

void TransformImpl::Draw() {
	/* This resets to the original values from file */
	this->newRotation = this->Rotation;
	this->newPosition = this->Position;
	this->newScale = this->Scale;
}
