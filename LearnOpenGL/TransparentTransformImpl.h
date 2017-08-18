#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <algorithm>

class TransparentGameObjectImpl;

// TODO: Create a TransparentTransformImpl which derives from this class.
// But the difference is that in sorts the Transform vector in the Draw Method
// And Send an extra param to InstancedGameObjecImpl to instantiate the Transparent class
// Instead of this class
class TransparentTransformImpl
{
public:
	TransparentTransformImpl();
	~TransparentTransformImpl();

	TransparentTransformImpl(GLchar* fileLocation, TransparentGameObjectImpl * gameObject);

	glm::vec3* getPositions(GLuint i);

	glm::mat4 getModel(GLuint i);

	// TODO: Change mesh so we don't need to send it the model for this class
	std::vector<glm::mat4> getModels();

	void refreshModel();

	void Draw();

	TransparentGameObjectImpl* getGameObject();

private:
	struct InstancedTransformProps {
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;

		glm::mat4 model;
	};

	// TODO: Implement model to react to sort
	std::vector<glm::mat4> models;

	/* Reference to own GameObject instance*/
	TransparentGameObjectImpl* gameObject;

	/* Only the file should set this */
	std::vector<InstancedTransformProps> props;

	void readFile(GLchar* filename);

	GLboolean sortFunction(InstancedTransformProps const &x, InstancedTransformProps const &y);
};

