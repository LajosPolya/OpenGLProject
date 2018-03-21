#include "SimpleGameObject.h"


SimpleGameObject::SimpleGameObject(std::string meshPath, std::string transformPath) : SimpleGameObject("", "", meshPath, "", transformPath) {}
SimpleGameObject::SimpleGameObject(std::string diffuseMapPath, std::string meshPath, std::string transformPath) : SimpleGameObject(diffuseMapPath, "", meshPath, "", transformPath) {}


SimpleGameObject::SimpleGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string materialPath, std::string transformPath) {
	this->transform = new TransformImpl(transformPath);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapPath);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapPath);
	this->mesh = makeMeshes(meshPath, diffuseMaps, specularMaps);
}

SimpleGameObject::SimpleGameObject(std::string meshPath, Transform * transform) {
	this->transform = transform;
	this->mesh = makeMeshes(meshPath, std::vector<Texture*>(), std::vector<Texture*>());
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

std::vector<Mesh*> SimpleGameObject::makeMeshes(std::string path, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps) {
	GLchar * tokens;
	GLchar * context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Mesh*> mesh;
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
