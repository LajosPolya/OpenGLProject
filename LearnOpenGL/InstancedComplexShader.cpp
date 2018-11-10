#include "InstancedComplexShader.h"


InstancedComplexShader::InstancedComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string & vertexPath, const string & fragmentPath) : InstancedArrayComplexShader(camera, lightsContainer, projection, vertexPath, fragmentPath) {
	// Calls the Base Class' Constructor
}

InstancedComplexShader::InstancedComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string materialPath, const string & vertexPath, const string & fragmentPath) : InstancedArrayComplexShader(camera, lightsContainer, projection, materialPath, vertexPath, fragmentPath) {

}

InstancedComplexShader::~InstancedComplexShader() {}

void InstancedComplexShader::sendInstancesToShader(vector<mat4> intances) {
	this->use();
	for (GLuint i = 0; i < intances.size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(this->shaderId, ("model[" + to_string(i) + "]").c_str()), 1, GL_FALSE, value_ptr(intances[i]));
	}
}
