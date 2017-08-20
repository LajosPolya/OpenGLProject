#include "InstancedGameObjectImpl.h"

InstancedGameObjectImpl::InstancedGameObjectImpl() {}

InstancedGameObjectImpl::~InstancedGameObjectImpl() {}

InstancedGameObjectImpl::InstancedGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * specularMapLoc, GLchar * meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->diffuseMap->name = "material.diffuse";
	this->specularMap = new Texture(specularMapLoc, true);
	this->specularMap->name = "material.specular";
	this->material = new Material(materialLoc);
	this->transform = new InstancedTransformImpl(transformLoc, this);
	this->mesh = new Mesh(meshLoc, this->transform->getSize());
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

InstancedGameObjectImpl::InstancedGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->diffuseMap->name = "material.diffuse";
	this->specularMap = new Texture(nullptr, true);
	this->transform = new InstancedTransformImpl(transformLoc, this);
	this->mesh = new Mesh(meshLoc, this->transform->getSize());
	this->material = new Material(nullptr);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = nullptr;

	this->shader->setProjectionMatrix(projection);
}

void InstancedGameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->Draw();

	if (this->lightsContainer != nullptr) {
		this->shader->sendToShader(lightsContainer->getDirLight(), lightsContainer->getSpotLight(), lightsContainer->getPointLights());
	}
	this->shader->sendToShader(this);
	this->shader->sendToShader(this->material);
	this->mesh->Draw();
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

Texture * InstancedGameObjectImpl::getDiffuseMap()
{
	return this->diffuseMap;
}

Texture * InstancedGameObjectImpl::getSpecularMap()
{
	return this->specularMap;
}

LightsContainer * InstancedGameObjectImpl::getLightsContainer()
{
	return this->lightsContainer;
}