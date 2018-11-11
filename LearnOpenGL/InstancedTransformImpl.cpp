#include "InstancedTransformImpl.h"

InstancedTransformImpl::InstancedTransformImpl() {}

InstancedTransformImpl::~InstancedTransformImpl() {}

InstancedTransformImpl::InstancedTransformImpl(const string & fileLocation) {
	this->readFile(fileLocation);
}

InstancedTransformImpl::InstancedTransformImpl(vector<vec3> positions, vector<vec3> rotations, vector<vec3> scales) {
	if (positions.size() != rotations.size() && rotations.size() != scales.size()) {
		cout << "ERROR::InstancedTransformImpl::Different Sized Transform Properties" << endl;
	}

	this->Position = positions;
	this->Rotation = rotations;
	this->Scale = scales;

	// Assume Position, Rotation, Scale are all of same size
	for (GLuint i = 0; i < this->Position.size(); i++) {
		mat4 model = mat4();
		model = rotate(model, this->Rotation[i].x, { 1.0f, 0.0f, 0.0f });
		model = rotate(model, this->Rotation[i].y, { 0.0f, 1.0f, 0.0f });
		model = rotate(model, this->Rotation[i].z, { 0.0f, 0.0f, 1.0f });

		model = translate(model, this->Position[i]);

		model = scale(model, this->Scale[i]);

		this->model.push_back(model);
	}
}

vector<vec3> * InstancedTransformImpl::getPositions() {
	return &this->Position;
}

const vector<vec3> * InstancedTransformImpl::getPositions() const {
	return &this->Position;
}

vector<vec3>* InstancedTransformImpl::getRotations() {
	return &this->Rotation;
}

vector<vec3>* InstancedTransformImpl::getScales() {
	return &this->Scale;
}

vector<mat4> InstancedTransformImpl::getModels() {
	return this->model;
}

void InstancedTransformImpl::setModels(vector<mat4> models) {
	this->model = models;
}

void InstancedTransformImpl::setPositions(vector<vec3> positions) {
	this->Position = positions;
}

void InstancedTransformImpl::setRotations(vector<vec3> rotations) {
	this->Rotation = rotations;
}

void InstancedTransformImpl::setScales(vector<vec3> scales) {
	this->Scale = scales;
}

void InstancedTransformImpl::addNewInstance(vec3 position, vec3 rotation, vec3 scale) {
	mat4 model;

	model = doTransformOperations(position, rotation, scale);

	this->Position.push_back(position);
	this->Rotation.push_back(rotation);
	this->Scale.push_back(scale);

	this->model.push_back(model);
}

void InstancedTransformImpl::refreshModel() {
	// this->model = mat4();
}

void InstancedTransformImpl::Draw() {
	/* This resets to the original values from file */
	// this->newRotation = this->Rotation;
	// this->newPosition = this->Position;
	// this->newScale = this->Scale;
}

// TODO: Create a Base Class which will have this method (implemented by this and InstancedTransformImpl)
void InstancedTransformImpl::readFile(const string & filename) {
	ifstream file(filename);
	if (file.is_open()) {
		string line;

		while (getline(file, line)) {
			GLchar * token;
			GLchar * context = NULL;
			vec3 position;
			vec3 rotation;
			vec3 scale;
			mat4 model;

			token = strtok_s(&line[0], ",", &context);
			position.x = stof(token);

			token = strtok_s(NULL, ",", &context);
			position.y = stof(token);

			token = strtok_s(NULL, ",", &context);
			position.z = stof(token);

			token = strtok_s(NULL, ",", &context);
			rotation.x = stof(token);

			token = strtok_s(NULL, ",", &context);
			rotation.y = stof(token);

			token = strtok_s(NULL, ",", &context);
			rotation.z = stof(token);

			token = strtok_s(NULL, ",", &context);
			scale.x = stof(token);

			token = strtok_s(NULL, ",", &context);
			scale.y = stof(token);

			token = strtok_s(NULL, ",", &context);
			scale.z = stof(token);

			model = doTransformOperations(position, rotation, scale);

			this->Position.push_back(position);
			this->Rotation.push_back(rotation);
			this->Scale.push_back(scale);

			this->model.push_back(model);
		}
	}
	else {
		cout << "ERROR::BAD TRANSFORM FILENAME" << endl;
	}
}

mat4 InstancedTransformImpl::doTransformOperations(vec3 position, vec3 rotation, vec3 scale) {
	mat4 model;

	model = rotate(model, rotation.x, vec3(1.0f, 0.0f, 0.0f));
	model = rotate(model, rotation.y, vec3(0.0f, 1.0f, 0.0f));
	model = rotate(model, rotation.z, vec3(0.0f, 0.0f, 1.0f));

	model = translate(model, position);

	model = glm::scale(model, scale);

	return model;
}
