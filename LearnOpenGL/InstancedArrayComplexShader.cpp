#include "InstancedArrayComplexShader.h"

InstancedArrayComplexShader::InstancedArrayComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string & vertexPath, const string & fragmentPath) : camera(camera), lightsContainer(lightsContainer) {
	this->projection = projection;
	
	buildShaders(vertexPath, fragmentPath);

	setSamplers();
	sendCameraToShader();
	sendLightsContainerToShader();
	sendProjectionMatrixToShader();
}

InstancedArrayComplexShader::InstancedArrayComplexShader(const Camera & camera, const LightsContainer & lightsContainer, const mat4 & projection, const string & materialPath, const string & vertexPath, const string & fragmentPath) : InstancedArrayComplexShader(camera, lightsContainer, projection, vertexPath, fragmentPath) {
	this->material = new Material(materialPath);
	sendMaterialToShader();
}

InstancedArrayComplexShader::~InstancedArrayComplexShader() {}

void InstancedArrayComplexShader::sendToShader(Material * material) {
	this->use();
	glUniform1f(glGetUniformLocation(this->shaderId, "material.shininess"), material->getShininess());
}

void InstancedArrayComplexShader::buildShaders(const string & vertexPath, const string & fragmentPath) {
	// 1. Retrieve the source code from filepath
	string vertexCode = this->readShaderFile(vertexPath);
	string fragmentCode = this->readShaderFile(fragmentPath);

	// 2. Compile Shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Shader Program
	this->shaderId = glCreateProgram();

	// Vertex Shader
	vertex = createShader(GL_VERTEX_SHADER, vertexCode);

	// Fragment Shader
	fragment = createShader(GL_FRAGMENT_SHADER, fragmentCode);

	// Link the previously attached Shaders (glAttachShader) to the program (glCreateProgram)
	glLinkProgram(this->shaderId);
	// Print Linking Errors
	glGetProgramiv(this->shaderId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->shaderId, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// Delete the Shaders as they are Linked to our Program and are no Longer Necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

string InstancedArrayComplexShader::readShaderFile(const string & path) {
	ifstream shaderFile;
	string code;

	// Ensure ifstream objects can throw exceptions
	shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		// Open File
		shaderFile.open(path);
		stringstream shaderStream;

		// Read buffer into stream
		shaderStream << shaderFile.rdbuf();

		// Close files
		shaderFile.close();

		// Convert Stream into GLchar array
		code = shaderStream.str();
		return code;
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ : " << path << endl;
		return code;
	}
}

GLuint InstancedArrayComplexShader::createShader(GLint type, const string & code) {
	// 2. Compile Shaders
	const GLchar * shaderCode = code.c_str();
	GLuint shader;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	// Print Compile Errors
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		string shaderName = string("");
		if (type == GL_VERTEX_SHADER) {
			shaderName = string("Vertex");
		}
		else if (type == GL_FRAGMENT_SHADER) {
			shaderName = string("Fragment");
		}
		else if (type == GL_GEOMETRY_SHADER) {
			shaderName = string("Geometry");
		}
		cout << "ERROR::SHADER::" + shaderName + "::COMPILATION_FAILED\n" << infoLog << endl;
	}
	glAttachShader(this->shaderId, shader);
	return shader;
}

// This only has to be done once
void InstancedArrayComplexShader::setSamplers() {
	this->use();
	glUniform1i(glGetUniformLocation(this->shaderId, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(this->shaderId, "material.specular"), 1);
}

void InstancedArrayComplexShader::sendLightsContainerToShader() {
	const DirLight * dirLight = this->lightsContainer.getDirLight();
	const SpotLight * spotLight = this->lightsContainer.getSpotLight();
	const vector<PointLight> * pointLights = this->lightsContainer.getPointLights();

	this->use();
	// Directional light
	glUniform3f(glGetUniformLocation(this->shaderId, "dirLight.direction"), dirLight->direction.x, dirLight->direction.y, dirLight->direction.z);
	glUniform3f(glGetUniformLocation(this->shaderId, "dirLight.ambient"), dirLight->ambient.x, dirLight->ambient.y, dirLight->ambient.z);
	glUniform3f(glGetUniformLocation(this->shaderId, "dirLight.diffuse"), dirLight->diffuse.x, dirLight->diffuse.y, dirLight->diffuse.z);
	glUniform3f(glGetUniformLocation(this->shaderId, "dirLight.specular"), dirLight->specular.x, dirLight->specular.y, dirLight->specular.z);

	// Point Light
	glUniform1i(glGetUniformLocation(this->shaderId, "size"), pointLights->size());
	string iString;
	for (GLuint i = 0; i < pointLights->size(); i++) {
		iString = to_string(i);
		string pointLightString = "pointLights[" + iString + "].";
		glUniform3f(glGetUniformLocation(this->shaderId, (pointLightString + "position").c_str()), (*pointLights)[i].position.x, (*pointLights)[i].position.y, (*pointLights)[i].position.z);
		glUniform3f(glGetUniformLocation(this->shaderId, (pointLightString + "ambient").c_str()), (*pointLights)[i].ambient.x, (*pointLights)[i].ambient.y, (*pointLights)[i].ambient.z);
		glUniform3f(glGetUniformLocation(this->shaderId, (pointLightString + "diffuse").c_str()), (*pointLights)[i].diffuse.x, (*pointLights)[i].diffuse.y, (*pointLights)[i].diffuse.z);
		glUniform3f(glGetUniformLocation(this->shaderId, (pointLightString + "specular").c_str()), (*pointLights)[i].specular.x, (*pointLights)[i].specular.y, (*pointLights)[i].specular.z);
		glUniform1f(glGetUniformLocation(this->shaderId, (pointLightString + "constant").c_str()), (*pointLights)[i].constant);
		glUniform1f(glGetUniformLocation(this->shaderId, (pointLightString + "linear").c_str()), (*pointLights)[i].linear);
		glUniform1f(glGetUniformLocation(this->shaderId, (pointLightString + "quadratic").c_str()), (*pointLights)[i].quadratic);
	}

	// SpotLight
	glUniform3f(glGetUniformLocation(this->shaderId, "spotLight.ambient"), spotLight->ambient.x, spotLight->ambient.y, spotLight->ambient.z);
	glUniform3f(glGetUniformLocation(this->shaderId, "spotLight.diffuse"), spotLight->diffuse.x, spotLight->diffuse.y, spotLight->diffuse.z);
	glUniform3f(glGetUniformLocation(this->shaderId, "spotLight.specular"), spotLight->specular.x, spotLight->specular.y, spotLight->specular.z);
	glUniform1f(glGetUniformLocation(this->shaderId, "spotLight.constant"), spotLight->constant);
	glUniform1f(glGetUniformLocation(this->shaderId, "spotLight.linear"), spotLight->linear);
	glUniform1f(glGetUniformLocation(this->shaderId, "spotLight.quadratic"), spotLight->quadratic);
	glUniform1f(glGetUniformLocation(this->shaderId, "spotLight.cutOff"), cos(radians(spotLight->cutOff)));
	glUniform1f(glGetUniformLocation(this->shaderId, "spotLight.outerCutOff"), cos(radians(spotLight->outerCutOff)));
}

void InstancedArrayComplexShader::sendProjectionMatrixToShader() {
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->shaderId, "projection"), 1, GL_FALSE, value_ptr(projection));
}

void InstancedArrayComplexShader::sendCameraToShader() {
	this->use();
	glUniform3f(glGetUniformLocation(this->shaderId, "viewPos"), this->camera.Position.x, this->camera.Position.y, this->camera.Position.z);

	glUniformMatrix4fv(glGetUniformLocation(this->shaderId, "view"), 1, GL_FALSE, value_ptr(this->camera.GetViewMatrix()));

	// Set material properties
	glUniform3f(glGetUniformLocation(this->shaderId, "spotLight.position"), this->camera.Position.x, this->camera.Position.y, this->camera.Position.z);
	glUniform3f(glGetUniformLocation(this->shaderId, "spotLight.direction"), this->camera.Front.x, this->camera.Front.y, this->camera.Front.z);
}

void InstancedArrayComplexShader::sendMaterialToShader() {
	this->use();
	glUniform1f(glGetUniformLocation(this->shaderId, "material.shininess"), this->material->getShininess());
}

const LightsContainer & InstancedArrayComplexShader::getLightsContainer() {
	return this->lightsContainer;
}

void InstancedArrayComplexShader::use() {
	glUseProgram(this->shaderId);
}
