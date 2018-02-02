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


SimpleInstancedArrayGameObject::~SimpleInstancedArrayGameObject() {}
