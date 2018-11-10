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

using namespace std;

class GameObjectUtils
{
public:
	static vector<Texture*> getDiffuseTextures(string path);
	static vector<Texture*> getSpecularTextures(string path);
};

