#include "InstancedTransformImpl.h"
#include "GameObjectImpl.h"



InstancedTransformImpl::InstancedTransformImpl()
{
}


InstancedTransformImpl::~InstancedTransformImpl()
{
}

InstancedTransformImpl::InstancedTransformImpl(GLchar* fileLocation, GameObject* gameObject) {
	this->gameObject = gameObject;
	this->readFile(fileLocation);
}

glm::vec3 InstancedTransformImpl::getPosition() {
	return glm::vec3();
}

void InstancedTransformImpl::setPosition() {
	// this->model = glm::translate(this->model, this->newPosition);
}

void InstancedTransformImpl::setPosition(glm::vec3 pos) {
	// this->newPosition = pos;
}

void InstancedTransformImpl::setYRotation(GLfloat radians)
{
	// this->newRotation.y = radians;
}


/* Only on x-axis currently */
void InstancedTransformImpl::setRotation() {
	// this->model = glm::rotate(this->model, this->newRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 InstancedTransformImpl::getModel() {
	// return this->model;
	return glm::mat4();
}

std::vector<glm::mat4> InstancedTransformImpl::getModels()
{
	return this->model;
}

void InstancedTransformImpl::refreshModel() {
	// this->model = glm::mat4();
}

void InstancedTransformImpl::setScale() {
	// this->model = glm::scale(model, this->Scale);
}

void InstancedTransformImpl::Draw() {
	/* This resets to the original values from file */
	// this->newRotation = this->Rotation;
	// this->newPosition = this->Position;
	// this->newScale = this->Scale;
}

void InstancedTransformImpl::readFile(GLchar * filename)
{
	std::ifstream file(filename);
	GLint vertexProp_BitMap = 0;
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
		std::cout << "ERROR::BAD TRANSFORM FILENAME" << std::endl;
	}
}
