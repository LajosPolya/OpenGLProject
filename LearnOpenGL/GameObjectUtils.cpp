#include "GameObjectUtils.h"

std::vector<Texture*> GameObjectUtils::getDiffuseTextures(std::string path) {
	Texture * diffuseMap = nullptr;
	GLchar * tokens;
	GLchar* context = NULL;


	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Texture*> diffuseMaps;
	while (tokens != nullptr) {
		///diffuseMap = ResourceManager::getTexture(tokens);
		///if (diffuseMap == nullptr) {
			diffuseMap = new Texture(tokens, true);
			diffuseMap->setName("material.diffuse");
			///ResourceManager::addInstance(tokens, diffuseMap);
		///}
		diffuseMaps.push_back(diffuseMap);
		tokens = strtok_s(NULL, ",", &context);
	}
	return diffuseMaps;
}

std::vector<Texture*> GameObjectUtils::getSpecularTextures(std::string path) {
	Texture * specularMap;
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Texture*> specularMaps;
	while (tokens != nullptr) {
		specularMap = ResourceManager::getTexture(tokens);
		if (specularMap == nullptr) {
			specularMap = new Texture(tokens, true);
			specularMap->setName("material.specular");
			ResourceManager::addInstance(tokens, specularMap);
		}
		specularMaps.push_back(specularMap);
		tokens = strtok_s(NULL, ",", &context);
	}
	return specularMaps;
}

std::vector<Mesh*> GameObjectUtils::getMeshes(std::string path, Transform * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps) {
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Mesh*> mesh;
	GLuint i = 0;
	Texture * tempDif;
	Texture * tempSpec;
	while (tokens != NULL) {
		tempDif = nullptr;
		if (i < diffuseMaps.size()) {
			tempDif = diffuseMaps[i];
		}
		tempSpec = nullptr;
		if (i < specularMaps.size()) {
			tempSpec = specularMaps[i];
		}
		mesh.push_back(new Mesh(tokens, transform, tempDif, tempSpec));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}

std::vector<Mesh*> GameObjectUtils::getMeshes(std::string path, InstancedArrayTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps) {
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Mesh*> mesh;
	GLint i = 0;
	while (tokens != NULL) {
		mesh.push_back(new Mesh(tokens, transform, diffuseMaps[i], specularMaps[i]));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}

std::vector<Mesh*> GameObjectUtils::getMeshes(std::string path, InstancedArrayTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps, GLuint primitiveType) {
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Mesh*> mesh;
	GLuint i = 0;
	Texture * tempDif;
	Texture * tempSpec;
	while (tokens != NULL) {
		tempDif = nullptr;
		if (i < diffuseMaps.size()) {
			tempDif = diffuseMaps[i];
		}
		tempSpec = nullptr;
		if (i < specularMaps.size()) {
			tempSpec = specularMaps[i];
		}
		mesh.push_back(new Mesh(tokens, transform,  primitiveType, tempDif, tempSpec));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}

std::vector<Mesh*> GameObjectUtils::getMeshes(std::string path, InstancedTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps) {
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Mesh*> mesh;
	GLint i = 0;
	while (tokens != NULL) {
		mesh.push_back(new Mesh(tokens, transform, diffuseMaps[i], specularMaps[i]));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}

std::vector<Mesh*> GameObjectUtils::getMeshes(std::string path, TransparentTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps) {
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Mesh*> mesh;
	GLuint i = 0;
	Texture * tempDif;
	Texture * tempSpec;
	while (tokens != NULL) {
		
		tempDif = nullptr;
		if (i < diffuseMaps.size()) {
			tempDif = diffuseMaps[i];
		}
		tempSpec = nullptr;
		if (i < specularMaps.size()) {
			tempSpec = specularMaps[i];
		}
		
		mesh.push_back(new Mesh(tokens, transform->getModels(), INSTANCED_SHADER, tempDif, tempSpec));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}

Material * GameObjectUtils::getMaterial(std::string path) {
	Material * material = ResourceManager::getMaterial(path);
	if (material == nullptr) {
		material = new Material(path);
		ResourceManager::addInstance(path, material);
	}
	return material;
}

// TODO: GameOBject is no longer needed here
TransformImpl * GameObjectUtils::getTransform(std::string path, GameObject * gameObject) {
	TransformImpl * transform = ResourceManager::getTransform(path, gameObject);
	if (transform == nullptr) {
		transform = new TransformImpl(path);
		ResourceManager::addInstance(path, transform);
	}
	return transform;
}

// TODO: GameOBject is no longer needed here
InstancedArrayTransformImpl * GameObjectUtils::getTransform(std::string path, InstancedArrayGameObjectImpl * gameObject) {
	InstancedArrayTransformImpl * transform = ResourceManager::getTransform(path, gameObject);
	if (transform == nullptr) {
		transform = new InstancedArrayTransformImpl(path);
		ResourceManager::addInstance(path, transform);
	}
	return transform;
}
