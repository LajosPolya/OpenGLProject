#pragma once

#ifndef MESH_H
#define MESH_H

// STD
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GLM
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "LightTypes.h"
#include "Texture.h"
#include "Shader.h"

#define POSITION 0
#define NORMAL 1
#define TEXT_COORDS 2

#define POSITION_BITMAP 1
#define NORMAL_BITMAP 2
#define TEXTURE_COORDS_BITMAP 4
#define INSTANCE_POSITION_BITMAP 8

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
	std::vector<Texture> textures;
	std::vector<glm::vec3> instances;

	Mesh();

	Mesh(GLchar* vertexLocation, std::vector<Texture> textures, Shader shader);
	Mesh(GLchar* vertexLocation, GLchar* instanceLoc, std::vector<Texture> textures, Shader shader);

	
	void Draw(Shader shader, int * placeHolder);

	void Draw(Shader shader);

private:
	// Render Data
	GLuint VAO, VBO, EBO = -1;
	GLuint instanceVBO = -1;

	GLuint vertexProp_BitMap;

	// Function
	void setupMesh();

	/* Reads in a mesh file and Processes Vertices */
	void readVertexFile(GLchar* filename);

	/* Reads in an instance file and saves the position */
	void readInstanceFile(GLchar* filename);
};

#endif // !MESH_H