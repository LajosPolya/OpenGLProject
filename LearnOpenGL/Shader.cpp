#include "Shader.h"
#include "TransparentGameObjectImpl.h"

Shader::Shader() {}

Shader::Shader(const Shader & toCopy) {
	this->Program = toCopy.Program;
}

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath) {
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

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, GLchar * geometryPath) {
	// 1. Retrieve the source code from filepath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	this->readShaderFile(vertexPath, &vertexCode);
	this->readShaderFile(fragmentPath, &fragmentCode);
	this->readShaderFile(geometryPath, &geometryCode);
	const GLchar * vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();
	const GLchar * gShaderCode = geometryCode.c_str();

	// 2. Compile Shaders
	GLuint vertex, fragment, geometry;
	GLint success;
	GLchar infoLog[512];

	// Shader Program
	this->Program = glCreateProgram();

	// Vertex Shader
	vertex = createShader(GL_VERTEX_SHADER, vShaderCode);

	// Fragment Shader
	fragment = createShader(GL_FRAGMENT_SHADER, fShaderCode);

	// Geometry Shader
	geometry = createShader(GL_GEOMETRY_SHADER, gShaderCode);

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
	glDeleteShader(geometry);
}

void Shader::readShaderFile(const GLchar * path, std::string * code) {
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

GLuint Shader::createShader(GLint type, const GLchar * code) {
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
		std::string shaderName = std::string("");
		if (type == GL_VERTEX_SHADER) {
			shaderName = std::string("Vertex");
		}
		else if (type == GL_FRAGMENT_SHADER) {
			shaderName = std::string("Fragment");
		}
		else if (type == GL_GEOMETRY_SHADER) {
			shaderName = std::string("Geometry");
		}
		std::cout << "ERROR::SHADER::" + shaderName + "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glAttachShader(this->Program, shader);
	return shader;
}

void Shader::Use() {
	glUseProgram(this->Program);
}

void Shader::sendToShader(LightsContainer * lightsContainer) {
	DirLight * dirLight = lightsContainer->getDirLight();
	SpotLight * spotLight = lightsContainer->getSpotLight();
	std::vector<PointLight> * pointLights = lightsContainer->getPointLights();

	this->Use();
	// Directional light
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.direction"), dirLight->direction.x, dirLight->direction.y, dirLight->direction.z);
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.ambient"), dirLight->ambient.x, dirLight->ambient.y, dirLight->ambient.z);
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.diffuse"), dirLight->diffuse.x, dirLight->diffuse.y, dirLight->diffuse.z);
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.specular"), dirLight->specular.x, dirLight->specular.y, dirLight->specular.z);

	// Point Light
	glUniform1i(glGetUniformLocation(this->Program, "size"), pointLights->size());
	std::string iString;
	for (GLuint i = 0; i < pointLights->size(); i++) {
		iString = std::to_string(i);
		std::string pointLightString = "pointLights[" + iString + "].";
		glUniform3f(glGetUniformLocation(this->Program, (pointLightString + "position").c_str()), (*pointLights)[i].position.x, (*pointLights)[i].position.y, (*pointLights)[i].position.z);
		glUniform3f(glGetUniformLocation(this->Program, (pointLightString + "ambient").c_str()), (*pointLights)[i].ambient.x, (*pointLights)[i].ambient.y, (*pointLights)[i].ambient.z);
		glUniform3f(glGetUniformLocation(this->Program, (pointLightString + "diffuse").c_str()), (*pointLights)[i].diffuse.x, (*pointLights)[i].diffuse.y, (*pointLights)[i].diffuse.z);
		glUniform3f(glGetUniformLocation(this->Program, (pointLightString + "specular").c_str()), (*pointLights)[i].specular.x, (*pointLights)[i].specular.y, (*pointLights)[i].specular.z);
		glUniform1f(glGetUniformLocation(this->Program, (pointLightString + "constant").c_str()), (*pointLights)[i].constant);
		glUniform1f(glGetUniformLocation(this->Program, (pointLightString + "linear").c_str()), (*pointLights)[i].linear);
		glUniform1f(glGetUniformLocation(this->Program, (pointLightString + "quadratic").c_str()), (*pointLights)[i].quadratic);
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

void Shader::sendToShader(TransparentGameObjectImpl * gameObject) {
	this->Use();
	sendCommonToShader(gameObject);

	std::vector<glm::mat4> models = gameObject->getTransform()->getModels();
	for (GLuint i = 0; i < models.size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(this->Program, ("model[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(models[i]));
	}
}

void Shader::sendToShader(Material * material) {
	this->Use();
	glUniform1f(glGetUniformLocation(this->Program, "material.shininess"), material->getShininess());
}

void Shader::setProjectionMatrix(glm::mat4 projection) {
	this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::sendCommonToShader(TransparentGameObjectImpl * gameObject) {
	Camera * camera = gameObject->getCamera();
	glUniform3f(glGetUniformLocation(this->Program, "viewPos"), camera->Position.x, camera->Position.y, camera->Position.z);
	
	glUniform1i(glGetUniformLocation(this->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(this->Program, "material.specular"), 1);

	glUniformMatrix4fv(glGetUniformLocation(this->Program, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	// Set material properties
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.position"), camera->Position.x, camera->Position.y, camera->Position.z);
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.direction"), camera->Front.x, camera->Front.y, camera->Front.z);
}

// TODO: This can allow Shaders to have variable amount of Textures
/*
for (GLuint i = 0; i < this->textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i); // Activate Texture Unit

    glUniform1i(glGetUniformLocation(shader.Program, this->textures[i].name), i);
    glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
}
*/