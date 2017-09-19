#include "InstancedArrayGameObjectImpl.h"
#include "InstancedArrayTransformImpl.h"

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl() {}

InstancedArrayGameObjectImpl::~InstancedArrayGameObjectImpl() {}

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * specularMapLoc, GLchar * meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	Texture * diffuseMap;
	Texture * specularMap;

	this->shader = new Shader(vertexShader, fragmentShader);
	diffuseMap = new Texture(diffuseMapLoc, true);
	diffuseMap->name = "material.diffuse";
	specularMap = new Texture(specularMapLoc, true);
	specularMap->name = "material.specular";
	this->material = new Material(materialLoc);
	this->transform = new InstancedArrayTransformImpl(transformLoc, this);
	this->mesh.push_back(new Mesh(meshLoc, this->transform->getModels(), INSTANCED_ARRAY_SHADER, diffuseMap, specularMap));
	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc1, GLchar * specularMapLoc1, GLchar * diffuseMapLoc2, GLchar * specularMapLoc2, GLchar * diffuseMapLoc3, GLchar * specularMapLoc3, GLchar * meshLoc1, GLchar * meshLoc2, GLchar * meshLoc3, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	Texture * diffuseMap;
	Texture * specularMap;
	this->shader = new Shader(vertexShader, fragmentShader);

	diffuseMap = new Texture(diffuseMapLoc1, true);
	diffuseMap->name = "material.diffuse";
	specularMap = new Texture(specularMapLoc1, true);
	specularMap->name = "material.specular";
	this->material = new Material(materialLoc);
	this->transform = new InstancedArrayTransformImpl(transformLoc, this);
	this->mesh.push_back(new Mesh(meshLoc1, this->transform->getModels(), INSTANCED_ARRAY_SHADER, diffuseMap, specularMap));

	diffuseMap = new Texture(diffuseMapLoc2, true);
	diffuseMap->name = "material.diffuse";
	specularMap = new Texture(specularMapLoc2, true);
	specularMap->name = "material.specular";
	this->material = new Material(materialLoc);
	this->transform = new InstancedArrayTransformImpl(transformLoc, this);
	this->mesh.push_back(new Mesh(meshLoc2, this->transform->getModels(), INSTANCED_ARRAY_SHADER, diffuseMap, specularMap));

	diffuseMap = new Texture(diffuseMapLoc3, true);
	diffuseMap->name = "material.diffuse";
	specularMap = new Texture(specularMapLoc3, true);
	specularMap->name = "material.specular";
	this->material = new Material(materialLoc);
	this->transform = new InstancedArrayTransformImpl(transformLoc, this);
	this->mesh.push_back(new Mesh(meshLoc3, this->transform->getModels(), INSTANCED_ARRAY_SHADER, diffuseMap, specularMap));

	this->camera = camera;
	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

void InstancedArrayGameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->Draw();

	if (this->lightsContainer != nullptr) {
		this->shader->sendToShader(lightsContainer->getDirLight(), lightsContainer->getSpotLight(), lightsContainer->getPointLights());
	}
	this->shader->sendToShader(this);
	this->shader->sendToShader(this->material);

	/* With a std::vector<Mesh*> we might be losing a reference to 
	 * We may need to have a std::vector<Mesh**> 
	*/
	for (GLint i = 0; i < this->mesh.size(); i++) {
		// Bind Diffuse Map
		this->shader->sendToShader(this->mesh[i]);
		this->mesh[i]->Draw();
	}
	// this->mesh->Draw();
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