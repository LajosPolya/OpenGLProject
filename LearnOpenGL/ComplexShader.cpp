#include "ComplexShader.h"

ComplexShader::ComplexShader(Camera * camera, LightsContainer * lightsContainer, glm::mat4 projection, const GLchar* vertexPath, const GLchar* fragmentPath) {
	this->camera = camera;
	this->lighstContainer = lightsContainer;
	this->projection = projection;
	buildShaders(vertexPath, fragmentPath);
}

ComplexShader::~ComplexShader() {}

void ComplexShader::buildShaders(const GLchar * vertexPath, const GLchar * fragmentPath) {
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
	this->shaderId = glCreateProgram();

	// Vertex Shader
	vertex = createShader(GL_VERTEX_SHADER, vShaderCode);

	// Fragment Shader
	fragment = createShader(GL_FRAGMENT_SHADER, fShaderCode);

	// Link the previously attached Shaders (glAttachShader) to the program (glCreateProgram)
	glLinkProgram(this->shaderId);
	// Print Linking Errors
	glGetProgramiv(this->shaderId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the Shaders as they are Linked to our Program and are no Longer Necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void ComplexShader::readShaderFile(const GLchar * path, std::string * code) {
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

GLuint ComplexShader::createShader(GLint type, const GLchar * code) {
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
	glAttachShader(this->shaderId, shader);
	return shader;
}
