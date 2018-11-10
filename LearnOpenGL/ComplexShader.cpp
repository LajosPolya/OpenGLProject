#include "ComplexShader.h"



ComplexShader::ComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string & vertexPath, const string & fragmentPath) : InstancedArrayComplexShader(camera, lightsContainer, projection, vertexPath, fragmentPath) {
	// Calls Base Class Constructor
}

ComplexShader::ComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string materialPath, const string & vertexPath, const string & fragmentPath) : InstancedArrayComplexShader(camera, lightsContainer, projection, materialPath, vertexPath, fragmentPath) {

}


ComplexShader::~ComplexShader() {}

void ComplexShader::sendModelToShader(mat4 model) {
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->shaderId, "model"), 1, GL_FALSE, value_ptr(model));
}
