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

using namespace std;
using namespace glm;

class TransparentTransformImpl
{
public:
	TransparentTransformImpl();
	~TransparentTransformImpl();

	TransparentTransformImpl(GLchar * fileLocation);

	vec3 * getPositions(GLuint i);

	mat4 getModel(GLuint i);

	vector<mat4> getModels();

	void refreshModel();

	void Draw();

	GLuint getSize();

private:
	struct InstancedTransformProps {
		vec3 Position;
		vec3 Rotation;
		vec3 Scale;

		mat4 model;
	};

	/* This allows me to return models which are sorted,
	   the above data structure allows me to get a model by index
	*/
	vector<mat4> models;

	/* Only the file should set this */
	vector<InstancedTransformProps> props;

	void readFile(GLchar * filename);

	// TODO: This needs to be fixed since Transform doesn't have reference back to GameObject
	GLboolean sortFunction(InstancedTransformProps const &x, InstancedTransformProps const &y);
};

