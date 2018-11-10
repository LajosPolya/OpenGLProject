#include "InstancedArrayTransformImpl.h"

InstancedArrayTransformImpl::InstancedArrayTransformImpl() {}

InstancedArrayTransformImpl::~InstancedArrayTransformImpl() {}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(const InstancedArrayTransformImpl & toCopy) {

	this->Position = toCopy.Position;
	this->Rotation = toCopy.Rotation;
	this->Scale = toCopy.Scale;

	this->model = toCopy.model;
}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(string fileLocation) {
	this->readFile(fileLocation);
}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(vector<vec3> positions) {
	this->setup(positions);
}

InstancedArrayTransformImpl::InstancedArrayTransformImpl(vector<vec3> positions, vector<vec3> rotations, vector<vec3> scales) {
	this->Position = positions;
	this->Rotation = rotations;
	this->Scale = scales;
	
	if (this->Rotation.size() == this->Position.size() && this->Position.size() == this->Scale.size()) {
		mat4 model;
		for (GLuint i = 0; i < this->Position.size(); i++) {
			model = {};
			model = rotate(model, this->Rotation[i].x, vec3(1.0f, 0.0f, 0.0f));
			model = rotate(model, this->Rotation[i].y, vec3(0.0f, 1.0f, 0.0f));
			model = rotate(model, this->Rotation[i].z, vec3(0.0f, 0.0f, 1.0f));

			model = translate(model, this->Position[i]);

			model = scale(model, this->Scale[i]);

			this->model.push_back(model);
		}
	}
	else {
		cout << "ERROR:: Instanced_Array_Transform:: MODEL NOT CREATED" << endl;
	}
}

vector<vec3>* InstancedArrayTransformImpl::getPositions() {
	return &(this->Position);
}

vector<vec3>* InstancedArrayTransformImpl::getRotations() {
	return &(this->Rotation);
}

vector<vec3>* InstancedArrayTransformImpl::getScales() {
	return &(this->Scale);
}

void InstancedArrayTransformImpl::setPositions(vector<vec3> positions) {
	this->Position = positions;
}

void InstancedArrayTransformImpl::setRotations(vector<vec3> rotations) {
	this->Rotation = rotations;
}

void InstancedArrayTransformImpl::setScales(vector<vec3> scales) {
	this->Scale = scales;
}

void InstancedArrayTransformImpl::setModels(vector<mat4> models) {
	this->model = models;
}

vector<mat4> InstancedArrayTransformImpl::getModels() {
	return this->model;
}

void InstancedArrayTransformImpl::refreshModel() {
	// this->model = mat4();
}

void InstancedArrayTransformImpl::Draw() {
	/* This resets to the original values from file */
	// this->newRotation = this->Rotation;
	// this->newPosition = this->Position;
	// this->newScale = this->Scale;
}

void InstancedArrayTransformImpl::readFile(string filename) {
	ifstream file(filename);
	if (file.is_open()) {
		string line;

		while (getline(file, line)) {
			GLchar * token;
			GLchar* context = NULL;
			GLuint i = 0;
			vec3 _position;
			vec3 _rotation;
			vec3 _scale;
			mat4 model;

			token = strtok_s(&line[0], ",", &context);
			_position.x = stof(token);

			token = strtok_s(NULL, ",", &context);
			_position.y = stof(token);

			token = strtok_s(NULL, ",", &context);
			_position.z = stof(token);

			token = strtok_s(NULL, ",", &context);
			_rotation.x = stof(token);

			token = strtok_s(NULL, ",", &context);
			_rotation.y = stof(token);

			token = strtok_s(NULL, ",", &context);
			_rotation.z = stof(token);

			token = strtok_s(NULL, ",", &context);
			_scale.x = stof(token);

			token = strtok_s(NULL, ",", &context);
			_scale.y = stof(token);

			token = strtok_s(NULL, ",", &context);
			_scale.z = stof(token);

			model = rotate(model, _rotation.x, vec3(1.0f, 0.0f, 0.0f));
			model = rotate(model, _rotation.y, vec3(0.0f, 1.0f, 0.0f));
			model = rotate(model, _rotation.z, vec3(0.0f, 0.0f, 1.0f));

			model = translate(model, _position);

			model = scale(model, _scale);

			this->Position.push_back(_position);
			this->Rotation.push_back(_rotation);
			this->Scale.push_back(_scale);

			this->model.push_back(model);
		}
	}
	else {
		cout << "ERROR:: CAN'T READ TRANSFORM FILE: " << filename << endl;
	}
}

void InstancedArrayTransformImpl::setup(vector<vec3> positions) {
	this->Position = positions;
	for (GLint i = 0; i < (GLint)positions.size(); i++) {
		mat4 model;

		model = translate(model, positions[i]);

		this->Rotation.push_back(vec3(0.0f, 0.0f, 0.0f));
		this->Scale.push_back(vec3(0.0f, 0.0f, 0.0f));

		this->model.push_back(model);
	}
}
