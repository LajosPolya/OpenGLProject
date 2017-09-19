#pragma once

// STD
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h> 

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LightTypes.h"
#include "Texture.h"

#define POSITION 0
#define NORMAL 1
#define TEXT_COORDS 2

#define POSITION_BITMAP 1
#define NORMAL_BITMAP 2
#define TEXTURE_COORDS_BITMAP 4
#define INSTANCE_POSITION_BITMAP 8

#define INSTANCED_ARRAY_SHADER 1
#define INSTANCED_SHADER  2

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh {
public:
	// Mesh Data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::mat4> instances;

	Mesh();
	Mesh(GLchar* vertexLocation);
	Mesh(GLchar* vertexLocation, std::vector<glm::mat4> instances, GLuint type);
	Mesh(GLchar* vertexLocation, std::vector<glm::mat4> instances, GLuint type, Texture * diffuseMap, Texture * specularMap);
	Mesh(GLchar* vertexLocation, GLuint size);

	void Draw();

	void setInstance(std::vector<glm::mat4> instances);

	Texture * getDiffuseMap();
	Texture * getSpecularMap();

private:
	// Render Data
	GLuint VAO, VBO, EBO = -1;
	GLuint instanceVBO = -1;
	GLuint numInstances = 0;

	GLuint vertexProp_BitMap;
	GLuint type = 0;

	Texture * diffuseMap;
	Texture * specularMap;

	// Function
	void setupMesh();

	/* Reads in a mesh file and Processes Vertices */
	void readVertexFile(GLchar* filename);
};
