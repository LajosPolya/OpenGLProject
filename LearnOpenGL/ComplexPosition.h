#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "CompositionType.h"

using namespace std;
using namespace glm;

class ComplexPosition
{
public:
	ComplexPosition();
	~ComplexPosition();

	vector<vec3> getPositions(CompositionType i);
	void setPositions(vector<vec3> positions, CompositionType i);

	vector<vec3> getDrawablePositions(CompositionType i);
	void setDrawablePositions(vector<vec3> drawablePositions, CompositionType i);

private:
	vector<vector<vec3>> positions;
	vector<vector<vec3>> drawablePositions;
};

