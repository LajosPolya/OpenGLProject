#include "GameObjectImpl.h"
#include "TransformImpl.h"

GameObjectImpl::~GameObjectImpl()
{
	for (GLuint i = 0; i < this->mesh.size(); i++) {
		if (this->mesh[i] != nullptr && GameObjectMemoryManager::decrement(this->mesh[i]) == 0) {
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

GameObjectImpl::GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * specularMapLoc, std::string meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc,  Camera * camera, glm::mat4 projection) {
	this->shader = new Shader(vertexShader, fragmentShader);
	
	this->transform = new TransformImpl(transformLoc, this);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapLoc);
	this->mesh = GameObjectUtils::getMeshes(meshLoc, this->transform, diffuseMaps, specularMaps);

	this->material = GameObjectUtils::getMaterial(materialLoc);
	
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

GameObjectImpl::GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, std::string meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);

	this->transform = new TransformImpl(transformLoc, this);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc);

	this->mesh = GameObjectUtils::getMeshes(meshLoc, this->transform, diffuseMaps, {});
	this->material = nullptr;
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = nullptr;

	this->shader->setProjectionMatrix(projection);

	GameObjectMemoryManager::add(this->shader);
	GameObjectMemoryManager::add(this->transform);
	for (GLuint i = 0; i < this->mesh.size(); i++) {
		GameObjectMemoryManager::add(this->mesh[i]);
	}
	GameObjectMemoryManager::add(this->camera, false);
}

GameObjectImpl::GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->transform = new TransformImpl(transformLoc, this);
	this->mesh = GameObjectUtils::getMeshes(meshLoc, this->transform, {}, {});
	this->material = nullptr;
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = nullptr;

	this->shader->setProjectionMatrix(projection);

	GameObjectMemoryManager::add(this->shader);
	GameObjectMemoryManager::add(this->transform);
	for (GLuint i = 0; i < this->mesh.size(); i++) {
		GameObjectMemoryManager::add(this->mesh[i]);
	}
	GameObjectMemoryManager::add(this->camera, false);
}

void GameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->setRotation();
	this->transform->setPosition();
	this->transform->setScale();
	this->transform->Draw();

	if (this->lightsContainer != nullptr) {
		this->shader->sendToShader(this->lightsContainer->getDirLight(), this->lightsContainer->getSpotLight(), this->lightsContainer->getPointLights());
	}
	this->shader->sendToShader(this);
	this->shader->sendToShader(this->material);

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		// Bind Diffuse Map
		this->shader->sendToShader(this->mesh[i]);
		this->mesh[i]->Draw();
	}
}

Shader * GameObjectImpl::getShader() {
	return this->shader;
}

Camera * GameObjectImpl::getCamera() {
	return this->camera;
}

Transform * GameObjectImpl::getTransform() {
	return this->transform;
}

Texture * GameObjectImpl::getDiffuseMap(GLuint i)
{
	return this->mesh[i]->getDiffuseMap();
}

Texture * GameObjectImpl::getSpecularMap(GLuint i)
{
	return this->mesh[i]->getSpecularMap();
}

LightsContainer * GameObjectImpl::getLightsContainer()
{
	return this->lightsContainer;
}
