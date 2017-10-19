#pragma once

#include <unordered_map>

#include "Material.h"
#include "Texture.h"
#include "TransformImpl.h"
#include "GameObject.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static void addInstance(std::string, void * ptr);
	static Material  * getMaterial(std::string path);
	static Texture * getTexture(std::string path);
	static TransformImpl * getTransform(std::string path, GameObject * gameObject);

private:
	static std::unordered_map<std::string, void *> manager;
};

