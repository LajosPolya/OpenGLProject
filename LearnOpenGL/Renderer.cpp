#include "Renderer.h"
#include "GameObject.h"



Renderer::Renderer(GameObject* gameObject, glm::mat4 projection) {
	this->gameObject = gameObject;
	glUniformMatrix4fv(glGetUniformLocation(this->gameObject->getShader()->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}


Renderer::~Renderer() {}

void Renderer::Render() {


	this->gameObject->getShader()->Use();

	glUniform3f(glGetUniformLocation(this->gameObject->getShader()->Program, "viewPos"), this->gameObject->getCamera()->Position.x, this->gameObject->getCamera()->Position.y, this->gameObject->getCamera()->Position.z);

	// Bind Diffuse Map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->gameObject->getDiffuseMap()->getTextureID());

	// Bind Specular Map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->gameObject->getSpecularMap()->getTextureID());

	// The Zero here can be GL_TEXTURE0
	// and One here can be GL_TEXTURE0 + 1
	glUniform1i(glGetUniformLocation(this->gameObject->getShader()->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(this->gameObject->getShader()->Program, "material.specular"), 1);

	glUniformMatrix4fv(glGetUniformLocation(this->gameObject->getShader()->Program, "view"), 1, GL_FALSE, glm::value_ptr(this->gameObject->getCamera()->GetViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(this->gameObject->getShader()->Program, "model"), 1, GL_FALSE, glm::value_ptr(this->gameObject->getTransform()->getModel()));

	// Set material properties
	glUniform3f(glGetUniformLocation(this->gameObject->getShader()->Program, "spotLight.position"), this->gameObject->getCamera()->Position.x, this->gameObject->getCamera()->Position.y, this->gameObject->getCamera()->Position.z);
	glUniform3f(glGetUniformLocation(this->gameObject->getShader()->Program, "spotLight.direction"), this->gameObject->getCamera()->Front.x, this->gameObject->getCamera()->Front.y, this->gameObject->getCamera()->Front.z);
}
