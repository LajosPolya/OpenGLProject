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

InstancedArrayGameObjectImpl::InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc1, std::string specularMapLoc1, GLchar * diffuseMapLoc2, GLchar * specularMapLoc2, GLchar * diffuseMapLoc3, GLchar * specularMapLoc3, std::string meshLoc1, GLchar * meshLoc2, GLchar * meshLoc3, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection)
{
	// TODO: Send paths delimited by commas and parse them in hear instead of having a param for each map, mesh...
	Texture * diffuseMap;
	Texture * specularMap;
	this->shader = new Shader(vertexShader, fragmentShader);

	GLchar * tokens;
	GLchar* context = NULL;

	
	tokens = strtok_s(&diffuseMapLoc1[0], ",", &context);
	std::vector<Texture*> diffuseMaps;
	while (tokens != NULL) {
		diffuseMap = new Texture(tokens, true);
		diffuseMap->name = "material.diffuse";
		diffuseMaps.push_back(diffuseMap);
		tokens = strtok_s(NULL, ",", &context);
	}

	tokens = strtok_s(&specularMapLoc1[0], ",", &context);
	std::vector<Texture*> specularMaps;
	while (tokens != NULL) {
		specularMap = new Texture(tokens, true);
		specularMap->name = "material.specular";
		specularMaps.push_back(specularMap);
		tokens = strtok_s(NULL, ",", &context);
	}

	this->transform = new InstancedArrayTransformImpl(transformLoc, this);

	tokens = strtok_s(&meshLoc1[0], ",", &context);
	GLint i = 0;
	while (tokens != NULL) {
		this->mesh.push_back(new Mesh(tokens, this->transform->getModels(), INSTANCED_ARRAY_SHADER, diffuseMaps[i], specularMaps[i]));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}

	this->material = new Material(materialLoc);

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
	for (GLuint i = 0; i < this->mesh.size(); i++) {
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