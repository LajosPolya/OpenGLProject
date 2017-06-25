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

#define POSITION 0
#define NORMAL 1
#define TEXT_COORDS 2

#define POSITION_BITMAP 1
#define NORMAL_BITMAP 2
#define TEXTURE_COORDS_BITMAP 4

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

	Mesh();

	Mesh(GLchar* vertexLocation, std::vector<Texture> textures, Shader shader);

	
	void Draw(Shader shader, int * placeHolder);

	void Draw(Shader shader);

private:
	// Render Data
	GLuint VAO, VBO, EBO = -1;

	// Function
	void setupMesh(GLuint vertexProp_BitMap);

	/* Reads in a mesh file and Processes Vertices */
	GLint readVertexFile(GLchar* filename);
};

#endif // !MESH_H