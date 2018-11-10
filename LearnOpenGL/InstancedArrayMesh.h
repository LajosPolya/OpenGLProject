#pragma once

#include "Mesh.h"

using namespace glm;

class InstancedArrayMesh : private Mesh
{
public:
	InstancedArrayMesh(const InstancedArrayMesh & toCopy);
	InstancedArrayMesh();
	~InstancedArrayMesh();

	InstancedArrayMesh(GLchar * vertexLocation, vector<mat4> instances, Texture * diffuseMap, Texture * specularMap);
	InstancedArrayMesh(GLchar * vertexLocation, vector<mat4>, GLuint primitiveType, Texture * diffuseMap, Texture * specularMap);

	virtual void Draw() override;

	void setInstances(vector<mat4> instances);

private:
	GLuint instanceVBO = (GLuint)-1;

	vector<mat4> instances;
	GLuint numInstances = (GLuint)0;

	virtual void setupMesh() override;
};

