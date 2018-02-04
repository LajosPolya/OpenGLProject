#include "SimpleInstancedArrayGameObject.h"



SimpleInstancedArrayGameObject::SimpleInstancedArrayGameObject() {}

SimpleInstancedArrayGameObject::SimpleInstancedArrayGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, GLchar * materialPath, std::vector<glm::vec3> positions) {
	// TODO: Textures should be created where the Mesh is created
	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapPath);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapPath);

	this->transform = transform = new InstancedArrayTransformImpl(positions);
	this->mesh = GameObjectUtils::getMeshes(meshPath, this->transform, diffuseMaps, specularMaps);

	this->material = GameObjectUtils::getMaterial(materialPath);
}

SimpleInstancedArrayGameObject::SimpleInstancedArrayGameObject(const SimpleInstancedArrayGameObject & toCopy) {

	this->transform = new InstancedArrayTransformImpl(*toCopy.transform);
	for (GLuint i = 0; i < toCopy.mesh.size(); i++) {
		this->mesh.push_back(new Mesh(*toCopy.mesh[i]));
	}
	this->material = new Material(*toCopy.material);
}


SimpleInstancedArrayGameObject::~SimpleInstancedArrayGameObject() {}

void SimpleInstancedArrayGameObject::Draw() {
	// GameObject waiting to be filled if it has no models
	if (this->transform->getModels().size() == 0) {
		return;
	}

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		this->mesh[i]->Draw();
	}
}

InstancedArrayTransformImpl * SimpleInstancedArrayGameObject::getTransform() {
	return this->transform;
}

Material * SimpleInstancedArrayGameObject::getMaterial() {
	return this->material;
}

void SimpleInstancedArrayGameObject::setInstances(InstancedArrayTransformImpl * transform) {
	this->transform->setModels(transform->getModels());
	this->transform->setPositions(*(transform->getPositions()));
	this->transform->setRotations(*(transform->getRotations()));
	this->transform->setScales(*(transform->getScales()));

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		this->mesh[i]->setInstance(transform->getModels());
	}
}
