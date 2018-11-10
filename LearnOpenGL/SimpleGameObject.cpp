#include "SimpleGameObject.h"


SimpleGameObject::SimpleGameObject(string meshPath, string transformPath) : SimpleGameObject("", "", meshPath, "", transformPath) {}
SimpleGameObject::SimpleGameObject(string diffuseMapPath, string meshPath, string transformPath) : SimpleGameObject(diffuseMapPath, "", meshPath, "", transformPath) {}


SimpleGameObject::SimpleGameObject(string diffuseMapPath, string specularMapPath, string meshPath, string materialPath, string transformPath) {
	this->transform = new TransformImpl(transformPath);

	vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapPath);
	vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapPath);
	this->mesh = makeMeshes(meshPath, diffuseMaps, specularMaps);
}

SimpleGameObject::SimpleGameObject(string meshPath, Transform * transform) {
	this->transform = transform;
	this->mesh = makeMeshes(meshPath, vector<Texture*>(), vector<Texture*>());
}


SimpleGameObject::~SimpleGameObject() {}

void SimpleGameObject::Draw() {
	this->transform->refreshModel();
	this->transform->setRotation();
	this->transform->setPosition();
	this->transform->setScale();
	this->transform->Draw();

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		this->mesh[i]->Draw();
	}
}

Transform * SimpleGameObject::getTransform() {
	return this->transform;
}

vector<Mesh*> SimpleGameObject::makeMeshes(string path, vector<Texture*> diffuseMaps, vector<Texture*> specularMaps) {
	GLchar * tokens;
	GLchar * context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	vector<Mesh*> mesh;
	GLuint i = 0;
	Texture * tempDif;
	Texture * tempSpec;
	while (tokens != NULL) {
		tempDif = nullptr;
		if (i < diffuseMaps.size()) {
			tempDif = diffuseMaps[i];
		}
		tempSpec = nullptr;
		if (i < specularMaps.size()) {
			tempSpec = specularMaps[i];
		}
		mesh.push_back(new Mesh(tokens, tempDif, tempSpec));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}
