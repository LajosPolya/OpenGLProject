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

#include "Texture.h"

using namespace std;
using namespace glm;

#define POSITION 0
#define NORMAL 1
#define TEXT_COORDS 2

#define POSITION_BITMAP 1
#define NORMAL_BITMAP 2
#define TEXTURE_COORDS_BITMAP 4

// TODO: Don't send Transform to Constructor
/* It doesn't make sense to send the constructor over because it's only used 
   to see if it's instanced or not. The Mesh should have isntanced children 
*/

struct Vertex {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};

class Mesh {
public:
	Mesh();
	Mesh(const Mesh & toCopy);
	Mesh(GLchar * vertexLocation);
	Mesh(GLchar * vertexLocation, Texture * diffuseMap, Texture * specularMap);

	virtual void Draw();

	GLuint getVAO();

protected:
	// Mesh Data
	vector<Vertex> vertices;
	vector<GLuint> indices;

	// Render Data
	GLuint VAO = (GLuint)-1;
	GLuint EBO = (GLuint)-1;
	GLuint VBO = (GLuint)-1;

	GLuint vertexProp_BitMap;

	Texture * diffuseMap = nullptr;
	Texture * specularMap = nullptr;

	// Initialize to GL_TRIANGLES
	GLuint primitiveType = GL_TRIANGLES;

	virtual void setupMesh();

	/* Reads in a mesh file and Processes Vertices */
	void readVertexFile(GLchar * filename);
};
