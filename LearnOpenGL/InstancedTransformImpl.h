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

// TODO: Create a TransparentTransformImpl which derives from this class.
// But the difference is that in sorts the Transform vector in the Draw Method
// And Send an extra param to InstancedGameObjecImpl to instantiate the Transparent class
// Instead of this class
class InstancedTransformImpl : public InstancedTransform
{
public:
	InstancedTransformImpl();
	~InstancedTransformImpl();

	InstancedTransformImpl(string fileLocation);
	InstancedTransformImpl(vector<vec3> positions, vector<vec3> rotations, vector<vec3> scales);

	vector<vec3> * getPositions();
	vector<vec3> * getRotations();
	vector<vec3> * getScales();

	vector<mat4> getModels();
	void setModels(vector<mat4> models);

	void setPositions(vector<vec3> positions);
	void setRotations(vector<vec3> rotations);
	void setScales(vector<vec3> scales);

	void addNewInstance(vec3 position, vec3 rotation, vec3 scale);

	void refreshModel();

	void Draw();

private:

	/* Only the file should set this */
	vector<vec3> Position;
	vector<vec3> Rotation;
	vector<vec3> Scale;

	vector<mat4> model;

	void readFile(string filename);

	mat4 doTransformOperations(vec3 position, vec3 rotation, vec3 scale);
};

