#include "Transform.h"
#include "GameObject.h"

Transform::Transform() {

}

Transform::Transform(GLchar* fileLocation, GameObject* gameObject) {
	this->gameObject = gameObject;
	readFile(fileLocation);

	std::cout << Position.x << " " << Position.y << " " << Position.z << std::endl;
	std::cout << Scale.x << " " << Scale.y << " " << Scale.z << std::endl;
	std::cout << Rotation.x << " " << Rotation.y << " " << Rotation.z << std::endl;
}

glm::vec3 Transform::getPosition() {
	return this->Position;
}

void Transform::readFile(GLchar* filename) {
	std::ifstream file(filename);
	GLint vertexProp_BitMap = 0;
	if (file.is_open()) {
		std::string line;

		while (std::getline(file, line)) {
			std::cout << line << std::endl;
			GLchar * token;
			GLchar* context = NULL;
			GLuint i = 0;
			glm::vec3 prop;
			GLchar* propType = '\0';

			propType = strtok_s(&line[0], ",", &context);

			token = strtok_s(NULL, ",", &context);
			while (token != NULL) {
				prop[i] = std::stof(token, NULL);
				token = strtok_s(NULL, ",", &context);
				i++;
			}

			if (propType[0] == 'P') {
				Position = prop;
			}
			else if (propType[0] == 'S') {
				Scale = prop;
			}
			else if (propType[0] == 'R') { /* Rotation May Not Be Used Yet */
				Rotation = prop;
			}


		}
	}
	else {
		std::cout << "ERROR::BAD TRANSFORM FILENAME" << std::endl;
	}
}

void Transform::setPosition() {
	this->model = glm::translate(this->model, this->newPosition);
}

void Transform::setPosition(glm::vec3 pos) {
	this->newPosition = pos;
}

void Transform::setYRotation(GLfloat radians)
{
	this->newRotation.y = radians;
}


/* Only on x-axis currently */
void Transform::setRotation() {
	//std::cout << this->model[0][0] << " " << this->model[0][1] << " " << this->model[0][2] << " " << std::endl;
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->model = glm::rotate(this->model, this->newRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//std::cout << this->model[0][0] << " " << this->model[0][1] << " " << this->model[0][2] << " " << std::endl;
}

glm::mat4 Transform::getModel() {
	return this->model;
}

void Transform::refreshModel() {
	this->model = glm::mat4();
}

void Transform::setScale() {
	this->model = glm::scale(model, this->Scale);
}

void Transform::Draw() {
	/* This resets to the original values from file */
	this->newRotation = this->Rotation;
	this->newPosition = this->Position;
	this->newScale = this->Scale;
}
