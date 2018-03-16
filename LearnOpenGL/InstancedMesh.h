#pragma once

#include "Mesh.h"


class InstancedMesh : private Mesh
{
public:
	InstancedMesh(const InstancedMesh & instancedMesh);
	InstancedMesh(GLchar * vertexLocation, GLuint numInstances, Texture * diffuseMap, Texture * specularMap);
	~InstancedMesh();

	virtual void Draw() override;

	void setInstances(std::vector<glm::mat4> instances);

private:
	GLuint instanceVBO = (GLuint)-1;
	GLuint numInstances = (GLuint)0;
};

