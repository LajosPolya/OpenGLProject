#include "TransformImpl.h"

TransformImpl::TransformImpl() {}

TransformImpl::TransformImpl(const TransformImpl & toCopy) {
	this->Position = toCopy.Position;
	this->Rotation = toCopy.Rotation;
	this->Scale = toCopy.Scale;

	this->newPosition = toCopy.newPosition;
	this->newRotation = toCopy.newRotation;
	this->newScale = toCopy.newScale;

	this->model = toCopy.model;
}

TransformImpl::TransformImpl(string fileLocation) {
	readFile(fileLocation);
}

TransformImpl::TransformImpl(vec3 Position, vec3 Rotation, vec3 Scale) {
	this->Position = Position;
	this->Rotation = Rotation;
	this->Scale = Scale;

	this->model = rotate(this->model, this->Rotation.x, vec3(1.0f, 0.0f, 0.0f));
	this->model = rotate(this->model, this->Rotation.y, vec3(0.0f, 1.0f, 0.0f));
	this->model = rotate(this->model, this->Rotation.z, vec3(0.0f, 0.0f, 1.0f));

	this->model = translate(this->model, this->Position);

	this->model = scale(this->model, this->Scale);
}

vec3 TransformImpl::getPosition() {
	return this->Position;
}

vec3 TransformImpl::getRotation() {
	return this->Rotation;
}

vec3 TransformImpl::getScale() {
	return this->Scale;
}

void TransformImpl::readFile(string filename) {
	ifstream file(filename);
	if (file.is_open()) {
		string line;

		while (getline(file, line)) {
			// cout << line << endl;
			GLchar * token;
			GLchar * context = NULL;

			token = strtok_s(&line[0], ",", &context);
			this->Position.x = stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Position.y = stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Position.z = stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Rotation.x = stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Rotation.y = stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Rotation.z = stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Scale.x = stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Scale.y = stof(token);

			token = strtok_s(NULL, ",", &context);
			this->Scale.z = stof(token);

		}
	}
	else {
		cout << "ERROR::BAD TRANSFORM FILENAME" << endl;
	}
}

void TransformImpl::setPosition() {
	this->model = translate(this->model, this->newPosition);
}

void TransformImpl::setPosition(vec3 pos) {
	this->newPosition = pos;
}

void TransformImpl::setYRotation(GLfloat radians) {
	this->newRotation.y = radians;
}


/* Only on x-axis currently */
void TransformImpl::setRotation() {
	this->model = rotate(this->model, this->newRotation.y, { 0.0f, 1.0f, 0.0f });
}

mat4 TransformImpl::getModel() {
	return this->model;
}

void TransformImpl::refreshModel() {
	this->model = mat4();
}

void TransformImpl::setScale() {
	this->model = scale(model, this->Scale);
}

void TransformImpl::Draw() {
	/* This resets to the original values from file */
	this->newRotation = this->Rotation;
	this->newPosition = this->Position;
	this->newScale = this->Scale;
}
