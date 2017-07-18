#include "TransformImpl.h"
#include "GameObject.h"

TransformImpl::TransformImpl() {

}

TransformImpl::TransformImpl(GLchar* fileLocation, GameObject* gameObject) {
	this->gameObject = gameObject;
	readFile(fileLocation);

	std::cout << Position.x << " " << Position.y << " " << Position.z << std::endl;
	std::cout << Scale.x << " " << Scale.y << " " << Scale.z << std::endl;
	std::cout << Rotation.x << " " << Rotation.y << " " << Rotation.z << std::endl;
}

glm::vec3 TransformImpl::getPosition() {
	return this->Position;
}

void TransformImpl::readFile(GLchar* filename) {
	std::ifstream file(filename);
	GLint vertexProp_BitMap = 0;
	if (file.is_open()) {
		std::string line;

		while (std::getline(file, line)) {
			// std::cout << line << std::endl;
			GLchar * token;
			GLchar* context = NULL;
			GLuint i = 0;

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

void TransformImpl::setYRotation(GLfloat radians)
{
	this->newRotation.y = radians;
}


/* Only on x-axis currently */
void TransformImpl::setRotation() {
	//std::cout << this->model[0][0] << " " << this->model[0][1] << " " << this->model[0][2] << " " << std::endl;
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->model = glm::rotate(this->model, this->newRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//std::cout << this->model[0][0] << " " << this->model[0][1] << " " << this->model[0][2] << " " << std::endl;
}

glm::mat4 TransformImpl::getModel() {
	return this->model;
}

std::vector<glm::mat4> TransformImpl::getModels()
{
	return std::vector<glm::mat4>();
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