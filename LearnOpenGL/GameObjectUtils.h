#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "Texture.h"
#include "Material.h"
#include "InstancedMesh.h"
#include "InstancedArrayMesh.h"
#include "InstancedArrayTransformImpl.h"

class GameObjectUtils
{
public:
	static std::vector<Texture*> getDiffuseTextures(std::string path);
	static std::vector<Texture*> getSpecularTextures(std::string path);
};

