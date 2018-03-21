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
		///specularMap = ResourceManager::getTexture(tokens);
		///if (specularMap == nullptr) {
			specularMap = new Texture(tokens, true);
			specularMap->setName("material.specular");
			///ResourceManager::addInstance(tokens, specularMap);
		///}
		specularMaps.push_back(specularMap);
		tokens = strtok_s(NULL, ",", &context);
	}
	return specularMaps;
}
