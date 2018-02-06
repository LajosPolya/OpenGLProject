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

	virtual void setInstance(std::vector<glm::mat4> instances) override;

private:
	virtual void setupMesh() override;
};

