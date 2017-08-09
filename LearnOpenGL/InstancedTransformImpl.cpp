#include "InstancedTransformImpl.h"
#include "InstancedGameObjectImpl.h"

InstancedTransformImpl::InstancedTransformImpl() {}

InstancedTransformImpl::~InstancedTransformImpl() {}

InstancedTransformImpl::InstancedTransformImpl(GLchar* fileLocation, InstancedGameObjectImpl * gameObject) {
	this->gameObject = gameObject;
	this->readFile(fileLocation);
}

std::vector<glm::vec3>* InstancedTransformImpl::getPositions()
{
	return &this->props.Position;
}

std::vector<glm::mat4> InstancedTransformImpl::getModels()
{
	return this->props.model;
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

InstancedGameObjectImpl * InstancedTransformImpl::getGameObject()
{
	return this->gameObject;
}

// TODO: Create a Base Class which will have this method (implemented by this and InstancedTransformImpl)
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

			this->props.Position.push_back(position);
			this->props.Rotation.push_back(rotation);
			this->props.Scale.push_back(scale);

			this->props.model.push_back(model);
		}
	}
	else {
		std::cout << "ERROR::BAD TRANSFORM FILENAME" << std::endl;
	}
}
