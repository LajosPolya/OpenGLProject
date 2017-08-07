#include "GameObjectImpl.h"
#include "TransformImpl.h"

GameObjectImpl::GameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* specularMapLoc, GLchar* meshLoc, GLchar* materialLoc, GLchar* transformLoc, GLchar* lightsLoc,  Camera * camera, glm::mat4 projection) {
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->diffuseMap->name = "material.diffuse";
	this->specularMap = new Texture(specularMapLoc, true);
	this->specularMap->name = "material.specular";
	this->mesh = new Mesh(meshLoc);
	this->material = new Material(materialLoc);
	this->transform = new TransformImpl(transformLoc, this);
	this->camera = camera;
	this->projection = projection;
	
	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

GameObjectImpl::GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->specularMap = new Texture(nullptr, true);
	this->mesh = new Mesh(meshLoc);
	this->material = new Material(nullptr);
	this->transform = new TransformImpl(transformLoc, this);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(nullptr);

	this->shader->setProjectionMatrix(projection);
}

GameObjectImpl::GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(nullptr, true);
	this->specularMap = new Texture(nullptr, true);
	this->mesh = new Mesh(meshLoc);
	this->material = new Material(nullptr);
	this->transform = new TransformImpl(transformLoc, this);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(nullptr);

	this->shader->setProjectionMatrix(projection);
}

void GameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->setRotation();
	this->transform->setPosition();
	this->transform->setScale();
	this->transform->Draw();

	this->shader->sendToShader(lightsContainer->getDirLight(), lightsContainer->getSpotLight(), lightsContainer->getPointLights());
	this->shader->sendToShader(this);
	this->shader->sendToShader(this->material);
	this->mesh->Draw();
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

Texture * GameObjectImpl::getDiffuseMap()
{
	return this->diffuseMap;
}

Texture * GameObjectImpl::getSpecularMap()
{
	return this->specularMap;
}

LightsContainer * GameObjectImpl::getLightsContainer()
{
	return this->lightsContainer;
}
