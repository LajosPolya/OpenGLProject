#include "SimpleInstancedGameObject.h"



SimpleInstancedGameObject::SimpleInstancedGameObject(string diffuseMapPath, string specularMapPath, string meshPath, string transformPath) {
	vector<Texture*> diffuseMaps = GameObjectUtils::getDiffuseTextures(diffuseMapPath);
	vector<Texture*> specularMaps = GameObjectUtils::getSpecularTextures(specularMapPath);

	this->transform = new InstancedTransformImpl(transformPath);
	this->mesh = makeMeshes(meshPath, this->transform->getModels().size(), diffuseMaps, specularMaps);
}

SimpleInstancedGameObject::SimpleInstancedGameObject(string meshPath, InstancedTransformImpl * transform) {
	this->transform = transform;
	this->mesh = makeMeshes(meshPath, this->transform->getModels().size(), vector<Texture*>(), vector<Texture*>());
}

SimpleInstancedGameObject::~SimpleInstancedGameObject() {}

InstancedTransformImpl * SimpleInstancedGameObject::getTransform() {
	return this->transform;
}

void SimpleInstancedGameObject::Draw() {
	for (GLuint i = 0; i < this->mesh.size(); i++) {
		this->mesh[i]->Draw();
	}
}

void SimpleInstancedGameObject::setInstances(InstancedTransformImpl * transform) {
	this->transform->setModels(transform->getModels());
	this->transform->setPositions(*transform->getPositions());
	this->transform->setRotations(*transform->getRotations());
	this->transform->setScales(*transform->getScales());

	for (GLuint i = 0; i < this->mesh.size(); i++) {
		this->mesh[i]->setInstances(transform->getModels());
	}

	this->instancesUpdated++;
}

GLuint SimpleInstancedGameObject::hasInstancesChangedAndSetFalse() {
	GLuint temp = this->instancesUpdated;
	this->instancesUpdated = 0;
	
	return temp;
}

vector<InstancedMesh*> SimpleInstancedGameObject::makeMeshes(string path, GLuint numInstances, vector<Texture*> diffuseMaps, vector<Texture*> specularMaps) {
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	vector<InstancedMesh*> mesh;
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
