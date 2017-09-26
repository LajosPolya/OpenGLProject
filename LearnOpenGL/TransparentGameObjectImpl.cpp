#include "TransparentGameObjectImpl.h"

TransparentGameObjectImpl::TransparentGameObjectImpl() {}

TransparentGameObjectImpl::~TransparentGameObjectImpl() {}

TransparentGameObjectImpl::TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc, std::string specularMapLoc, std::string meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);

	this->diffuseMap = GameObjectUtils::getDiffuseTextures(diffuseMapLoc);
	this->specularMap = GameObjectUtils::getSpecularTextures(specularMapLoc);

	this->material = new Material(materialLoc);
	this->transform = new TransparentTransformImpl(transformLoc, this);
	this->mesh = GameObjectUtils::getMeshes(meshLoc, this->transform, this->diffuseMap, this->specularMap);

	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

TransparentGameObjectImpl::TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);

	this->diffuseMap = GameObjectUtils::getDiffuseTextures(diffuseMapLoc);

	this->transform = new TransparentTransformImpl(transformLoc, this);
	this->mesh = GameObjectUtils::getMeshes(meshLoc, this->transform, this->diffuseMap, this->specularMap);

	this->camera = camera;
	this->projection = projection;

	this->shader->setProjectionMatrix(projection);
}

void TransparentGameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->Draw();

	if (this->lightsContainer != nullptr) {
		this->shader->sendToShader(lightsContainer->getDirLight(), lightsContainer->getSpotLight(), lightsContainer->getPointLights());
	}
	this->shader->sendToShader(this);
	if (this->material != nullptr) {
		this->shader->sendToShader(this->material);
	}
	

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		// Bind Diffuse Map
		this->shader->sendToShader(this->mesh[i]);
		this->mesh[i]->Draw();
	}
}

Shader * TransparentGameObjectImpl::getShader() {
	return this->shader;
}

Camera * TransparentGameObjectImpl::getCamera() {
	return this->camera;
}

TransparentTransformImpl * TransparentGameObjectImpl::getTransform() {
	return this->transform;
}

Texture * TransparentGameObjectImpl::getDiffuseMap(GLint i)
{
	return this->mesh[i]->getDiffuseMap();
}

Texture * TransparentGameObjectImpl::getSpecularMap(GLint i)
{
	return this->mesh[i]->getSpecularMap();
}

LightsContainer * TransparentGameObjectImpl::getLightsContainer()
{
	return this->lightsContainer;
}
