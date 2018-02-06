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
#include "Transform.h"
#include "InstancedArrayTransformImpl.h"
#include "InstancedTransformImpl.h"

#define POSITION 0
#define NORMAL 1
#define TEXT_COORDS 2

#define POSITION_BITMAP 1
#define NORMAL_BITMAP 2
#define TEXTURE_COORDS_BITMAP 4
#define INSTANCE_POSITION_BITMAP 8

#define INSTANCED_SHADER  2

// TODO: Don't send Transform to Constructor
/* It doesn't make sense to send the constructor over because it's only used 
   to see if it's instanced or not. The Mesh should have isntanced children 
*/

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh {
public:
	Mesh();
	Mesh(const Mesh & toCopy);
	Mesh(GLchar * vertexLocation);
	Mesh(GLchar * vertexLocation, Texture * diffuseMap, Texture * specularMap);
	Mesh(GLchar * vertexLocation, Transform * transform, Texture * diffuseMap, Texture * specularMap);
	Mesh(GLchar * vertexLocation, std::vector<glm::mat4> instances, GLuint type);
	Mesh(GLchar * vertexLocation, std::vector<glm::mat4> instances, GLuint type, Texture * diffuseMap, Texture * specularMap);
	Mesh(GLchar * vertexLocation, InstancedTransformImpl * transform, Texture * diffuseMap, Texture * specularMap);
	Mesh(GLchar * vertexLocation, std::vector<glm::mat4> instances, GLuint type, GLuint primitiveType, Texture * diffuseMap, Texture * specularMap);
	Mesh(GLchar * vertexLocation, InstancedTransformImpl * transform, GLuint primitiveType, Texture * diffuseMap, Texture * specularMap);
	Mesh(GLchar * vertexLocation, GLuint size);

	virtual void Draw();

	virtual void setInstance(std::vector<glm::mat4> instances);

	Texture * getDiffuseMap();
	Texture * getSpecularMap();

	GLuint getInstancedVBO();
	GLuint getVAO();

protected:
	// Mesh Data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::mat4> instances;

	// Render Data
	GLuint numInstances = (GLuint)0;
	GLuint VAO = (GLuint)-1, EBO = (GLuint)-1, VBO = (GLuint)-1;
	GLuint instanceVBO = (GLuint)-1;

	GLuint vertexProp_BitMap;

	Texture * diffuseMap = nullptr;
	Texture * specularMap = nullptr;

	// Initialize to GL_TRIANGLES
	GLuint primitiveType = GL_TRIANGLES;

	virtual void setupMesh();

	/* Reads in a mesh file and Processes Vertices */
	void readVertexFile(GLchar * filename);

private:

	// Instanced or InstancedArray
	GLuint type = 0;

	void setupMesh(Transform * transform);
	void setupMesh(InstancedTransformImpl * transform);
};
