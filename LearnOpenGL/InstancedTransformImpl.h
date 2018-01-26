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

#include "InstancedTransform.h"

class InstancedGameObjectImpl;

// TODO: Create a TransparentTransformImpl which derives from this class.
// But the difference is that in sorts the Transform vector in the Draw Method
// And Send an extra param to InstancedGameObjecImpl to instantiate the Transparent class
// Instead of this class
class InstancedTransformImpl : public InstancedTransform
{
public:
	InstancedTransformImpl();
	~InstancedTransformImpl();

	InstancedTransformImpl(GLchar * fileLocation, InstancedGameObjectImpl * gameObject);
	InstancedTransformImpl(GLchar * fileLocation);

	std::vector<glm::vec3> * getPositions();
	std::vector<glm::vec3> * getRotations();
	std::vector<glm::vec3> * getScales();

	std::vector<glm::mat4> getModels();
	void setModels(std::vector<glm::mat4> models);

	void setPositions(std::vector<glm::vec3> positions);
	void setRotations(std::vector<glm::vec3> rotations);
	void setScales(std::vector<glm::vec3> scales);

	void refreshModel();

	void Draw();
	
	InstancedGameObjectImpl * getGameObject();
	void setGameObject(InstancedGameObjectImpl * gameObject);

private:
	/* Reference to own GameObject instance*/
	InstancedGameObjectImpl * gameObject = nullptr;

	/* Only the file should set this */
	std::vector<glm::vec3> Position;
	std::vector<glm::vec3> Rotation;
	std::vector<glm::vec3> Scale;

	std::vector<glm::mat4> model;

	void readFile(std::string filename);
};

