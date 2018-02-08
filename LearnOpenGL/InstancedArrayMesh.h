#pragma once

#include "Mesh.h"

class InstancedArrayMesh : private Mesh
{
public:
	InstancedArrayMesh();
	~InstancedArrayMesh();
	InstancedArrayMesh(const InstancedArrayMesh & toCopy);

	InstancedArrayMesh(GLchar * vertexLocation, InstancedArrayTransformImpl * transform, Texture * diffuseMap, Texture * specularMap);
	InstancedArrayMesh(GLchar * vertexLocation, InstancedArrayTransformImpl * transform, GLuint primitiveType, Texture * diffuseMap, Texture * specularMap);

	virtual void Draw() override;

	void setInstances(std::vector<glm::mat4> instances);

private:
	virtual void setupMesh() override;
};

