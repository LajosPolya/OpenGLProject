#include "InstancedMesh.h"



InstancedMesh::InstancedMesh(const InstancedMesh & instancedMesh) : Mesh(instancedMesh) {
	this->instanceVBO = instancedMesh.instanceVBO;
	this->numInstances = instancedMesh.numInstances;
}

InstancedMesh::InstancedMesh(GLchar * vertexLocation, GLuint numInstances, Texture * diffuseMap, Texture * specularMap) {
	this->diffuseMap = diffuseMap;
	this->specularMap = specularMap;

	this->readVertexFile(vertexLocation);
	this->numInstances = numInstances;

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
		cout << "ERROR: DRAWING in InstancedMesh" << endl;
	}
	glBindVertexArray(0);
}

void InstancedMesh::setInstances(vector<mat4> instances) {
	this->numInstances = instances.size();
}
