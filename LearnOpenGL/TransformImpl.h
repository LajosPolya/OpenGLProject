#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#include "Transform.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <map>

using namespace std;
using namespace glm;

class TransformImpl : public Transform {

public:

	TransformImpl();
	TransformImpl(const TransformImpl & toCopy);

	TransformImpl(string fileLocation);
	TransformImpl(vec3 Position, vec3 Rotation, vec3 Scale);

	vec3 getPosition();
	const vec3 getPosition() const;
	vec3 getRotation();
	vec3 getScale();

	void setPosition();
	void setPosition(vec3 pos);

	void setRotation();
	void setYRotation(GLfloat radians);

	void setScale();

	mat4 getModel();

	void refreshModel();

	void Draw();

private:

	/* Only the file should set this */
	vec3 Position;
	vec3 Rotation;
	vec3 Scale;

	/* These get updated which update the model */
	vec3 newPosition;
	vec3 newRotation;
	vec3 newScale;

	mat4 model;

	void readFile(string filename);
};
