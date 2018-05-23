#include "ChunkObjectTransformStore.h"



ChunkObjectTransformStore::ChunkObjectTransformStore() {
	this->tranforms = std::vector<InstancedArrayTransformImpl>();
}

ChunkObjectTransformStore::ChunkObjectTransformStore(InstancedArrayTransformImpl one, InstancedArrayTransformImpl two) {
	this->tranforms.push_back(one);
	this->tranforms.push_back(two);
}


ChunkObjectTransformStore::~ChunkObjectTransformStore() {}

InstancedArrayTransformImpl ChunkObjectTransformStore::getTransform(CompositionType i) {
	return this->tranforms[i];
}

