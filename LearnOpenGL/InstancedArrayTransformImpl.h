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

using namespace std;
using namespace glm;

class InstancedArrayTransformImpl : public InstancedTransform
{
public:
	InstancedArrayTransformImpl();
	~InstancedArrayTransformImpl();
	InstancedArrayTransformImpl(const InstancedArrayTransformImpl & toCopy);

	InstancedArrayTransformImpl(string fileLocation);
	InstancedArrayTransformImpl(vector<vec3> positions);
	InstancedArrayTransformImpl(vector<vec3> positions, vector<vec3> rotations, vector<vec3> scales);

	vector<vec3> * getPositions();
	vector<vec3> * getRotations();
	vector<vec3> * getScales();

	void setPositions(vector<vec3> positions);
	void setRotations(vector<vec3> rotations);
	void setScales(vector<vec3> scales);

	void setModels(vector<mat4> models);
	vector<mat4> getModels();

	void refreshModel();

	void Draw();

private:
	/* Only the file should set this */
	vector<vec3> Position;
	vector<vec3> Rotation;
	vector<vec3> Scale;

	vector<mat4> model;

	void readFile(string filename);
	void setup(vector<vec3> positions);
};
