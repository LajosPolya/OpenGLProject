#include "ChunkObjectTransformStore.h"



ChunkObjectTransformStore::ChunkObjectTransformStore() {
	this->tranforms = std::vector<InstancedArrayTransformImpl>();
}

ChunkObjectTransformStore::ChunkObjectTransformStore(ComplexPosition CoPo) {
	this->tranforms = { CoPo.getDrawablePositions(GRASS),
	CoPo.getDrawablePositions(COAL) };
}

ChunkObjectTransformStore::~ChunkObjectTransformStore() {}

InstancedArrayTransformImpl ChunkObjectTransformStore::getTransform(CompositionType i) {
	return this->tranforms[i];
}

