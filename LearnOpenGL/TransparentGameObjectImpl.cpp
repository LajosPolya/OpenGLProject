#include "TransparentGameObjectImpl.h"

TransparentGameObjectImpl::TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc, std::string specularMapLoc, std::string meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, const Camera & camera, glm::mat4 projection) : camera(camera) {
	this->shader = new Shader(vertexShader, fragmentShader);

	std::vector<Texture*> diffuseMap = GameObjectUtils::getDiffuseTextures(diffuseMapLoc);
	std::vector<Texture*> specularMap = GameObjectUtils::getSpecularTextures(specularMapLoc);

	this->material = new Material(materialLoc);
	this->transform = new TransparentTransformImpl(transformLoc);
	// TODO: Meshes need to be sorted every frame, not just TransparentTransforms
	// unless I want to make every transparent GameObject its own GameObject
	this->mesh = makeMeshes(meshLoc, this->transform->getModels().size(), diffuseMap, specularMap);

	this->projection = projection;

	this->lightsContainer = new LightsContainer(lightsLoc);

	this->shader->setProjectionMatrix(projection);
}

TransparentGameObjectImpl::TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * meshLoc, GLchar * transformLoc, const Camera & camera, glm::mat4 projection) : camera(camera) {
	this->shader = new Shader(vertexShader, fragmentShader);

	std::vector<Texture*> diffuseMap = GameObjectUtils::getDiffuseTextures(diffuseMapLoc);

	this->transform = new TransparentTransformImpl(transformLoc);
	this->mesh = makeMeshes(meshLoc, this->transform->getModels().size(), diffuseMap, {});

	this->projection = projection;

	this->shader->setProjectionMatrix(projection);
}

void TransparentGameObjectImpl::Draw() {

	this->transform->refreshModel();
	this->transform->Draw();

	if (this->lightsContainer != nullptr) {
		this->shader->sendToShader(lightsContainer);
	}
	this->shader->sendToShader(this);
	if (this->material != nullptr) {
		this->shader->sendToShader(this->material);
	}
	

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		this->mesh[i]->Draw();
	}
}

Shader * TransparentGameObjectImpl::getShader() {
	return this->shader;
}

Camera TransparentGameObjectImpl::getCamera() {
	return this->camera;
}

TransparentTransformImpl * TransparentGameObjectImpl::getTransform() {
	return this->transform;
}

LightsContainer * TransparentGameObjectImpl::getLightsContainer()
{
	return this->lightsContainer;
}

std::vector<InstancedMesh*> TransparentGameObjectImpl::makeMeshes(std::string path, GLuint numInstances, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps) {
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<InstancedMesh*> mesh;
	GLuint i = 0;
	Texture * tempDif;
	Texture * tempSpec;
	while (tokens != NULL) {
		tempDif = nullptr;
		if (i < diffuseMaps.size()) {
			tempDif = diffuseMaps[i];
		}
		tempSpec = nullptr;
		if (i < specularMaps.size()) {
			tempSpec = specularMaps[i];
		}
		mesh.push_back(new InstancedMesh(tokens, numInstances, tempDif, tempSpec));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}
