#include "ResourceManager.h"


unordered_map<string, void *> ResourceManager::manager;

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void ResourceManager::addInstance(string path, void * ptr) {
	manager[path] = ptr;
}

Material * ResourceManager::getMaterial(string path) {
	auto it = manager.find(path);

	if (it == manager.end()) {
		return nullptr;
	}
	else {
		return (Material*)new Material(((Material*)it->second)->getMaterialProp());
	}
	
}

Texture * ResourceManager::getTexture(string path) {
	auto it = manager.find(path);

	if (it == manager.end()) {
		return nullptr;
	}
	else {
		Texture * tempText = ((Texture *)it->second);
		return (Texture*)new Texture(tempText->getTextureID(), tempText->getName(), tempText->getType());
	}
}

// TODO: GameOBject is no longer needed here
TransformImpl * ResourceManager::getTransform(string path, GameObject * gameObject) {
	auto it = manager.find(path);

	if (it == manager.end()) {
		return nullptr;
	}
	else {
		TransformImpl * tempTransform = ((TransformImpl *)it->second);
		return (TransformImpl*)new TransformImpl(tempTransform->getPosition(), tempTransform->getRotation(), tempTransform->getScale());
	}
}
