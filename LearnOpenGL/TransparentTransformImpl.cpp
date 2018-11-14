#include "TransparentTransformImpl.h"

TransparentTransformImpl::TransparentTransformImpl() {}

TransparentTransformImpl::~TransparentTransformImpl() {}

TransparentTransformImpl::TransparentTransformImpl(string  fileLocation) {
	this->readFile(fileLocation);
}

vec3 * TransparentTransformImpl::getPositions(GLuint i) {
	return &this->props[i].Position;
}

mat4 TransparentTransformImpl::getModel(GLuint i) {
	return this->props[i].model;
}

vector<mat4> TransparentTransformImpl::getModels() {
	return this->models;
}

void TransparentTransformImpl::refreshModel() {
	sort(this->props.begin(), this->props.end(), [this](InstancedTransformProps const &x, InstancedTransformProps const& y) { return this->sortFunction(x, y); });
}

void TransparentTransformImpl::Draw() {
	/* This resets to the original values from file */
	// this->newRotation = this->Rotation;
	// this->newPosition = this->Position;
	// this->newScale = this->Scale;
}


GLuint TransparentTransformImpl::getSize() {
	return this->props.size();
}

void TransparentTransformImpl::readFile(string  filename) {
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
			InstancedTransformProps prop;

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

			model = rotate(model, rotation.x, vec3(1.0f, 0.0f, 0.0f));
			model = rotate(model, rotation.y, vec3(0.0f, 1.0f, 0.0f));
			model = rotate(model, rotation.z, vec3(0.0f, 0.0f, 1.0f));

			model = translate(model, position);

			model = glm::scale(model, scale);

			prop.Position = position;
			prop.Rotation = rotation;
			prop.Scale = scale;
			prop.model = model;
			this->props.push_back(prop);

			this->models.push_back(this->props[this->props.size()-1].model);
		}
	}
	else {
		cout << "ERROR::BAD TRANSFORM FILENAME" << endl;
	}
}

// TODO: This needs to be fixed since Transform doesn't have reference back to GameObject
/* Note this sort returns the opposite of what a sort function is supposed to */
GLboolean TransparentTransformImpl::sortFunction(InstancedTransformProps const &x, InstancedTransformProps const &y) {
	//GLfloat dist1 = length(this->gameObject->getCamera()->Position - x.Position);
	//GLfloat dist2 = length(this->gameObject->getCamera()->Position - y.Position);
	//return dist1 > dist2;
	return false;
}
