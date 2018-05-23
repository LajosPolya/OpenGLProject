#pragma once

#include <vector>

#include "InstancedArrayTransformImpl.h"
#include "CompositionType.h"

class ChunkObjectTransformStore
{
public:
	ChunkObjectTransformStore();
	ChunkObjectTransformStore(InstancedArrayTransformImpl one, InstancedArrayTransformImpl two);
	~ChunkObjectTransformStore();

	InstancedArrayTransformImpl getTransform(CompositionType i);

private:
	std::vector<InstancedArrayTransformImpl> tranforms;
};

