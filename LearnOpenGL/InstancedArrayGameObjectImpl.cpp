#include "InstancedArrayGameObjectImpl.h"
#include "InstancedArrayTransformImpl.h"

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl() {}

InstancedArrayGameObjectImpl::~InstancedArrayGameObjectImpl() {
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

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	
	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc1);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapLoc1);

	this->transform = GameObjectUtils::getTransform(transformLoc, this);
	this->mesh = GameObjectUtils::getMeshes(meshLoc1, this->transform, diffuseMaps, specularMaps);

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

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * geometryShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader, geometryShader);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc1);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapLoc1);

	this->transform = GameObjectUtils::getTransform(transformLoc, this);
	this->mesh = GameObjectUtils::getMeshes(meshLoc1, this->transform, diffuseMaps, specularMaps);

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

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, std::vector<glm::vec3>& positions, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc1);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapLoc1);

	this->transform = new InstancedArrayTransformImpl(positions, this);
	this->mesh = GameObjectUtils::getMeshes(meshLoc1, this->transform, diffuseMaps, specularMaps);

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

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * geometryShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, std::vector<glm::vec3>& positions, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader, geometryShader);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc1);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapLoc1);

	this->transform = new InstancedArrayTransformImpl(positions, this);
	this->mesh = GameObjectUtils::getMeshes(meshLoc1, this->transform, diffuseMaps, specularMaps);

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

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * geometryShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, std::vector<glm::vec3>& positions, GLchar * lightsLoc, Camera * camera, glm::mat4 projection, GLuint primitiveType)
{
	this->shader = new Shader(vertexShader, fragmentShader, geometryShader);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc1);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapLoc1);

	this->transform = new InstancedArrayTransformImpl(positions, this);
	this->mesh = GameObjectUtils::getMeshes(meshLoc1, this->transform, diffuseMaps, specularMaps, primitiveType);

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

void InstancedArrayGameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->Draw();

	if (this->lightsContainer != nullptr) {
		this->shader->sendToShader(lightsContainer);
	}
	this->shader->sendToShader(this);
	this->shader->sendToShader(this->material);

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		// Bind Diffuse Map
		this->shader->sendToShader(this->mesh[i]);
		this->mesh[i]->Draw();
	}
}

Shader * InstancedArrayGameObjectImpl::getShader() {
	return this->shader;
}

Camera * InstancedArrayGameObjectImpl::getCamera() {
	return this->camera;
}

InstancedArrayTransformImpl * InstancedArrayGameObjectImpl::getTransform() {
	return this->transform;
}

Texture * InstancedArrayGameObjectImpl::getDiffuseMap(GLint i)
{
	return this->mesh[i]->getDiffuseMap();
}

Texture * InstancedArrayGameObjectImpl::getSpecularMap(GLint i)
{
	return this->mesh[i]->getSpecularMap();
}

LightsContainer * InstancedArrayGameObjectImpl::getLightsContainer()
{
	return this->lightsContainer;
}