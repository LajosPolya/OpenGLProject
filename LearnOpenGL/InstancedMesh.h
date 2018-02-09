#pragma once

#include "Mesh.h"


class InstancedMesh : private Mesh
{
public:
	// TODO: Create CopyConstructor
	InstancedMesh(const Mesh & InstancedMesh);
	InstancedMesh(GLchar * vertexLocation, InstancedTransformImpl * transform, Texture * diffuseMap, Texture * specularMap);
	InstancedMesh(GLchar * vertexLocation, std::vector<glm::mat4> instances, Texture * diffuseMap, Texture * specularMap);
	~InstancedMesh();

	virtual void Draw() override;

	void setInstances(std::vector<glm::mat4> instances);

private:
	GLuint instanceVBO = (GLuint)-1;
	GLuint numInstances = (GLuint)0;
};

