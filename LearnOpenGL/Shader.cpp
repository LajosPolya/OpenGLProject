#include "Shader.h"
#include "GameObjectImpl.h"
#include "InstancedGameObjectImpl.h"
#include "InstancedArrayGameObjectImpl.h"

Shader::Shader() {}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// 1. Retrieve the source code from filepath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open File
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// Read buffer into stream
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close files
		vShaderFile.close();
		fShaderFile.close();

		// Convert Stream into GLchar array
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2. Compile Shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print Compile Errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print Compile Errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);

	// Print Linking Errors
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the Shaders as they are Linked to our Program and are no Longer Necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use() {
	glUseProgram(this->Program);
}

void Shader::sendToShader(DirLight * dirLight, SpotLight * spotLight, std::vector<PointLight>* pointLights)
{
	this->Use();
	// Directional light
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.direction"), dirLight->direction.x, dirLight->direction.y, dirLight->direction.z);
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.ambient"), dirLight->ambient.x, dirLight->ambient.y, dirLight->ambient.z);
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.diffuse"), dirLight->diffuse.x, dirLight->diffuse.y, dirLight->diffuse.z);
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.specular"), dirLight->specular.x, dirLight->specular.y, dirLight->specular.z);

	// Point Light
	glUniform1i(glGetUniformLocation(this->Program, "size"), pointLights->size());
	for (GLuint i = 0; i < pointLights->size(); i++) {
		glUniform3f(glGetUniformLocation(this->Program, ("pointLights[" + std::to_string(i) + "].position").c_str()), (*pointLights)[i].position.x, (*pointLights)[i].position.y, (*pointLights)[i].position.z);
		glUniform3f(glGetUniformLocation(this->Program, ("pointLights[" + std::to_string(i) + "].ambient").c_str()), (*pointLights)[i].ambient.x, (*pointLights)[i].ambient.y, (*pointLights)[i].ambient.z);
		glUniform3f(glGetUniformLocation(this->Program, ("pointLights[" + std::to_string(i) + "].diffuse").c_str()), (*pointLights)[i].diffuse.x, (*pointLights)[i].diffuse.y, (*pointLights)[i].diffuse.z);
		glUniform3f(glGetUniformLocation(this->Program, ("pointLights[" + std::to_string(i) + "].specular").c_str()), (*pointLights)[i].specular.x, (*pointLights)[i].specular.y, (*pointLights)[i].specular.z);
		glUniform1f(glGetUniformLocation(this->Program, ("pointLights[" + std::to_string(i) + "].constant").c_str()), (*pointLights)[i].constant);
		glUniform1f(glGetUniformLocation(this->Program, ("pointLights[" + std::to_string(i) + "].linear").c_str()), (*pointLights)[i].linear);
		glUniform1f(glGetUniformLocation(this->Program, ("pointLights[" + std::to_string(i) + "].quadratic").c_str()), (*pointLights)[i].quadratic);
	}

	// SpotLight
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.ambient"), spotLight->ambient.x, spotLight->ambient.y, spotLight->ambient.z);
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.diffuse"), spotLight->diffuse.x, spotLight->diffuse.y, spotLight->diffuse.z);
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.specular"), spotLight->specular.x, spotLight->specular.y, spotLight->specular.z);
	glUniform1f(glGetUniformLocation(this->Program, "spotLight.constant"), spotLight->constant);
	glUniform1f(glGetUniformLocation(this->Program, "spotLight.linear"), spotLight->linear);
	glUniform1f(glGetUniformLocation(this->Program, "spotLight.quadratic"), spotLight->quadratic);
	glUniform1f(glGetUniformLocation(this->Program, "spotLight.cutOff"), glm::cos(glm::radians(spotLight->cutOff)));
	glUniform1f(glGetUniformLocation(this->Program, "spotLight.outerCutOff"), glm::cos(glm::radians(spotLight->outerCutOff)));
}


// TODO: Take out common code from sendToShader({InstancedArray}GameObjectImpl * gameObject);
void Shader::sendToShader(GameObjectImpl * gameObject)
{
	this->Use();

	glUniform3f(glGetUniformLocation(this->Program, "viewPos"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);

	// Bind Diffuse Map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameObject->getDiffuseMap()->getTextureID());

	// Bind Specular Map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gameObject->getSpecularMap()->getTextureID());

	glUniform1i(glGetUniformLocation(this->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(this->Program, "material.specular"), 1);

	glUniformMatrix4fv(glGetUniformLocation(this->Program, "view"), 1, GL_FALSE, glm::value_ptr(gameObject->getCamera()->GetViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(this->Program, "model"), 1, GL_FALSE, glm::value_ptr(gameObject->getTransform()->getModel()));

	// Set material properties
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.position"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.direction"), gameObject->getCamera()->Front.x, gameObject->getCamera()->Front.y, gameObject->getCamera()->Front.z);
}

void Shader::sendToShader(InstancedArrayGameObjectImpl * gameObject)
{
	this->Use();

	glUniform3f(glGetUniformLocation(this->Program, "viewPos"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);

	// Bind Diffuse Map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameObject->getDiffuseMap()->getTextureID());

	// Bind Specular Map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gameObject->getSpecularMap()->getTextureID());

	glUniform1i(glGetUniformLocation(this->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(this->Program, "material.specular"), 1);

	glUniformMatrix4fv(glGetUniformLocation(this->Program, "view"), 1, GL_FALSE, glm::value_ptr(gameObject->getCamera()->GetViewMatrix()));

	// Set material properties
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.position"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.direction"), gameObject->getCamera()->Front.x, gameObject->getCamera()->Front.y, gameObject->getCamera()->Front.z);

}

void Shader::sendToShader(InstancedGameObjectImpl * gameObject)
{
	this->Use();

	glUniform3f(glGetUniformLocation(this->Program, "viewPos"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);

	// Bind Diffuse Map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameObject->getDiffuseMap()->getTextureID());

	// Bind Specular Map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gameObject->getSpecularMap()->getTextureID());

	glUniform1i(glGetUniformLocation(this->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(this->Program, "material.specular"), 1);

	for (GLuint i = 0; i < gameObject->getTransform()->getModels().size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(this->Program, ("model[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(gameObject->getTransform()->getModels()[i]));
	}

	glUniformMatrix4fv(glGetUniformLocation(this->Program, "view"), 1, GL_FALSE, glm::value_ptr(gameObject->getCamera()->GetViewMatrix()));

	// Set material properties
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.position"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.direction"), gameObject->getCamera()->Front.x, gameObject->getCamera()->Front.y, gameObject->getCamera()->Front.z);

}

void Shader::sendToShader(Material * material)
{
	this->Use();
	glUniform1f(glGetUniformLocation(this->Program, "material.shininess"), material->getShininess());
}

void Shader::setProjectionMatrix(glm::mat4 projection)
{
	this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}


// TODO: This can allow Shaders to have variable amount of Textures
/*
for (GLuint i = 0; i < this->textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i); // Activate Texture Unit

    glUniform1i(glGetUniformLocation(shader.Program, this->textures[i].name), i);
    glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
}
*/