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
	std::vector<Texture> textures;
	textures.push_back(*(this->diffuseMap));
	textures.push_back(*(this->specularMap));
	this->material = new Material(materialLoc, *(this->shader));
	this->transform = new InstancedTransformImpl(transformLoc, this);
	this->mesh = new Mesh(meshLoc, textures, *(this->shader), this->transform->getModels(), INSTANCED_SHADER);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

void InstancedGameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->Draw();

	// TODO: Don't make every GameObject reload the Light Data to the Shader
	/* I don't think this should be called by every sort of instance of GameObject */
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