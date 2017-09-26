#pragma once

#include <vector>
#include <string>

#include "Texture.h"
#include "Mesh.h"
#include "InstancedArrayTransformImpl.h"
#include "InstancedTransformImpl.h"
#include "TransparentTransformImpl.h"

class GameObjectUtils
{
public:
	static std::vector<Texture*> getDiffuseTextures(std::string path);
	static std::vector<Texture*> getSpecularTextures(std::string path);
	static std::vector<Mesh*> getMeshes(std::string path, InstancedArrayTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
	static std::vector<Mesh*> getMeshes(std::string path, InstancedTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
	static std::vector<Mesh*> getMeshes(std::string path, TransparentTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
};

