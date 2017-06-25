#pragma once

#ifndef RENDERER_H
#define RENDERER_H

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

class GameObject;

class Renderer {
public:

	Renderer(GameObject* gameObject, glm::mat4 projection);
	~Renderer();

	void Render();

private:
	GameObject* gameObject;
};

#endif // !RENDERER_H
