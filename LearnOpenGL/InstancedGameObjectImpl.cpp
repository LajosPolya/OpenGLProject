#include "InstancedGameObjectImpl.h"



InstancedGameObjectImpl::InstancedGameObjectImpl()
{
}


InstancedGameObjectImpl::~InstancedGameObjectImpl()
{
}


InstancedGameObjectImpl::InstancedGameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* specularMapLoc, GLchar* meshLoc, GLchar* materialLoc, GLchar* transformLoc, GLchar* lightsLoc, Camera * camera, glm::mat4 projection) {
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->diffuseMap->name = "material.diffuse";
	this->specularMap = new Texture(specularMapLoc, true);
	this->specularMap->name = "material.specular";
	std::vector<Texture> textures;
	textures.push_back(*(this->diffuseMap));
	textures.push_back(*(this->specularMap));
	this->mesh = new Mesh(meshLoc, textures, *(this->shader));
	this->material = new Material(materialLoc, *(this->shader));
	this->transform = new TransformImpl(transformLoc, this);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

InstancedGameObjectImpl::InstancedGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->specularMap = new Texture(NULL, true);
	std::vector<Texture> textures;
	textures.push_back(*(this->diffuseMap));
	textures.push_back(*(this->specularMap));
	this->mesh = new Mesh(meshLoc, textures, *(this->shader));
	this->material = new Material(NULL, *(this->shader));
	this->transform = new TransformImpl(transformLoc, this);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(NULL);

	this->shader->setProjectionMatrix(projection);
}

InstancedGameObjectImpl::InstancedGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(NULL, true);
	this->specularMap = new Texture(NULL, true);
	std::vector<Texture> textures;
	textures.push_back(*(this->diffuseMap));
	textures.push_back(*(this->specularMap));
	this->mesh = new Mesh(meshLoc, textures, *(this->shader));
	this->material = new Material(NULL, *(this->shader));
	this->transform = new TransformImpl(transformLoc, this);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(NULL);

	this->shader->setProjectionMatrix(projection);
}

InstancedGameObjectImpl::InstancedGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * specularMapLoc, GLchar * meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection, void * ptr)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->diffuseMap->name = "material.diffuse";
	this->specularMap = new Texture(specularMapLoc, true);
	this->specularMap->name = "material.specular";
	std::vector<Texture> textures;
	textures.push_back(*(this->diffuseMap));
	textures.push_back(*(this->specularMap));
	this->material = new Material(materialLoc, *(this->shader));
	this->transform = new InstancedTransform(transformLoc, this);
	this->mesh = new Mesh(meshLoc, textures, *(this->shader), this->transform->getModels());
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

void InstancedGameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->setRotation();
	this->transform->setPosition();
	this->transform->setScale();
	this->transform->Draw();

	this->shader->sendToShader(lightsContainer->getDirLight(), lightsContainer->getSpotLight(), lightsContainer->getPointLights());
	this->shader->sendToShader(this);
	this->mesh->Draw(*(this->shader));
}

Shader * InstancedGameObjectImpl::getShader() {
	return this->shader;
}

Camera * InstancedGameObjectImpl::getCamera() {
	return this->camera;
}

Transform * InstancedGameObjectImpl::getTransform() {
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