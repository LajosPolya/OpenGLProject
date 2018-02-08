#include "SimpleInstancedGameObject.h"



SimpleInstancedGameObject::SimpleInstancedGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string materialPath, std::string transformPath) {
	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapPath);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapPath);

	this->transform = new InstancedTransformImpl(transformPath);
	this->mesh = GameObjectUtils::getMeshes(meshPath, this->transform, diffuseMaps, specularMaps);

	this->material = GameObjectUtils::getMaterial(materialPath);
}
SimpleInstancedGameObject::~SimpleInstancedGameObject() {}

Material * SimpleInstancedGameObject::getMaterial() {
	return this->material;
}

InstancedTransformImpl * SimpleInstancedGameObject::getTransform() {
	return this->transform;
}

void SimpleInstancedGameObject::Draw() {
	for (GLuint i = 0; i < this->mesh.size(); i++) {
		this->mesh[i]->Draw();
	}
}

void SimpleInstancedGameObject::setInstances(InstancedTransformImpl * transform) {
	this->transform->setModels(transform->getModels());
	this->transform->setPositions(*transform->getPositions());
	this->transform->setRotations(*transform->getRotations());
	this->transform->setScales(*transform->getScales());

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		this->mesh[i]->setInstances(transform->getModels());
	}

	this->instancesUpdated++;
}

GLuint SimpleInstancedGameObject::hasInstancesChangedAndSetFalse() {
	GLuint temp = this->instancesUpdated;
	this->instancesUpdated = 0;
	
	return temp;
}
