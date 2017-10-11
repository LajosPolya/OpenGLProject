#include "InstancedGameObjectImpl.h"

InstancedGameObjectImpl::InstancedGameObjectImpl() {}

InstancedGameObjectImpl::~InstancedGameObjectImpl() {
	for (GLuint i = 0; i < this->mesh.size(); i++) {
		if (this->mesh[i] != nullptr && GameObjectMemoryManager::decrement(this->material) == 0) {
			delete this->mesh[i];
		}
	}
	if (this->material != nullptr && GameObjectMemoryManager::decrement(this->material) == 0) {
		delete this->material;
	}
	if (this->shader != nullptr && GameObjectMemoryManager::decrement(this->shader) == 0) {
		delete this->shader;
	}
	if (this->camera != nullptr && GameObjectMemoryManager::decrement(this->camera) == 0) {
		delete this->camera;
	}
	if (this->transform != nullptr && GameObjectMemoryManager::decrement(this->transform) == 0) {
		delete this->transform;
	}
	if (this->lightsContainer != nullptr && GameObjectMemoryManager::decrement(this->lightsContainer) == 0) {
		delete this->lightsContainer;
	}
}

InstancedGameObjectImpl::InstancedGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc, std::string specularMapLoc, std::string meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapLoc);

	this->transform = new InstancedTransformImpl(transformLoc, this);
	this->mesh = GameObjectUtils::getMeshes(meshLoc, this->transform, diffuseMaps, specularMaps);

	this->material = new Material(materialLoc);

	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);

	GameObjectMemoryManager::add(this->shader);
	GameObjectMemoryManager::add(this->transform);
	for (GLuint i = 0; i < this->mesh.size(); i++) {
		GameObjectMemoryManager::add(this->mesh[i]);
	}
	GameObjectMemoryManager::add(this->material);
	GameObjectMemoryManager::add(this->camera, false);
	GameObjectMemoryManager::add(this->lightsContainer);
}

void InstancedGameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->Draw();

	if (this->lightsContainer != nullptr) {
		this->shader->sendToShader(lightsContainer->getDirLight(), lightsContainer->getSpotLight(), lightsContainer->getPointLights());
	}
	// TODO: Don't do this every frame, only do this if one of those vars have changed
	this->shader->sendToShader(this);
	this->shader->sendToShader(this->material);
	
	for (GLuint i = 0; i < this->mesh.size(); i++) {
		// Bind Diffuse Map
		this->shader->sendToShader(this->mesh[i]);
		this->mesh[i]->Draw();
	}
}

Shader * InstancedGameObjectImpl::getShader() {
	return this->shader;
}

Camera * InstancedGameObjectImpl::getCamera() {
	return this->camera;
}

InstancedTransformImpl * InstancedGameObjectImpl::getTransform() {
	return this->transform;
}

Texture * InstancedGameObjectImpl::getDiffuseMap(GLint i)
{
	return this->mesh[i]->getDiffuseMap();
}

Texture * InstancedGameObjectImpl::getSpecularMap(GLint i)
{
	return this->mesh[i]->getSpecularMap();
}

LightsContainer * InstancedGameObjectImpl::getLightsContainer()
{
	return this->lightsContainer;
}