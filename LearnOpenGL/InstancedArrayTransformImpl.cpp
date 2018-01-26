#include "InstancedArrayTransformImpl.h"
#include "InstancedArrayGameObjectImpl.h"

InstancedArrayTransformImpl::InstancedArrayTransformImpl() {}

InstancedArrayTransformImpl::~InstancedArrayTransformImpl() {}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(const InstancedArrayTransformImpl & toCopy) {

	this->Position = toCopy.Position;
	this->Rotation = toCopy.Rotation;
	this->Scale = toCopy.Scale;

	this->model = toCopy.model;
}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(std::string fileLocation, InstancedArrayGameObjectImpl * gameObject) {
	this->gameObject = gameObject;
	this->readFile(fileLocation);
}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(std::vector<glm::vec3> positions, InstancedArrayGameObjectImpl * gameObject)
{
	this->gameObject = gameObject;
	this->setup(positions);
}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(std::vector<glm::vec3> positions, std::vector<glm::vec3> rotations, std::vector<glm::vec3> scales, InstancedArrayGameObjectImpl * gameObject)
{
	this->Position = positions;
	this->Rotation = rotations;
	this->Scale = scales;
	
	if (this->Rotation.size() == this->Position.size() && this->Position.size() == this->Scale.size()) {
		glm::mat4 model;
		for (GLuint i = 0; i < this->Position.size(); i++) {
			model = {};
			model = glm::rotate(model, this->Rotation[i].x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, this->Rotation[i].y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, this->Rotation[i].z, glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::translate(model, this->Position[i]);

			model = glm::scale(model, this->Scale[i]);

			this->model.push_back(model);
		}
	}
	else {
		std::cout << "ERROR:: Instanced_Array_Transform:: MODEL NOT CREATED" << std::endl;
	}

	this->gameObject = gameObject;
}

void InstancedArrayTransformImpl::setGameObject(InstancedArrayGameObjectImpl * instancedArrayGameObject) {
	this->gameObject = instancedArrayGameObject;
}

std::vector<glm::vec3>* InstancedArrayTransformImpl::getPositions()
{
	return &(this->Position);
}

std::vector<glm::vec3>* InstancedArrayTransformImpl::getRotations()
{
	return &(this->Rotation);
}

std::vector<glm::vec3>* InstancedArrayTransformImpl::getScales()
{
	return &(this->Scale);
}

void InstancedArrayTransformImpl::setPositions(std::vector<glm::vec3> positions) {
	this->Position = positions;
}

void InstancedArrayTransformImpl::setRotations(std::vector<glm::vec3> rotations) {
	this->Rotation = rotations;
}

void InstancedArrayTransformImpl::setScales(std::vector<glm::vec3> scales) {
	this->Scale = scales;
}

void InstancedArrayTransformImpl::setModels(std::vector<glm::mat4> models) {
	this->model = models;
}

std::vector<glm::mat4> InstancedArrayTransformImpl::getModels() {
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

void InstancedArrayTransformImpl::readFile(std::string filename)
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
