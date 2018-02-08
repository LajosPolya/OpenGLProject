#pragma once

#include "Mesh.h"


class InstancedMesh : private Mesh
{
public:
	InstancedMesh(GLchar * vertexLocation, InstancedTransformImpl * transform, Texture * diffuseMap, Texture * specularMap);
	InstancedMesh(GLchar * vertexLocation, std::vector<glm::mat4> instances, Texture * diffuseMap, Texture * specularMap);
	~InstancedMesh();

	virtual void Draw() override;

	void setInstances(std::vector<glm::mat4> instances);
};

