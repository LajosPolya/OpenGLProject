#include "InstancedComplexShader.h"


InstancedComplexShader::InstancedComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const glm::mat4 & projection, const GLchar * vertexPath, const GLchar * fragmentPath) : InstancedArrayComplexShader(camera, lightsContainer, projection, vertexPath, fragmentPath) {
	// Calls the Base Class' Constructor
}

InstancedComplexShader::InstancedComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const glm::mat4 & projection, std::string materialPath, const GLchar * vertexPath, const GLchar * fragmentPath) : InstancedArrayComplexShader(camera, lightsContainer, projection, materialPath, vertexPath, fragmentPath) {

}

InstancedComplexShader::~InstancedComplexShader() {}

void InstancedComplexShader::sendInstancesToShader(std::vector<glm::mat4> intances) {
	this->use();
	for (GLuint i = 0; i < intances.size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(this->shaderId, ("model[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(intances[i]));
	}
}
