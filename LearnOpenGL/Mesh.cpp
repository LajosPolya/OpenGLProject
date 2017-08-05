#include "Mesh.h"

Mesh::Mesh() {

}

Mesh::Mesh(GLchar* vertexLocation, std::vector<Texture> textures, Shader shader) {
	this->readVertexFile(vertexLocation);
	this->textures = textures;

	/*for (GLuint i = 0; i < vertices.size(); i++) {
		std::cout << vertices[i].Position.x << " ";
		std::cout << vertices[i].Position.y << " ";
		std::cout << vertices[i].Position.z << " ";

		std::cout << vertices[i].Normal.x << " ";
		std::cout << vertices[i].Normal.y << " ";
		std::cout << vertices[i].Normal.z << " ";

		std::cout << vertices[i].TexCoords.x << " ";
		std::cout << vertices[i].TexCoords.y << " ";
		std::cout << std::endl;
	}

	std::cout << std::endl;*/

	this->setupMesh();
}

Mesh::Mesh(GLchar * vertexLocation, std::vector<Texture> textures, Shader shader, std::vector<glm::mat4> instances)
{
	this->readVertexFile(vertexLocation);
	this->instances = instances;
	this->textures = textures;

	this->setupMesh();
}

void Mesh::Draw(Shader shader, int * placeHolder) {

	for (GLuint i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); // Activate Texture Unit

		glUniform1i(glGetUniformLocation(shader.Program, this->textures[i].name), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	// Draw Mesh
	glBindVertexArray(this->VAO);
	if (EBO == -1) { // If EBO is not initialized
		glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	}
	else { // If EBO is initialized
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader) {
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	/*for (GLuint i = 0; i < this->textures.size(); i++) {
	glActiveTexture(GL_TEXTURE0 + i); // Activate Texture Unit

	// Retrieve Texture Number
	std::string number = "";
	std::string name = this->textures[i].type;

	if (name == "texture_diffuse") {
	number = std::to_string(diffuseNr++); // Transfer GLuint to stream
	}
	else if (name == "texture_specular") {
	number = std::to_string(specularNr++);
	}

	glUniform1i(glGetUniformLocation(shader.Program, ("material." + name + number).c_str()), i);
	glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}*/

	// Draw Mesh
	glBindVertexArray(this->VAO);
	if (EBO == -1 && !(this->vertexProp_BitMap & INSTANCE_POSITION_BITMAP)) { // If EBO is not initialized
		glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	}
	else if (EBO == -1 && this->vertexProp_BitMap & INSTANCE_POSITION_BITMAP) {
		glDrawArraysInstanced(GL_TRIANGLES, 0, this->vertices.size(), this->instances.size());
	}
	else { // If EBO is initialized
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
	//glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// TODO: Better impl needed for normal, instanced, or instancedArray comparison
void Mesh::setInstance(std::vector<glm::mat4> instances)
{
	this->instances = instances;
}

// Function
void Mesh::setupMesh() {
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	if (this->indices.size() != 0) {
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
	}

	// Vertex Positions
	if (this->vertexProp_BitMap & POSITION_BITMAP) {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	}


	// Vertex Normals
	if (this->vertexProp_BitMap & NORMAL_BITMAP) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	}

	// Vertex Texture Coords
	if (this->vertexProp_BitMap & TEXTURE_COORDS_BITMAP) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	}

	// Instancing
	if (this->instances.size() > 0) {
		this->vertexProp_BitMap = this->vertexProp_BitMap | INSTANCE_POSITION_BITMAP;
		glGenBuffers(1, &this->instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->instances.size(), &this->instances[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	glBindVertexArray(0);
}

/* Reads in a mesh file and Processes Vertices */
void Mesh::readVertexFile(GLchar* filename) {
	std::string line;
	std::ifstream file(filename);
	if (file.is_open()) {

		// The File has specific locations for each vertex property (position, normal, texture coordinates)
		std::getline(file, line);
		if (line[POSITION] != NULL && line[POSITION] == '1') {
			this->vertexProp_BitMap = this->vertexProp_BitMap | POSITION_BITMAP; // 00000001
			//std::cout << position << std::endl;
		}
		if (line[NORMAL] != NULL && line[NORMAL] == '1') {
			this->vertexProp_BitMap = this->vertexProp_BitMap | NORMAL_BITMAP; // 00000010
			//std::cout << normal << std::endl;
		}
		if (line[TEXT_COORDS] != NULL && line[TEXT_COORDS] == '1') {
			this->vertexProp_BitMap = this->vertexProp_BitMap | TEXTURE_COORDS_BITMAP; // 00000100
			//std::cout << textCoords << std::endl;
		}

		// Get each vertex
		std::vector<Vertex> vertices;
		while (std::getline(file, line)) {
			Vertex vertex;
			GLchar * tokens;
			GLchar* context = NULL;

			// Get all numbers and convert to floats (each vertex property has at least 2 floats)
			GLfloat floatNum;
			tokens = strtok_s(&line[0], ",", &context);
			if (this->vertexProp_BitMap & POSITION_BITMAP) {
				floatNum = std::stof(tokens, NULL);
				vertex.Position.x = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = std::stof(tokens, NULL);
				vertex.Position.y = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = std::stof(tokens, NULL);
				vertex.Position.z = floatNum;
				tokens = strtok_s(NULL, ",", &context);
			}
			if (this->vertexProp_BitMap & NORMAL_BITMAP) {
				floatNum = std::stof(tokens, NULL);
				vertex.Normal.x = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = std::stof(tokens, NULL);
				vertex.Normal.y = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = std::stof(tokens, NULL);
				vertex.Normal.z = floatNum;
				tokens = strtok_s(NULL, ",", &context);
			}
			if (this->vertexProp_BitMap & TEXTURE_COORDS_BITMAP) {
				floatNum = std::stof(tokens, NULL);
				vertex.TexCoords.x = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = std::stof(tokens, NULL);
				vertex.TexCoords.y = floatNum;
			}
			vertices.push_back(vertex);
		}
		this->vertices = vertices;
	}
	else {
		std::cout << "FAILED TO OPEN FILE" << std::endl;
		this->vertexProp_BitMap = 0;
	}
}
