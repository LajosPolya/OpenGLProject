#include "SimpleGameObject.h"



SimpleGameObject::SimpleGameObject(std::string meshPath, std::string transformPath) : SimpleGameObject("", "", meshPath, "", transformPath)
{}

SimpleGameObject::SimpleGameObject(std::string diffuseMapPath, std::string meshPath, std::string transformPath) : SimpleGameObject(diffuseMapPath, "", meshPath, "", transformPath)
{}
SimpleGameObject::SimpleGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string materialPath, std::string transformPath) {
	this->transform = new TransformImpl(transformPath);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapPath);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapPath);
	this->mesh = GameObjectUtils::getMeshes(meshPath, this->transform, diffuseMaps, specularMaps);

	this->material = GameObjectUtils::getMaterial(materialPath);
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

Material * SimpleGameObject::getMaterial() {
	return this->material;
}

Transform * SimpleGameObject::getTransform() {
	return this->transform;
}
