#include "ChunkObject.h"



ChunkObject::ChunkObject() {
	this->composition.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", std::vector<glm::vec3>()));
	this->composition.push_back(SimpleInstancedArrayGameObject("Textures/coal.jpg", "Textures/coalSpec.jpg", "Mesh/crate.txt", std::vector<glm::vec3>()));
}

ChunkObject::~ChunkObject() {}

void ChunkObject::DrawComponents() {
	this->composition[GRASS].Draw();
	this->composition[COAL].Draw();
}

void ChunkObject::setInstances(PositionRelativeCamera pos) {
	this->composition[GRASS].setInstances(&pos.getTransform(GRASS));
	this->composition[COAL].setInstances(&pos.getTransform(COAL));
}
