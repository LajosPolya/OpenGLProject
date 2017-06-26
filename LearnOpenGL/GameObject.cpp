#pragma once

#include "GameObject.h"

GameObject::GameObject(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* specularMapLoc, GLchar* meshLoc, GLchar* materialLoc, GLchar* transformLoc, Camera * camera, glm::mat4 projection) {
	this->shader = new Shader(vertexShader, fragmentShader);
	this->diffuseMap = new Texture(diffuseMapLoc, true);
	this->diffuseMap->name = "material.diffuse";
	this->specularMap = new Texture(specularMapLoc, true);
	this->specularMap->name = "material.specular";
	std::vector<Texture> textures;
	textures.push_back(*(this->diffuseMap));
	textures.push_back(*(this->specularMap));
	this->mesh = new Mesh(meshLoc, textures, *(this->shader));
	this->material = new Material(materialLoc, *(this->shader));
	this->transform = new Transform(transformLoc, this);
	this->camera = camera;
	this->projection = projection;

	this->shader->Use();

	this->renderer = new Renderer(this, this->projection);
}

void GameObject::Draw() {

	this->transform->refreshModel();
	this->transform->setRotation();
	this->transform->setPosition();
	this->transform->setScale();
	this->transform->Draw();

	this->renderer->Render();
	this->mesh->Draw(*(this->shader));
}

Shader * GameObject::getShader() {
	return this->shader;
}

Camera * GameObject::getCamera() {
	return this->camera;
}

Transform * GameObject::getTransform() {
	return this->transform;
}

Texture * GameObject::getDiffuseMap()
{
	return this->diffuseMap;
}

Texture * GameObject::getSpecularMap()
{
	return this->specularMap;
}
