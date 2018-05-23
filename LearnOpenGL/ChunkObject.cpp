#include "ChunkObject.h"



ChunkObject::ChunkObject() {
	this->composition.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", std::vector<glm::vec3>()));
	this->composition.push_back(SimpleInstancedArrayGameObject("Textures/coal.jpg", "Textures/coalSpec.jpg", "Mesh/crate.txt", "Material/crate.txt", std::vector<glm::vec3>()));
}


ChunkObject::~ChunkObject() {}

SimpleInstancedArrayGameObject & ChunkObject::getChunkObject(CompositionType i) {
	return this->composition[i];
}
