#include "InstancedComplexShader.h"


InstancedComplexShader::InstancedComplexShader(Camera * camera, LightsContainer * lightsContainer, glm::mat4 projection, const GLchar* vertexPath, const GLchar* fragmentPath) : ComplexShader(camera, lightsContainer, projection, vertexPath, fragmentPath) {
	// Calls the Base Class' Constructor
}

InstancedComplexShader::~InstancedComplexShader() {}

void InstancedComplexShader::sendInstancesToShader(std::vector<glm::mat4> intances) {
	for (GLuint i = 0; i < intances.size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(this->shaderId, ("model[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(intances[i]));
	}
}
