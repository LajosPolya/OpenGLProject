#pragma once

#include <unordered_map>

#include "Material.h"
#include "Texture.h"
#include "TransformImpl.h"
#include "InstancedArrayTransformImpl.h"
#include "GameObject.h"

// TODO: Use Copy Constructor
/* I'm not sure yet if I wan't to keep this class but if I do it should really use the CopyConstructor(const &) instead of the normal Constructor */

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

private:
	static std::unordered_map<std::string, void *> manager;
};

