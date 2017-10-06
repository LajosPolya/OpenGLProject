#include "GameObjectImpl.h"
#include "TransformImpl.h"

GameObjectImpl::GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * specularMapLoc, std::string meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc,  Camera * camera, glm::mat4 projection) {
	this->shader = new Shader(vertexShader, fragmentShader);
	
	this->transform = new TransformImpl(transformLoc, this);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc);
	std::vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapLoc);
	this->mesh = GameObjectUtils::getMeshes(meshLoc, this->transform, diffuseMaps, specularMaps);
	this->material = new Material(materialLoc);
	
	this->camera = camera;
	this->projection = projection;
	
	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

GameObjectImpl::GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, std::string meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);

	this->transform = new TransformImpl(transformLoc, this);

	std::vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapLoc);

	this->mesh = GameObjectUtils::getMeshes(meshLoc, this->transform, diffuseMaps, {});
	this->material = new Material(nullptr);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(nullptr);

	this->shader->setProjectionMatrix(projection);
}

GameObjectImpl::GameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection)
{
	this->shader = new Shader(vertexShader, fragmentShader);
	this->mesh.push_back(new Mesh(meshLoc));
	this->material = new Material(nullptr);
	this->transform = new TransformImpl(transformLoc, this);
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(nullptr);

	this->shader->setProjectionMatrix(projection);
}

GameObjectImpl::~GameObjectImpl()
{
	/*std::vector<Mesh*> mesh;
	Material * material = nullptr;
	Shader * shader = nullptr;
	Camera * camera = nullptr;
	Transform * transform = nullptr;
	LightsContainer * lightsContainer = nullptr;*/
	for (GLuint i = 0; i < this->mesh.size(); i++) {
		delete mesh[i];
	}
	if (this->material != nullptr) {
		delete this->material;
	}
	if (this->shader != nullptr) {
		delete this->shader;
	}
	if (this->camera != nullptr) {
		// delete this->camera;
	}
	if (this->transform != nullptr) {
		delete this->transform;
	}
	if (this->lightsContainer != nullptr) {
		delete this->lightsContainer;
	}
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
