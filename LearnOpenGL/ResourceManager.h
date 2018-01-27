#pragma once

#include <unordered_map>

#include "Material.h"
#include "Texture.h"
#include "TransformImpl.h"
#include "InstancedArrayTransformImpl.h"
#include "GameObject.h"
#include "InstancedArrayGameObjectImpl.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static void addInstance(std::string, void * ptr);
	static Material  * getMaterial(std::string path);
	static Texture * getTexture(std::string path);
	// TODO: GameOBject is no longer needed here
	static TransformImpl * getTransform(std::string path, GameObject * gameObject);
	static InstancedArrayTransformImpl * getTransform(std::string path, InstancedArrayGameObjectImpl * gameObject);

private:
	static std::unordered_map<std::string, void *> manager;
};

