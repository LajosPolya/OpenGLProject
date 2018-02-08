#pragma once

#include <vector>
#include <string>

#include "Texture.h"
#include "Material.h"
#include "InstancedMesh.h"
#include "InstancedArrayMesh.h"
#include "InstancedArrayTransformImpl.h"
#include "InstancedTransformImpl.h"
#include "TransparentTransformImpl.h"
#include "TransformImpl.h"
#include "ResourceManager.h"
#include "GameObject.h"

class GameObjectUtils
{
public:
	static std::vector<Texture*> getDiffuseTextures(std::string path);
	static std::vector<Texture*> getSpecularTextures(std::string path);
	static std::vector<Mesh*> getMeshes(std::string path, Transform * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
	static std::vector<InstancedArrayMesh*> getMeshes(std::string path, InstancedArrayTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
	static std::vector<InstancedArrayMesh*> getMeshes(std::string path, InstancedArrayTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps, GLuint primitiveType);
	static std::vector<InstancedMesh*> getMeshes(std::string path, InstancedTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
	static std::vector<InstancedMesh*> getMeshes(std::string path, TransparentTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
	static Material * getMaterial(std::string path);
	// TODO: GameOBject is no longer needed here
	static TransformImpl * getTransform(std::string path, GameObject * gameObject);
};

