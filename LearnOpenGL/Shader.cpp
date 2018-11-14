#include "Shader.h"
#include "TransparentGameObjectImpl.h"

Shader::Shader() {}

Shader::Shader(const Shader & toCopy) {
	this->Program = toCopy.Program;
}

Shader::Shader(const string vertexPath, const string fragmentPath) {
	// 1. Retrieve the source code from filepath
	string vertexCode;
	string fragmentCode;

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
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// Delete the Shaders as they are Linked to our Program and are no Longer Necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, GLchar * geometryPath) {
	// 1. Retrieve the source code from filepath
	string vertexCode;
	string fragmentCode;
	string geometryCode;

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
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// Delete the Shaders as they are Linked to our Program and are no Longer Necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

void Shader::readShaderFile(const string path, string * code) {
	ifstream shaderFile;

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
		*code = shaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ : " << path << endl;
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
	glAttachShader(this->Program, shader);
	return shader;
}

void Shader::Use() {
	glUseProgram(this->Program);
}

void Shader::sendToShader(LightsContainer * lightsContainer) {
	DirLight * dirLight = lightsContainer->getDirLight();
	SpotLight * spotLight = lightsContainer->getSpotLight();
	vector<PointLight> * pointLights = lightsContainer->getPointLights();

	this->Use();
	// Directional light
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.direction"), dirLight->direction.x, dirLight->direction.y, dirLight->direction.z);
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.ambient"), dirLight->ambient.x, dirLight->ambient.y, dirLight->ambient.z);
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.diffuse"), dirLight->diffuse.x, dirLight->diffuse.y, dirLight->diffuse.z);
	glUniform3f(glGetUniformLocation(this->Program, "dirLight.specular"), dirLight->specular.x, dirLight->specular.y, dirLight->specular.z);

	// Point Light
	glUniform1i(glGetUniformLocation(this->Program, "size"), pointLights->size());
	string iString;
	for (GLuint i = 0; i < pointLights->size(); i++) {
		iString = to_string(i);
		string pointLightString = "pointLights[" + iString + "].";
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
	glUniform1f(glGetUniformLocation(this->Program, "spotLight.cutOff"), cos(radians(spotLight->cutOff)));
	glUniform1f(glGetUniformLocation(this->Program, "spotLight.outerCutOff"), cos(radians(spotLight->outerCutOff)));
}

void Shader::sendToShader(TransparentGameObjectImpl * gameObject) {
	this->Use();
	sendCommonToShader(gameObject);

	vector<mat4> models = gameObject->getTransform()->getModels();
	for (GLuint i = 0; i < models.size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(this->Program, ("model[" + to_string(i) + "]").c_str()), 1, GL_FALSE, value_ptr(models[i]));
	}
}

void Shader::sendToShader(Material * material) {
	this->Use();
	glUniform1f(glGetUniformLocation(this->Program, "material.shininess"), material->getShininess());
}

void Shader::setProjectionMatrix(mat4 projection) {
	this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->Program, "projection"), 1, GL_FALSE, value_ptr(projection));
}

void Shader::sendCommonToShader(TransparentGameObjectImpl * gameObject) {
	Camera camera = gameObject->getCamera();
	const vec3 cPos = camera.getPosition();
	const vec3 cFront = camera.getFront();

	glUniform3f(glGetUniformLocation(this->Program, "viewPos"), cPos.x, cPos.y, cPos.z);
	
	glUniform1i(glGetUniformLocation(this->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(this->Program, "material.specular"), 1);

	glUniformMatrix4fv(glGetUniformLocation(this->Program, "view"), 1, GL_FALSE, value_ptr(camera.GetViewMatrix()));

	// Set material properties
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.position"), cPos.x, cPos.y, cPos.z);
	glUniform3f(glGetUniformLocation(this->Program, "spotLight.direction"), cFront.x, cFront.y, cFront.z);
}

// TODO: This can allow Shaders to have variable amount of Textures
/*
for (GLuint i = 0; i < this->textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i); // Activate Texture Unit

    glUniform1i(glGetUniformLocation(shader.Program, this->textures[i].name), i);
    glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
}
*/