#pragma once

using namespace std;
using namespace glm;

class InstancedTransform {
public:

	virtual vector<vec3> * getPositions() = 0;

	virtual vector<mat4> getModels() = 0;

	virtual void refreshModel() = 0;

	virtual void Draw() = 0;

private:

	virtual void readFile(string filename) = 0;
};