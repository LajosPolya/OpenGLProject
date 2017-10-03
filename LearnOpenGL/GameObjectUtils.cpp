#include "GameObjectUtils.h"

std::vector<Texture*> GameObjectUtils::getDiffuseTextures(std::string path)
{
	Texture * diffuseMap;
	GLchar * tokens;
	GLchar* context = NULL;


	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Texture*> diffuseMaps;
	while (tokens != nullptr) {
		diffuseMap = new Texture(tokens, true);
		diffuseMap->name = "material.diffuse";
		diffuseMaps.push_back(diffuseMap);
		tokens = strtok_s(NULL, ",", &context);
	}
	return diffuseMaps;
}

std::vector<Texture*> GameObjectUtils::getSpecularTextures(std::string path)
{
	Texture * specularMap;
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Texture*> specularMaps;
	while (tokens != nullptr) {
		specularMap = new Texture(tokens, true);
		specularMap->name = "material.specular";
		specularMaps.push_back(specularMap);
		tokens = strtok_s(NULL, ",", &context);
	}
	return specularMaps;
}

std::vector<Mesh*> GameObjectUtils::getMeshes(std::string path, InstancedArrayTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps)
{
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Mesh*> mesh;
	GLint i = 0;
	while (tokens != NULL) {
		mesh.push_back(new Mesh(tokens, transform->getModels(), INSTANCED_ARRAY_SHADER, diffuseMaps[i], specularMaps[i]));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}

std::vector<Mesh*> GameObjectUtils::getMeshes(std::string path, InstancedArrayTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps, GLuint primitiveType)
{
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Mesh*> mesh;
	GLint i = 0;
	while (tokens != NULL) {
		mesh.push_back(new Mesh(tokens, transform->getModels(), INSTANCED_ARRAY_SHADER,  primitiveType, diffuseMaps[i], specularMaps[i]));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}

std::vector<Mesh*> GameObjectUtils::getMeshes(std::string path, InstancedTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps)
{
	GLchar * tokens;
	GLchar* context = NULL;

	tokens = strtok_s(&path[0], ",", &context);
	std::vector<Mesh*> mesh;
	GLint i = 0;
	while (tokens != NULL) {
		mesh.push_back(new Mesh(tokens, transform->getModels(), INSTANCED_ARRAY_SHADER, diffuseMaps[i], specularMaps[i]));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}

std::vector<Mesh*> GameObjectUtils::getMeshes(std::string path, TransparentTransformImpl * transform, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps)
{
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
		
		mesh.push_back(new Mesh(tokens, transform->getModels(), INSTANCED_ARRAY_SHADER, tempDif, tempSpec));
		i++;
		tokens = strtok_s(NULL, ",", &context);
	}
	return mesh;
}
