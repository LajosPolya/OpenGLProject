#include "Mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(const Mesh & toCopy) {
	this->vertices = toCopy.vertices;
	this->indices = toCopy.indices;

	this->VAO = toCopy.VAO;
	this->VBO = toCopy.VBO;
	this->EBO = toCopy.EBO;

	this->vertexProp_BitMap = toCopy.vertexProp_BitMap;
	this->primitiveType = toCopy.primitiveType;

	this->diffuseMap = new Texture{ *toCopy.diffuseMap };
	this->specularMap = new Texture{ *toCopy.specularMap };
}

Mesh::Mesh(GLchar * vertexLocation) {
	this->readVertexFile(vertexLocation);
	this->setupMesh();
}

Mesh::Mesh(GLchar * vertexLocation, Texture * diffuseMap, Texture * specularMap) {
	this->readVertexFile(vertexLocation);

	this->diffuseMap = diffuseMap;
	this->specularMap = specularMap;
	this->setupMesh();
}

void Mesh::Draw() {
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	// Bind Diffuse Map

	if (this->diffuseMap != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->diffuseMap->getTextureID());
	}

	// Bind Specular Map
	if (this->specularMap != nullptr) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->specularMap->getTextureID());
	}

	/*for (GLuint i = 0; i < this->textures.size(); i++) {
	glActiveTexture(GL_TEXTURE0 + i); // Activate Texture Unit

	// Retrieve Texture Number
	string number = "";
	string name = this->textures[i].type;

	if (name == "texture_diffuse") {
	number = to_string(diffuseNr++); // Transfer GLuint to stream
	}
	else if (name == "texture_specular") {
	number = to_string(specularNr++);
	}

	glUniform1i(glGetUniformLocation(shader.Program, ("material." + name + number).c_str()), i);
	glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}*/

	// Draw Mesh
	glBindVertexArray(this->VAO);
	if (this->EBO == -1 ) { // If EBO is not initialized
		glDrawArrays(this->primitiveType, 0, this->vertices.size());
	}
	else { // If EBO is initialized
		glDrawElements(this->primitiveType, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

GLuint Mesh::getVAO() {
	return this->VAO;
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

	glBindVertexArray(0);
}

/* Reads in a mesh file and Processes Vertices */
void Mesh::readVertexFile(GLchar * filename) {
	string line;
	ifstream file(filename);
	if (file.is_open()) {

		// The File has specific locations for each vertex property (position, normal, texture coordinates)
		getline(file, line);
		if (line[POSITION] != NULL && line[POSITION] == '1') {
			this->vertexProp_BitMap = this->vertexProp_BitMap | POSITION_BITMAP; // 00000001
		}
		if (line[NORMAL] != NULL && line[NORMAL] == '1') {
			this->vertexProp_BitMap = this->vertexProp_BitMap | NORMAL_BITMAP; // 00000010
		}
		if (line[TEXT_COORDS] != NULL && line[TEXT_COORDS] == '1') {
			this->vertexProp_BitMap = this->vertexProp_BitMap | TEXTURE_COORDS_BITMAP; // 00000100
		}

		// Get each vertex
		vector<Vertex> vertices;
		while (getline(file, line)) {
			Vertex vertex;
			GLchar * tokens;
			GLchar * context = NULL;

			// Get all numbers and convert to floats (each vertex property has at least 2 floats)
			GLfloat floatNum;
			tokens = strtok_s(&line[0], ",", &context);
			if (this->vertexProp_BitMap & POSITION_BITMAP) {
				floatNum = stof(tokens, NULL);
				vertex.Position.x = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = stof(tokens, NULL);
				vertex.Position.y = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = stof(tokens, NULL);
				vertex.Position.z = floatNum;
				tokens = strtok_s(NULL, ",", &context);
			}
			if (this->vertexProp_BitMap & NORMAL_BITMAP) {
				floatNum = stof(tokens, NULL);
				vertex.Normal.x = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = stof(tokens, NULL);
				vertex.Normal.y = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = stof(tokens, NULL);
				vertex.Normal.z = floatNum;
				tokens = strtok_s(NULL, ",", &context);
			}
			if (this->vertexProp_BitMap & TEXTURE_COORDS_BITMAP) {
				floatNum = stof(tokens, NULL);
				vertex.TexCoords.x = floatNum;

				tokens = strtok_s(NULL, ",", &context);
				floatNum = stof(tokens, NULL);
				vertex.TexCoords.y = floatNum;
			}
			vertices.push_back(vertex);
		}
		this->vertices = vertices;
		file.close();
	}
	else {
		cout << "FAILED TO OPEN MESH FILE: " << filename << endl;
		this->vertexProp_BitMap = 0;
	}
}
