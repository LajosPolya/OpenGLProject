#pragma once

#include <unordered_map>

#include "Material.h"
#include "Texture.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static void addInstance(std::string, void * ptr);
	static Material  * getMaterial(std::string path);
	static Texture * getTexture(std::string path);

private:
	static std::unordered_map<std::string, void *> manager;
};

