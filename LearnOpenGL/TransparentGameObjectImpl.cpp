#include "TransparentGameObjectImpl.h"

TransparentGameObjectImpl::TransparentGameObjectImpl() {}

TransparentGameObjectImpl::~TransparentGameObjectImpl() {}

TransparentGameObjectImpl::TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * specularMapLoc, GLchar * meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->diffuseMap->name = "material.diffuse";
	this->specularMap = new Texture(specularMapLoc, true);
	this->specularMap->name = "material.specular";
	this->material = new Material(materialLoc);
	this->transform = new TransparentTransformImpl(transformLoc, this);
	// TODO: Does this need to send its model to Mesh?
	this->mesh = new Mesh(meshLoc, this->transform->getModels(), INSTANCED_SHADER);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

TransparentGameObjectImpl::TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->diffuseMap->name = "material.diffuse";
	this->specularMap = new Texture(nullptr, true);
	this->transform = new TransparentTransformImpl(transformLoc, this);
	this->mesh = new Mesh(meshLoc, this->transform->getSize());
	this->material = new Material(nullptr);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(nullptr);

	this->shader->setProjectionMatrix(projection);
}

void TransparentGameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->Draw();

	// TODO: Don't make every GameObject reload the Light Data to the Shader
	/* I don't think this should be called by every sort of instance of GameObject */
	this->shader->sendToShader(lightsContainer->getDirLight(), lightsContainer->getSpotLight(), lightsContainer->getPointLights());
	this->shader->sendToShader(this);
	this->shader->sendToShader(this->material);
	this->mesh->Draw();
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

Texture * TransparentGameObjectImpl::getDiffuseMap()
{
	return this->diffuseMap;
}

Texture * TransparentGameObjectImpl::getSpecularMap()
{
	return this->specularMap;
}

LightsContainer * TransparentGameObjectImpl::getLightsContainer()
{
	return this->lightsContainer;
}
