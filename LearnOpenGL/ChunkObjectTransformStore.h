#pragma once

#include <vector>

#include "InstancedArrayTransformImpl.h"
#include "CompositionType.h"
#include "ComplexPosition.h"

using namespace std;

class ChunkObjectTransformStore
{
public:
	ChunkObjectTransformStore();
	ChunkObjectTransformStore(ComplexPosition CoPo);

	~ChunkObjectTransformStore();

	InstancedArrayTransformImpl getTransform(CompositionType i);

private:
	vector<InstancedArrayTransformImpl> tranforms;
};

