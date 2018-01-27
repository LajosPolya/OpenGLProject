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

// TODO: Do I need to copy LightsContainer
/* Since the members are static, should I consider redesigning LightsContainer to not be a part of the GameObject */
InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl(const InstancedArrayGameObjectImpl & toCopy) {
	//this->mesh = toCopy.mesh; // std::vector has a Copy Construct which will call Mesh's Copy Constructor
	for (GLuint i = 0; i < toCopy.mesh.size(); i++) {
		this->mesh.push_back(new Mesh(*toCopy.mesh[i]));
	}
	this->material = new Material(*toCopy.material);
	this->shader = new Shader(*toCopy.shader);
	this->camera = toCopy.camera;
	this->transform = new InstancedArrayTransformImpl(*toCopy.transform);
	this->lightsContainer = toCopy.lightsContainer;

	GameObjectMemoryManager::add(toCopy.camera, false);
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

	this->transform = new InstancedArrayTransformImpl(positions);
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

	this->transform = new InstancedArrayTransformImpl(positions);
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

	this->transform = new InstancedArrayTransformImpl(positions);
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
	// GameObject waiting to be filled
	if (transform->getModels().size() == 0) {
		return;
	}

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

std::vector<Mesh*> InstancedArrayGameObjectImpl::getMeshes()
{
	return this->mesh;
}

void InstancedArrayGameObjectImpl::setTransform(InstancedArrayTransformImpl * transform) {
	this->transform->setModels(transform->getModels());
	this->transform->setPositions(*transform->getPositions());
	this->transform->setRotations(*transform->getRotations());
	this->transform->setScales(*transform->getScales());

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		this->mesh[i]->setInstance(transform->getModels());
	}
}
