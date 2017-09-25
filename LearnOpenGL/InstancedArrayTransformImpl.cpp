#include "InstancedArrayTransformImpl.h"
#include "InstancedArrayGameObjectImpl.h"

InstancedArrayTransformImpl::InstancedArrayTransformImpl() {}

InstancedArrayTransformImpl::~InstancedArrayTransformImpl() {}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(GLchar* fileLocation, InstancedArrayGameObjectImpl * gameObject) {
	this->gameObject = gameObject;
	this->readFile(fileLocation);
}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(std::vector<glm::vec3> positions, InstancedArrayGameObjectImpl * gameObject)
{
	this->gameObject = gameObject;
	this->setup(positions);
}

std::vector<glm::vec3>* InstancedArrayTransformImpl::getPositions()
{
	return &(this->Position);
}

std::vector<glm::mat4> InstancedArrayTransformImpl::getModels()
{
	return this->model;
}

void InstancedArrayTransformImpl::refreshModel() {
	// this->model = glm::mat4();
}

void InstancedArrayTransformImpl::Draw() {
	/* This resets to the original values from file */
	// this->newRotation = this->Rotation;
	// this->newPosition = this->Position;
	// this->newScale = this->Scale;
}

void InstancedArrayTransformImpl::readFile(GLchar * filename)
{
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;

		while (std::getline(file, line)) {
			// std::cout << line << std::endl;
			GLchar * token;
			GLchar* context = NULL;
			GLuint i = 0;
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
		std::cout << "ERROR:: CAN'T READ TRANSFORM FILE: " << filename << std::endl;
	}
}

void InstancedArrayTransformImpl::setup(std::vector<glm::vec3> positions)
{
	this->Position = positions;
	for (GLint i = 0; i < (GLint)positions.size(); i++) {
		glm::mat4 model;

		model = glm::translate(model, positions[i]);

		this->Rotation.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		this->Scale.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

		this->model.push_back(model);
	}
}
