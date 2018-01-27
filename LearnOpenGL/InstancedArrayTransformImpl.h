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

class InstancedArrayTransformImpl : public InstancedTransform
{
public:
	InstancedArrayTransformImpl();
	~InstancedArrayTransformImpl();
	InstancedArrayTransformImpl(const InstancedArrayTransformImpl & toCopy);

	InstancedArrayTransformImpl(std::string fileLocation);
	InstancedArrayTransformImpl(std::vector<glm::vec3> positions);
	InstancedArrayTransformImpl(std::vector<glm::vec3> positions, std::vector<glm::vec3> rotations, std::vector<glm::vec3> scales);

	std::vector<glm::vec3> * getPositions();
	std::vector<glm::vec3> * getRotations();
	std::vector<glm::vec3> * getScales();

	void setPositions(std::vector<glm::vec3> positions);
	void setRotations(std::vector<glm::vec3> rotations);
	void setScales(std::vector<glm::vec3> scales);

	void setModels(std::vector<glm::mat4> models);
	std::vector<glm::mat4> getModels();

	void refreshModel();

	void Draw();

private:
	/* Only the file should set this */
	std::vector<glm::vec3> Position;
	std::vector<glm::vec3> Rotation;
	std::vector<glm::vec3> Scale;

	std::vector<glm::mat4> model;

	void readFile(std::string filename);
	void setup(std::vector<glm::vec3> positions);
};
