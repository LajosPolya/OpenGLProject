#include "ComplexShader.h"



ComplexShader::ComplexShader(const Camera & camera, LightsContainer * lightsContainer, glm::mat4 projection, const GLchar * vertexPath, const GLchar * fragmentPath) : InstancedArrayComplexShader(camera, lightsContainer, projection, vertexPath, fragmentPath) {
	// Calls Base Class Constructor
}

ComplexShader::ComplexShader(const Camera & camera, LightsContainer * lightsContainer, glm::mat4 projection, std::string materialPath, GLchar * vertexPath, const GLchar * fragmentPath) : InstancedArrayComplexShader(camera, lightsContainer, projection, materialPath, vertexPath, fragmentPath) {

}


ComplexShader::~ComplexShader() {}

void ComplexShader::sendModelToShader(glm::mat4 model) {
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));
}
