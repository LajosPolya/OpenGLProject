#include "InstancedArrayMesh.h"



// Also call bass class' Copy Constructor
InstancedArrayMesh::InstancedArrayMesh(const InstancedArrayMesh & toCopy) : Mesh(toCopy) {
	this->instanceVBO = toCopy.instanceVBO;
	this->instances = toCopy.instances;
	this->numInstances = toCopy.numInstances;
}

InstancedArrayMesh::InstancedArrayMesh() {}
InstancedArrayMesh::~InstancedArrayMesh() {}

InstancedArrayMesh::InstancedArrayMesh(GLchar * vertexLocation, std::vector<glm::mat4> instances, Texture * diffuseMap, Texture * specularMap) {
	this->diffuseMap = diffuseMap;
	this->specularMap = specularMap;

	this->readVertexFile(vertexLocation);
	this->instances = instances;
	this->numInstances = this->instances.size();

	this->setupMesh();
}

InstancedArrayMesh::InstancedArrayMesh(GLchar * vertexLocation, std::vector<glm::mat4> instances, GLuint primitiveType, Texture * diffuseMap, Texture * specularMap) : InstancedArrayMesh(vertexLocation, instances, diffuseMap, specularMap) {
	this->primitiveType = primitiveType;
}

void InstancedArrayMesh::Draw() {

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

	// Draw Mesh
	glBindVertexArray(this->VAO);
	if (this->EBO == -1) {
		glDrawArraysInstanced(this->primitiveType, 0, this->vertices.size(), this->numInstances);
	}
	glBindVertexArray(0);
}

void InstancedArrayMesh::setInstances(std::vector<glm::mat4> instances) {
	/* VAO does't need to be bound since VBO is bound to a VAO Vertex Attribute Array */
	//glBindVertexArray(perlin3d.getMeshes()[i]->getVAO());
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
	if (instances.size() > 0) {
		if (instances.size() <= this->numInstances) {
			this->instances = instances;
			this->numInstances = this->instances.size();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * numInstances, &this->instances[0]);
		}
		else {
			this->instances = instances;
			this->numInstances = this->instances.size();
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->numInstances, &this->instances[0], GL_DYNAMIC_DRAW);
		}
	}
}

void InstancedArrayMesh::setupMesh() {
	Mesh::setupMesh();
	// VAO created in Mesh::setupMesh()
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
	if (this->instances.size() > 0) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->instances.size(), &this->instances[0], GL_DYNAMIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
