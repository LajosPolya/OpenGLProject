#include "ResourceManager.h"


std::unordered_map<std::string, void *> ResourceManager::manager;

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void ResourceManager::addInstance(std::string path, void * ptr)
{
	manager[path] = ptr;
}

Material * ResourceManager::getMaterial(std::string path)
{
	std::unordered_map<std::string, void *>::iterator it = manager.find(path);

	if (it == manager.end()) {
		return nullptr;
	}
	else {
		return (Material*)new Material(((Material*)it->second)->getMaterialProp());
	}
	
}

Texture * ResourceManager::getTexture(std::string path)
{
	std::unordered_map<std::string, void *>::iterator it = manager.find(path);

	if (it == manager.end()) {
		return nullptr;
	}
	else {
		Texture * tempText = ((Texture *)it->second);
		return (Texture*)new Texture(tempText->getTextureID(), tempText->getName(), tempText->getType());
	}
}

TransformImpl * ResourceManager::getTransform(std::string path, GameObject * gameObject)
{
	std::unordered_map<std::string, void *>::iterator it = manager.find(path);

	if (it == manager.end()) {
		return nullptr;
	}
	else {
		TransformImpl * tempTransform = ((TransformImpl *)it->second);
		return (TransformImpl*)new TransformImpl(tempTransform->getPosition(), tempTransform->getRotation(), tempTransform->getScale(), gameObject);
	}
}

InstancedArrayTransformImpl * ResourceManager::getTransform(std::string path, InstancedArrayGameObjectImpl * gameObject)
{
	std::unordered_map<std::string, void *>::iterator it = manager.find(path);

	if (it == manager.end()) {
		return nullptr;
	}
	else {
		InstancedArrayTransformImpl * tempTransform = ((InstancedArrayTransformImpl *)it->second);

		std::vector<glm::vec3> * positions = tempTransform->getPositions();
		std::vector<glm::vec3> * rotations = tempTransform->getRotations();
		std::vector<glm::vec3> * scales = tempTransform->getScales();
		if (positions->size() == rotations->size() && rotations->size() == scales->size()) {
			std::vector<glm::vec3> tempPositions;
			std::vector<glm::vec3> tempRotations;
			std::vector<glm::vec3> tempScales;
			for (GLuint i = 0; i < tempTransform->getPositions()->size(); i++) {
				tempPositions.push_back((*positions)[i]);
				tempRotations.push_back((*rotations)[i]);
				tempScales.push_back((*scales)[i]);
			}
			return (InstancedArrayTransformImpl*)new InstancedArrayTransformImpl(tempPositions, tempRotations, tempScales, gameObject);
		}
		else {
			std::cout << "ERROR:: RESOURCE_MANAGER_INSTNACED_ARRAY_TRANSFORM_IMPL" << std::endl;
			return nullptr;
		}
	}
}
