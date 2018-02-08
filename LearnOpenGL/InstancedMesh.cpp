#include "InstancedMesh.h"



InstancedMesh::InstancedMesh(GLchar * vertexLocation, InstancedTransformImpl * transform, Texture * diffuseMap, Texture * specularMap) {
	this->diffuseMap = diffuseMap;
	this->specularMap = specularMap;

	this->readVertexFile(vertexLocation);
	this->numInstances = transform->getModels().size();

	this->setupMesh();
}

InstancedMesh::InstancedMesh(GLchar * vertexLocation, std::vector<glm::mat4> instances, Texture * diffuseMap, Texture * specularMap) {
	this->diffuseMap = diffuseMap;
	this->specularMap = specularMap;

	this->readVertexFile(vertexLocation);
	this->numInstances = instances.size();

	this->setupMesh();
}


InstancedMesh::~InstancedMesh() {}

void InstancedMesh::Draw() {

	// Bind Diffuse Map
	if (this->diffuseMap != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->diffuseMap->getTextureID());
	}

	// Bind Specular Map
	if (this->specularMap != nullptr) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->specularMap->getTextureID());
	}

	glBindVertexArray(this->VAO);
	if (this->EBO == -1) {
		glDrawArraysInstanced(this->primitiveType, 0, this->vertices.size(), this->numInstances);
	}
	else {
		std::cout << "ERROR: DRAWING in InstancedMesh" << std::endl;
	}
	glBindVertexArray(0);
}

void InstancedMesh::setInstances(std::vector<glm::mat4> instances) {
	this->numInstances = instances.size();
}
