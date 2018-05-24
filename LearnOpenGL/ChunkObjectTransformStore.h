#pragma once

#include <vector>

#include "InstancedArrayTransformImpl.h"
#include "CompositionType.h"
#include "ComplexPosition.h"

class ChunkObjectTransformStore
{
public:
	ChunkObjectTransformStore();
	ChunkObjectTransformStore(ComplexPosition CoPo);

	~ChunkObjectTransformStore();

	InstancedArrayTransformImpl getTransform(CompositionType i);

private:
	std::vector<InstancedArrayTransformImpl> tranforms;
};

