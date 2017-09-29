#include "Shader.h"
#include "GameObjectImpl.h"
#include "InstancedGameObject.h"
#include "InstancedGameObjectImpl.h"
#include "InstancedArrayGameObjectImpl.h"
#include "TransparentGameObjectImpl.h"

Shader::Shader() {}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// 1. Retrieve the source code from filepath
	std::string vertexCode;
	std::string fragmentCode;

	this->readShaderFile(vertexPath, &vertexCode);
	this->readShaderFile(fragmentPath, &fragmentCode);
	const GLchar * vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();

	// 2. Compile Shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Shader Program
	this->Program = glCreateProgram();

	// Vertex Shader
	vertex = createShader(GL_VERTEX_SHADER, vShaderCode);

	// Fragment Shader
	fragment = createShader(GL_FRAGMENT_SHADER, fShaderCode);

	// Link the previously attached Shaders (glAttachShader) to the program (glCreateProgram)
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

// TODO: Create Constructor which also creates Geometry Shader
Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, GLchar * geometryPath) : Shader(vertexPath, fragmentPath)
{
}

// TODO: Some common method to share code between constructors
void Shader::createShaders(GLuint & vertex, GLuint & fragment)
{
}

void Shader::readShaderFile(const GLchar * path, std::string * code)
{
	std::ifstream shaderFile;

	// Ensure ifstream objects can throw exceptions
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open File
		shaderFile.open(path);
		std::stringstream shaderStream;

		// Read buffer into stream
		shaderStream << shaderFile.rdbuf();

		// Close files
		shaderFile.close();

		// Convert Stream into GLchar array
		*code = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ : " << path << std::endl;
	}
}

GLuint Shader::createShader(GLint type, const GLchar * code)
{
	// 2. Compile Shaders
	GLuint shader;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	// Print Compile Errors
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glAttachShader(this->Program, shader);
	return shader;
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


// TODO: Don't send data to Shader every frame.
// Only send data to shader when it has changed.
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
	
	this->sendCommonToShader(gameObject);
}

// TODO: SPIKE : All GameObjects should be equal except that they have different Transforms
// Thus they could all have the same Shader method to send info to the shader and separate Shader methods for the Transforms
void Shader::sendToShader(InstancedGameObjectImpl * gameObject)
{
	this->Use();

	this->sendCommonToShader(gameObject);

	std::vector<glm::mat4> models = gameObject->getTransform()->getModels();
	for (GLuint i = 0; i < gameObject->getTransform()->getModels().size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(this->Program, ("model[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(models[i]));
	}
}

void Shader::sendToShader(TransparentGameObjectImpl * gameObject)
{
	this->Use();
	sendCommonToShader(gameObject);

	for (GLuint i = 0; i < gameObject->getTransform()->getModels().size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(this->Program, ("model[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(gameObject->getTransform()->getModel(i)));
	}
}

void Shader::sendToShader(Material * material)
{
	this->Use();
	glUniform1f(glGetUniformLocation(this->Program, "material.shininess"), material->getShininess());
}

void Shader::sendToShader(Mesh * mesh)
{
	if (mesh->getDiffuseMap() != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->getDiffuseMap()->getTextureID());
	}

	// Bind Specular Map
	if (mesh->getSpecularMap() != nullptr) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh->getSpecularMap()->getTextureID());
	}
}

void Shader::setProjectionMatrix(glm::mat4 projection)
{
	this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::sendCommonToShader(TransparentGameObjectImpl * gameObject)
{
	glUniform3f(glGetUniformLocation(this->Program, "viewPos"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);
	
	glUniform1i(glGetUniformLocation(this->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(this->Program, "material.specular"), 1);

	glUniformMatrix4fv(glGetUniformLocation(this->Program, "view"), 1, GL_FALSE, glm::value_ptr(gameObject->getCamera()->GetViewMatrix()));

	// Set material properties
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.position"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.direction"), gameObject->getCamera()->Front.x, gameObject->getCamera()->Front.y, gameObject->getCamera()->Front.z);
}

void Shader::sendCommonToShader(InstancedGameObject * gameObject)
{
	glUniform3f(glGetUniformLocation(this->Program, "viewPos"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);

	glUniform1i(glGetUniformLocation(this->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(this->Program, "material.specular"), 1);

	glUniformMatrix4fv(glGetUniformLocation(this->Program, "view"), 1, GL_FALSE, glm::value_ptr(gameObject->getCamera()->GetViewMatrix()));

	// Set material properties
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.position"), gameObject->getCamera()->Position.x, gameObject->getCamera()->Position.y, gameObject->getCamera()->Position.z);
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.direction"), gameObject->getCamera()->Front.x, gameObject->getCamera()->Front.y, gameObject->getCamera()->Front.z);
}

// TODO: This can allow Shaders to have variable amount of Textures
/*
for (GLuint i = 0; i < this->textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i); // Activate Texture Unit

    glUniform1i(glGetUniformLocation(shader.Program, this->textures[i].name), i);
    glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
}
*/